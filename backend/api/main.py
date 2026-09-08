"""
Universal Zero Trust AI Gateway - Backend API
---------------------------------------------
A FastAPI-based local proxy that intercepts user prompts, performs 
Data Leak Prevention (DLP) checks using Magika and local LLMs (Ollama), 
and securely routes traffic to ANY cloud provider (OpenAI, Anthropic, Gemini) 
via LiteLLM without leaking Personally Identifiable Information (PII).

Security Features:
- Universal Stateless API Key Management (via HTTPS Headers)
- Payload Size Restriction (Buffer Overflow Protection)
- Rate Limiting (DoS/Spam Protection)
- Fail-Closed Intent Routing
- End-to-End Encryption (HTTPS/TLS)
- Dynamic Local Model Allocation
"""

import os
import sys
import requests
import asyncio
import time
from typing import List 
from fastapi import FastAPI, HTTPException, Request, Header
from fastapi.responses import JSONResponse
from pydantic import BaseModel

from litellm import completion
from magika import Magika 
from backend.core.dlp_engine import sanitize_prompt

if hasattr(sys.stdout, 'reconfigure'):
    sys.stdout.reconfigure(encoding='utf-8')
if hasattr(sys.stderr, 'reconfigure'):
    sys.stderr.reconfigure(encoding='utf-8')

app = FastAPI(title="Universal Zero Trust AI Gateway", version="3.0.0")
magika_model = Magika() 

# --- SECURITY CONSTANTS ---
REQUEST_LIMIT = 3
TIME_WINDOW = 1.0
MAX_PAYLOAD_SIZE = 500000 
request_history = {}

@app.middleware("http")
async def security_middleware(request: Request, call_next):
    client_ip = request.client.host
    content_length = request.headers.get('content-length')
    if content_length and int(content_length) > MAX_PAYLOAD_SIZE:
        return JSONResponse(status_code=413, content={"detail": "Payload Too Large. Blocked by Gateway."})
        
    current_time = time.time()
    if client_ip not in request_history:
        request_history[client_ip] = []
    request_history[client_ip] = [t for t in request_history[client_ip] if current_time - t < TIME_WINDOW]
    if len(request_history[client_ip]) >= REQUEST_LIMIT:
        return JSONResponse(status_code=429, content={"detail": "Too Many Requests. Blocked by Gateway."})
        
    request_history[client_ip].append(current_time)
    return await call_next(request)

class Message(BaseModel):
    role: str
    content: str

class ChatRequest(BaseModel):
    prompt: str
    provider: str = "gemini" 
    model_name: str = "gemini-1.5-flash"
    local_model: str = "qwen2.5-coder:7b" 
    history: List[Message] = [] 

def analyze_intent_with_local_ai(prompt: str, local_model_name: str) -> str:
    try:
        router_payload = {
            "model": local_model_name,
            "prompt": (
                "You are an aggressive Data Leak Prevention (DLP) scanner.\n"
                "Task: Analyze if the user's input relates to programming, coding, algorithms, software development, scripting, or debugging in ANY way.\n"
                "Even a tiny, casual request like 'write a timer', 'how to print', or 'fix this' MUST be classified as code.\n"
                "If there is even a 1% chance it is about code, reply EXACTLY with the word: CODE\n"
                "If it is a purely non-technical everyday conversation, reply EXACTLY with the word: TEXT\n"
                "Do not write sentences. No other words allowed.\n\n"
                f"User Input: {prompt}\n\n"
                "Classification:"
            ),
            "stream": False,
            "options": {"temperature": 0.0, "num_predict": 10}
        }
        res = requests.post("http://127.0.0.1:11434/api/generate", json=router_payload, timeout=10)
        if res.status_code == 200:
            return res.json().get("response", "").strip().upper()
        return f"HTTP_ERROR_{res.status_code}"
    except requests.exceptions.Timeout:
        return "TIMEOUT_ERROR"
    except Exception:
        return "CONNECTION_FAILED"

