"""
Zero Trust Hybrid AI Gateway - Local SSL Generator
--------------------------------------------------
This script automates the generation of self-signed X.509 certificates.
It is executed securely during the installation process to ensure that
all local traffic between the C++ frontend and Python backend is 
encrypted via HTTPS (TLS 1.2+), preventing local packet sniffing.

Note: API Key management is deliberately omitted here. In a Zero Trust 
architecture, the Python backend remains stateless. The API key is securely 
stored and injected per-request by the C++ client via HTTP headers.
"""

import datetime
from datetime import timezone
import ipaddress
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import hashes
from cryptography.x509.oid import NameOID
from cryptography import x509

def generate_local_ssl_certificates():
    """
    Generates a 2048-bit RSA keypair and a self-signed certificate bound 
    to 127.0.0.1 (localhost) valid for 10 years.
    """
    print("[SYSTEM] 🔒 Zero Trust Security Setup Initiated...")
    print("[SYSTEM] ⚙️ Generating Local SSL/TLS Certificates...")
    
    # Generate 2048-bit RSA Private Key
    key = rsa.generate_private_key(
        public_exponent=65537, 
        key_size=2048
    )
    
    # Define the subject (localhost)
    subject = issuer = x509.Name([
        x509.NameAttribute(NameOID.COMMON_NAME, u"127.0.0.1"),
    ])
    
    # Modern approach for UTC time (avoids deprecation warnings in Python 3.12+)
    now_utc = datetime.datetime.now(timezone.utc)
    
    # Build the Certificate
    cert = x509.CertificateBuilder().subject_name(
        subject
    ).issuer_name(
        issuer
    ).public_key(
        key.public_key()
    ).serial_number(
        x509.random_serial_number()
    ).not_valid_before(
        now_utc
    ).not_valid_after(
        now_utc + datetime.timedelta(days=3650)
    ).add_extension(
        x509.SubjectAlternativeName([
            x509.DNSName(u"localhost"), 
            x509.IPAddress(ipaddress.IPv4Address("127.0.0.1"))
        ]),
        critical=False,
    ).sign(key, hashes.SHA256())

    # Save Private Key (server.key)
    with open("server.key", "wb") as f:
        f.write(key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.TraditionalOpenSSL,
            encryption_algorithm=serialization.NoEncryption()
        ))

    # Save Public Certificate (server.crt)
    with open("server.crt", "wb") as f:
        f.write(cert.public_bytes(serialization.Encoding.PEM))

    print("[SUCCESS] ✅ server.crt and server.key generated successfully.")
    print("[SUCCESS] 🛡️ All local traffic will now be encrypted end-to-end via HTTPS.")


if __name__ == "__main__":
    generate_local_ssl_certificates()