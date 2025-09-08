# Ethernet Frame Parser (C)

A lightweight C program to parse raw Ethernet II frames and display their components in a human-readable format.  
Think of it as a **mini Wireshark-lite**, focused only on the Ethernet layer.

---

## 🎯 Purpose

This project demonstrates how raw Ethernet II frames can be decoded into human-readable components.  
It serves as a stepping stone towards understanding how professional tools like Wireshark dissect packets,  
while keeping the focus on simplicity and C programming fundamentals.

---

## 🚀 Features

- Extracts and prints:
  - **Destination MAC Address**
  - **Source MAC Address**
  - **EtherType** (e.g., IPv4, ARP, IPv6)
  - **Payload (raw data)**
- Supports:
  - **Phase 1**: Parse a hardcoded Ethernet frame
  - **Phase 2**: Command-line interface (CLI) to read frames from a hex dump file or arguments
  - **Phase 3 (Optional)**: Extensions (multi-frame parsing, IPv4/ARP header parsing, error handling)
- Clean and formatted output for readability.

---

## 📦 Ethernet Frame Format

![ethernet frame diagram](EthernetFrameDiagram.jpg)

Example:

```
FF FF FF FF FF FF   08 00 27 13 69 EA   08 00   45 00 ...
|<-- Dest MAC -->|  |<-- Src MAC ---->| |ET |  |<-- Payload...
```

---

## 🛠️ Usage

### Basic (Phase 1)

Hardcoded frame inside the program:

```bash
gcc parser.c -o parser
./parser
```

Output:

```
Destination MAC: FF:FF:FF:FF:FF:FF
Source MAC: 08:00:27:13:69:EA
EtherType: 0x0800 (IPv4)
Payload length: 46 bytes
Payload (first 16 bytes): 45 00 00 54 a6 f2 40 00 40 01 b7 e6 c0 a8 00 68
```

### CLI / File Input (Phase 2)

Read from hex dump file:

```bash
./parser frame.hex
```

Read directly from CLI argument:

```bash
./parser "FF FF FF FF FF FF 08 00 27 13 69 EA 08 00 45 00 ..."
```

---

## List of Commands (To be run after compiling the main file)

- Hardcoded test frame
  `./parser`

- Read hex dump from file
  `./parser frame.txt`

- Provide hex bytes directly via CLI
  `./parser 00 1A 2B 3C 4D 5E 00 0C 29 AA BB CC 08 00`

- Show help message
  `./parser -h`

## 🎯 Learning Goals

- Understand Ethernet II frame structure:
  - MAC addressing
  - EtherType values
  - Payload handling
- Practice **C programming**:
  - Byte-level data parsing
  - Hexadecimal printing
  - CLI arguments & file I/O
- Learn how raw packet data can be interpreted (like Wireshark does).

---

## 🔑 Skills Demonstrated

- Low-level network programming
- Byte manipulation & hexadecimal formatting
- C programming (arrays, pointers, file I/O)
- Understanding of Ethernet II frame structure
- Building CLI-based tools

---

## 📚 References

- [Ethernet II Frame Format](https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II)
- [IEEE EtherType Values](https://www.iana.org/assignments/ethernet-numbers/ethernet-numbers.xhtml)
- [Wireshark](https://www.wireshark.org/) – for inspiration and testing
