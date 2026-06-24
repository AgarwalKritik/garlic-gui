<div align="center">

# Project Architecture & Structure

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

**A guide to the internal architecture of Garlic Decompiler GUI.**

</div>

---

## High-Level Overview

The project is strictly divided into two distinct layers to ensure maximum performance and maintainability:
1. **The GUI Layer (`src/gui`)**: Written in modern C++17 and Qt6. It handles all user interactions, file management, code editing, and async threading.
2. **The Core Engine (`src/garlic`)**: The original Garlic Decompiler engine written in pure C. It performs the heavy lifting of reverse-engineering binaries into Java source code.

---

## Directory Structure

```text
garlic-gui/
├── .github/workflows/          # CI/CD Pipelines (e.g., automated release builds)
├── icon/                       # Application icons and GitHub badges
├── src/
│   ├── main.cpp                # Application entry point
│   │
│   ├── gui/                    # 🎨 GUI LAYER (C++ / Qt6)
│   │   ├── CodeEditorWidget    # The tabbed IDE code editor
│   │   ├── DecompilerInterface # The C++ Bridge talking to the C engine
│   │   ├── DecompilerProgress  # Progress dialog
│   │   ├── FileTreeWidget      # Project workspace navigation
│   │   ├── FindReplaceWidget   # Regex search & replace module
│   │   ├── MainWindow          # The primary application shell and status bar
│   │   ├── ProjectManager      # Handles loading and exporting project files
│   │   └── WelcomeWidget       # The landing dashboard UI
│   │
│   └── garlic/                 # ⚙️ CORE ENGINE LAYER (C)
│       ├── garlic_wrapper.c/h  # The interface wrapper exposing C functions to C++
│       ├── garlic.c            # The primary engine execution module
│       │
│       ├── apk/                # APK parsing and AndroidManifest handling
│       ├── common/             # Base tools, types, and logging macros
│       ├── dalvik/             # Dalvik (DEX) bytecode parsing, Smali generation, and optimization
│       ├── decompiler/         # The brain: AST generation, control flow, and expression parsing
│       ├── jar/                # JAR specific handling
│       ├── jvm/                # Standard Java Virtual Machine bytecode translation
│       ├── libs/               # 📦 Standalone Utility Libraries
│       │   ├── hashmap/        # High-performance C hashmap
│       │   ├── memory/         # Custom memory pool allocation for speed
│       │   ├── threadpool/     # Multithreading job queue
│       │   └── zip/            # Native ZIP archive extraction via Miniz
│       │
│       └── parser/             # Binary format definition parsers (Class, DEX, PE)
```

---

## The C++ / C Bridge

Because the GUI is built in C++ and the Core is pure C, they communicate through a dedicated wrapper layer.
If you are contributing to the UI and need to invoke a decompiler function:
1. Ensure the function is properly exposed in `src/garlic/garlic_wrapper.h`.
2. Call it asynchronously using `src/gui/DecompilerInterface.cpp` to prevent locking up the Qt Main Event Loop.
3. Thread pooling and memory allocation for the heavy decompilation are managed internally by the `garlic/libs` C utilities.

---

## Contributors

* [Kritik Agarwal](https://github.com/AgarwalKritik)
* [AbhiTheModder](https://github.com/AbhiTheModder)