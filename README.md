 #  Universal Zero Trust AI Gateway


*A secure AI proxy router built with a modern C++ (Qt) frontend and a powerful Python (FastAPI) backend.*


[]()

[]()

[]()

[]()




---


##  System Features


- **Zero Trust Architecture:** Operates on a strict **"Fail-Closed"** security model. If any security check or local model verification fails, data is strictly prevented from leaving the local device.

- **Smart Local Routing:** Automatically detects code or sensitive queries via Magika and local AI models (e.g., Qwen), forcing confidential information to stay within your local network.

- **Deep Learning DLP Engine:** Equipped with Microsoft Presidio and HuggingFace Transformers to scan and protect Personally Identifiable Information (PII) like keys, tokens, emails, and financial data.

- **Dynamic Hardware-Bound SSL:** Generates unique, self-signed cryptographic certificates on first launch for secure local communication between the C++ frontend and Python backend.

- **Multi-Provider Cloud & Local Support:** Seamlessly connects to major cloud APIs (**OpenAI, Anthropic, Gemini**) or runs entirely offline via **Ollama**.

---


## Running & Building from Source


### Prerequisites

* **Python 3.10+** ➔ [Download Official](https://www.python.org/downloads/) | Windows: `winget install Python.Python.3.11`

* **C++ Compiler (MSVC / MinGW) & CMake** ➔ [MSVC Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/) | [CMake](https://cmake.org/download/)
  * *Windows:* `winget install Kitware.CMake`
  * *macOS:* `brew install cmake`
  * *Linux (Debian/Ubuntu):* `sudo apt install cmake`

* **Qt 6.x SDK** ➔ [Qt Online Installer](https://www.qt.io/download-open-source) *(Select Qt 6.x during installation)*

* **Ollama** *(running locally for the smart routing engine)* ➔ [Download Ollama](https://ollama.com/download)
  * *Windows:* `winget install Ollama.Ollama`
  * *Linux/macOS:* `curl -fsSL https://ollama.com/install.sh | sh`


### 1. Backend Setup (Python)

Open a terminal in the root project directory and execute:


```bash

# Create and activate virtual environment

python -m venv venv

venv\Scripts\activate


# Install required dependencies

pip install -r requirements.txt


# Package the backend using PyInstaller

pyinstaller main.spec

```


### 2. Frontend Setup (C++ / Qt & CMake)

Navigate to the client directory and compile the C++ application:


```bash

cd desktop_client

mkdir build && cd build

cmake ..

cmake --build . --config Release

```


### 3. Running the Integrated System

After building both components, manually structure your release directory as follows:

1. Create a folder named `ZeroTrust_Release` (or any release folder you prefer).

2. Copy the built C++ frontend (`ZeroTrustAI.exe`) directly into this root release folder.

3. Inside this release folder, create a subfolder named `backend_engine`.

4. Copy the compiled backend executable (`main.exe`) and its `_internal` folder from `dist/main/` into the `backend_engine` subfolder.

Your final directory structure must look like this:

```
ZeroTrust_Release/
├── ZeroTrustAI.exe
└── backend_engine/
    ├── main.exe
    └── _internal/
```

5. Run `ZeroTrustAI.exe` to spin up the secure gateway launcher and the Python backend simultaneously.

---

Built with strict security and local privacy by default.

