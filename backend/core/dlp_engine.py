"""
Zero Trust Hybrid AI Gateway - Data Leak Prevention (DLP) Engine
----------------------------------------------------------------
This module utilizes Microsoft Presidio and HuggingFace Transformers (BERT)
to perform Deep Learning-based Named Entity Recognition (NER) combined with
strict Regex patterns (from rules.json) to catch IDs, SSNs, and Phone Numbers.

Security Features:
- Context-Aware NLP (BERT for English and Turkish)
- Cross-Border PII Detection (US SSN, UK NINO, TR Kimlik, IBAN, Global Phones)
- Thread-safe Tokenizer Execution
- Smart Memory Caching via lru_cache for performance optimization
- 100% Offline Execution (Bundled Models) to ensure Zero Trust compliance
"""

import json
import os
import sys
import logging
from functools import lru_cache
from langdetect import detect

# Explicit imports to ensure PyInstaller bundles the spaCy modules
import en_core_web_sm
import xx_ent_wiki_sm

from presidio_analyzer import AnalyzerEngine, RecognizerRegistry, PatternRecognizer, Pattern
from presidio_anonymizer import AnonymizerEngine
from presidio_analyzer.nlp_engine import NlpEngineProvider

# --- ENVIRONMENT & THREADING SAFEGUARDS ---
# Prevent deadlocks in local FastAPI servers when using HuggingFace Tokenizers
os.environ["TOKENIZERS_PARALLELISM"] = "false"
os.environ["OMP_NUM_THREADS"] = "1"

# Suppress HuggingFace symlink warnings and strictly enforce OFFLINE mode
os.environ["HF_HUB_DISABLE_SYMLINKS_WARNING"] = "1"
os.environ["HF_HUB_OFFLINE"] = "1" 

logging.getLogger("transformers").setLevel(logging.ERROR)

# --- PATH RESOLUTION FOR PYINSTALLER ---
# Determine if we are running in a bundled PyInstaller environment or a normal Python script
if getattr(sys, 'frozen', False):
    base_dir = sys._MEIPASS
    core_dir = os.path.join(base_dir, 'backend', 'core')
else:
    core_dir = os.path.dirname(__file__)

RULES_FILE = os.path.join(core_dir, "rules.json")
EN_MODEL_PATH = os.path.join(core_dir, "models", "bert-base-NER")
TR_MODEL_PATH = os.path.join(core_dir, "models", "bert-base-turkish-ner")

# --- 1. NLP ARCHITECTURE CONFIGURATION ---
configuration = {
    "nlp_engine_name": "transformers",
    "models": [
        # Pointing to the bundled local folders instead of HuggingFace Hub IDs
        {"lang_code": "en", "model_name": {"spacy": "en_core_web_sm", "transformers": EN_MODEL_PATH}},
        {"lang_code": "tr", "model_name": {"spacy": "xx_ent_wiki_sm", "transformers": TR_MODEL_PATH}}
    ],
    "ner_model_configuration": {
        "labels_to_ignore": ["O"],
        "aggregation_strategy": "simple",
        "stride": 16,
        "alignment_mode": "expand",
        "model_to_presidio_entity_mapping": {
            "PER": "PERSON",
            "LOC": "LOCATION",
            "ORG": "ORGANIZATION"
        }
    }
}

print("[SYSTEM] Initializing Offline Deep Learning DLP Engine...")

# Initialize NLP Engine using the offline local configuration
provider = NlpEngineProvider(nlp_configuration=configuration)
nlp_engine = provider.create_engine()

# --- 2. REGISTRY & CUSTOM RULE LOADING ---
registry = RecognizerRegistry()
registry.load_predefined_recognizers(nlp_engine=nlp_engine)

try:
    with open(RULES_FILE, "r", encoding="utf-8") as f:
        rules_data = json.load(f)
        
    for recognizer_data in rules_data.get("custom_recognizers", []):
        patterns = [
            Pattern(name=p["name"], regex=p["regex"], score=p["score"]) 
            for p in recognizer_data.get("patterns", [])
        ]
        registry.add_recognizer(PatternRecognizer(
            supported_entity=recognizer_data["name"], 
            patterns=patterns, 
            supported_language=recognizer_data["language"]
        ))
    print("[SYSTEM] Global Regex rules (US/UK/TR) loaded successfully.")
except FileNotFoundError:
    print(f"[WARNING] Rules file not found at {RULES_FILE}. Proceeding with default Presidio rules.")
except Exception as e:
    print(f"[ERROR] Failed to load custom rules. Error: {e}")

# --- 3. ENGINES SETUP ---
analyzer = AnalyzerEngine(nlp_engine=nlp_engine, registry=registry)
anonymizer = AnonymizerEngine()
print("[SYSTEM] DLP Engine is fully initialized and ready.")

# --- 4. SANITIZATION PIPELINE ---
@lru_cache(maxsize=1000)
def sanitize_prompt(text: str) -> str:
    """
    Scans the prompt for Global PII using both Deep Learning (Context) 
    and strict Regex (Format) patterns. Replaces sensitive data with safe tags.
    """
    if not text or not text.strip():
        return text
        
    # Fast language fallback: if text is too short or lacks alphabet characters, default to English
    if len(text.strip()) < 3:
        lang = 'en'
    else:
        try:
            lang = detect(text)
            lang = lang if lang in ['en', 'tr'] else 'en'
        except Exception:
            lang = 'en'

    # COMBINED TARGETS: NLP Defaults + Presidio Built-ins + Custom JSON Rules
    target_entities = [
        # NLP Context Targets (Names, Cities, Companies)
        "PERSON", "LOCATION", "ORGANIZATION", "EMAIL_ADDRESS", "IP_ADDRESS",
        
        # Presidio Built-in Global Targets
        "CREDIT_CARD", "CRYPTO", "US_PASSPORT", "UK_NHS", 
        
        # Custom JSON Targets (Strict Format)
        "TR_NATIONAL_ID", "TR_TAX_ID", 
        "US_SSN_CUSTOM", "UK_NINO", 
        "GLOBAL_PHONE", "IBAN_CUSTOM"
    ]   
    
    # Analyze text. Confidence threshold is set to 0.50 to catch strict regexes aggressively
    results = analyzer.analyze(
        text=text, 
        entities=target_entities, 
        language=lang, 
        score_threshold=0.50
    )
    
    return anonymizer.anonymize(text=text, analyzer_results=results).text