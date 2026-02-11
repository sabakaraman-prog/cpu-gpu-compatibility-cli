# CPU–GPU Compatibility CLI (C)

A bidirectional command-line tool written in C that recommends compatible CPU–GPU pairings based on real-world performance tiers.

This project allows users to:
- Enter a **CPU** and receive **GPU recommendations**, or
- Enter a **GPU** and receive **CPU recommendations**

The tool focuses on robust input parsing, modular design, and practical hardware considerations such as bottlenecks and performance balance.

---

##  Features
- Bidirectional recommendations (CPU → GPU or GPU → CPU)
- Case-insensitive input handling
- Supports common GPU suffixes (Ti, Super, XT, XTX)
- Defensive input validation
- Modular design using C structs and helper functions
- Clean CLI user flow with menu-based selection

---

##  Technologies
- **Language:** C (C99)
- **Concepts:** Structs, string parsing, input validation, modular design
- **Tools:** GCC, Make (optional)

---

## Example Usage:
What do you want to do?
1) I have a CPU → recommend GPU
2) I have a GPU → recommend CPU

Enter choice (1 or 2): 2

Which GPU do you have?
RTX 4070 Ti

GPU entered: rtx 4070 ti

--- CPU recommendations ---

Recommended CPUs: i7 or Ryzen 7

---

##  Why I Built This
After building my own custom gaming PC, I wanted to better understand how CPU–GPU pairing decisions affect performance and bottlenecks.  
This project was an opportunity to practice low-level programming in C while working with real-world, messy user input. This project also reflects my interest in PC hardware and systems-level software beyond web development. 

---

##  Possible Improvements
- Data-driven recommendation tables
- Bottleneck warnings based on resolution or workload
- Lightweight web interface built on top of the core logic
- Expanded support for older and mobile CPUs/GPUs

---

## License
MIT License