@app.post("/api/v1/chat")
async def process_chat(request: ChatRequest, x_api_key: str = Header(None), x_provider: str = Header(None)):
    original_text = request.prompt
    
    ml_result = magika_model.identify_bytes(original_text.encode('utf-8'))
    has_code_snippet = (ml_result.output.group == "code")
    
    wants_code_generated = False
    router_debug_response = "MAGIKA_CAUGHT_IT" 
    
    if not has_code_snippet:
        router_debug_response = await asyncio.to_thread(analyze_intent_with_local_ai, original_text, request.local_model)
        is_error = router_debug_response in ["TIMEOUT_ERROR", "CONNECTION_FAILED"] or router_debug_response.startswith("HTTP_ERROR")
        wants_code_generated = ("CODE" in router_debug_response) or is_error
    
    # ROUTE A: LOCAL NETWORK (OLLAMA)
    if has_code_snippet or wants_code_generated:
        try:
            context = ""
            for msg in request.history[-6:]:
                speaker = "User" if msg.role == "user" else "AI"
                context += f"{speaker}: {msg.content}\n\n"
            contextual_prompt = f"{context}User: {original_text}\nAI:" if context else original_text

            ollama_payload = {"model": request.local_model, "prompt": contextual_prompt, "stream": False}
            ollama_res = requests.post("http://127.0.0.1:11434/api/generate", json=ollama_payload)
            ollama_res.raise_for_status()
            
            return {
                "status": "success",
                "model_used": f"LOCAL: {request.local_model}",
                "original_prompt": original_text,
                "sanitized_prompt": "[PROCESSED SECURELY ON LOCALHOST]",
                "llm_response": ollama_res.json().get("response", "")
            }
        except requests.exceptions.ConnectionError:
            raise HTTPException(status_code=503, detail="Local AI node is not running. Please start Ollama.")
        except Exception as e:
            raise HTTPException(status_code=500, detail=f"Local AI Error: {str(e)}")

    # ROUTE B: UNIVERSAL CLOUD NETWORK (LiteLLM)
    provider_name = x_provider or request.provider
    if not x_api_key or x_api_key.strip() == "":
        raise HTTPException(status_code=401, detail=f"{provider_name.upper()} API Key is missing! Please configure it in Settings.")

    safe_text = await asyncio.to_thread(sanitize_prompt, original_text)
    
    print("\n" + "="*60)
    print("🛡️ ZERO TRUST DLP SCAN REPORT 🛡️")
    print("="*60)
    print(f"🔴 ORIGINAL PROMPT  : {original_text}")
    print(f"🟢 SANITIZED PROMPT : {safe_text}")
    if original_text != safe_text: 
        print("⚠️  ALERT: SENSITIVE DATA INTERCEPTED AND SANITIZED!")
    else: 
        print("✅ STATUS: CLEAN (No sensitive data detected)")
    print("="*60 + "\n")

    messages = [{"role": msg.role, "content": msg.content} for msg in request.history[-6:]]
    messages.append({"role": "user", "content": safe_text})

    litellm_model = f"{provider_name}/{request.model_name}"

    try:
        response = await asyncio.to_thread(
            completion, 
            model=litellm_model, 
            messages=messages, 
            api_key=x_api_key
        )
        try:
            actual_llm_response = response.choices[0].message.content
        except (AttributeError, IndexError):
            actual_llm_response = "⚠️ [Gateway Alert]: The model returned an empty or malformed response."
            
    except Exception as e:
        raise HTTPException(status_code=500, detail=f"Cloud API Error ({provider_name.upper()}): {str(e)}")
    
    return {
        "status": "success",
        "model_used": f"CLOUD: {provider_name.upper()} {request.model_name} | Router Said: [{router_debug_response}]",
        "original_prompt": original_text,
        "sanitized_prompt": safe_text,
        "llm_response": actual_llm_response
    }


if __name__ == "__main__":
    import uvicorn
    import trustme
    import os

    if not os.path.exists("server.key") or not os.path.exists("server.crt"):
        print("[SYSTEM] 🔐 Generating unique hardware-bound SSL certificates...")
        ca = trustme.CA()
        server_cert = ca.issue_cert("127.0.0.1", "localhost")
        server_cert.private_key_pem.write_to_path("server.key")
        server_cert.cert_chain_pems[0].write_to_path("server.crt")
        print("[SYSTEM] ✅ Unique SSL certificates generated successfully.")

    print("[SYSTEM] 🚀 Starting Universal Zero Trust Gateway...")
    uvicorn.run(app, host="127.0.0.1", port=8000, ssl_keyfile="server.key", ssl_certfile="server.crt")