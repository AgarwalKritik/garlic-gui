<div align="center">

# Project Architecture & Structure

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

**A guide to the internal architecture of Garlic Decompiler GUI.**

</div>

---

## <img src="https://api.iconify.design/heroicons:map.svg?color=%233994BC" width="28" align="center" /> High-Level Overview

The project is strictly divided into two distinct layers to ensure maximum performance and maintainability:

1. **The GUI Layer (`src/gui`)**: Written in modern C++17 and Qt6. It handles all user interactions, file management, code editing, and async threading.
2. **The Core Engine (`src/garlic`)**: The original Garlic Decompiler engine written in pure C. It performs the heavy lifting of reverse-engineering binaries into Java source code.

---

## <img src="https://api.iconify.design/heroicons:folder-open.svg?color=%233994BC" width="28" align="center" /> Directory Structure

```text
garlic-gui/
├── .github/workflows/          # CI/CD Pipelines (e.g., automated release builds)
├── icon/                       # Application icons and GitHub badges
├── scripts/                    # Utility scripts (e.g., syncing core C engine)
├── shell/                      # Developer shell scripts and opcode helpers
├── CMakeLists.txt              # Primary CMake build configuration
├── resources.qrc               # Qt Resource Collection file (UI icons, styling)
├── src/
│   ├── main.cpp                # Application entry point
│   │
│   ├── gui/                    # GUI LAYER (C++ / Qt6)
│   │   ├── CodeEditorWidget    # The tabbed IDE code editor
│   │   ├── DecompilerInterface # The C++ Bridge talking to the C engine
│   │   ├── DecompilerProgressDialog # Progress dialog
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
│       │   └── transformer/    # AST transformation and translation rules
│       ├── jar/                # JAR specific handling
│       ├── jvm/                # Standard Java Virtual Machine bytecode translation
│       ├── libs/               # 📦 Standalone Utility Libraries
│       │   ├── bitset/         # Bit vector implementation
│       │   ├── hashmap/        # High-performance C hashmap
│       │   ├── list/           # Linked list data structures
│       │   ├── memory/         # Custom memory pool allocation for speed
│       │   ├── queue/          # Queue data structures
│       │   ├── str/            # String manipulation utilities
│       │   ├── threadpool/     # Multithreading job queue
│       │   ├── trie/           # Trie tree data structure
│       │   └── zip/            # Native ZIP archive extraction via Miniz
│       │
│       └── parser/             # Binary format definition parsers
│           ├── class/          # Java CLASS format parser
│           ├── dex/            # Dalvik Executable (DEX) format parser
│           ├── pe/             # Portable Executable (PE) format parser
│           └── reader/         # Binary stream reading utilities
```

---

## <img src="https://api.iconify.design/heroicons:arrows-right-left.svg?color=%233994BC" width="28" align="center" /> The C++ / C Bridge

Because the GUI is built in C++ and the Core is pure C, they communicate through a dedicated wrapper layer.
If you are contributing to the UI and need to invoke a decompiler function:

1. Ensure the function is properly exposed in `src/garlic/garlic_wrapper.h`.
2. Call it asynchronously using `src/gui/DecompilerInterface.cpp` to prevent locking up the Qt Main Event Loop.
3. Thread pooling and memory allocation for the heavy decompilation are managed internally by the `garlic/libs` C utilities.

---

## <img src="https://api.iconify.design/heroicons:arrow-path.svg?color=%233994BC" width="28" align="center" /> GUI Data Flow & Lifecycle

When a user interacts with the application, the GUI layer orchestrates the flow:

1. **File Selection**: The `WelcomeWidget` or `MainWindow` intercepts the file open request and passes the path to the `ProjectManager`.
2. **Asynchronous Dispatch**: The `DecompilerInterface` spawns a background task calling the C wrapper to prevent locking the main UI.
3. **Progress Tracking**: The backend periodically emits progress updates, which are caught by Qt signals/slots and rendered by the `DecompilerProgressDialog`.
4. **Workspace Population**: Once the decompilation finishes, the `FileTreeWidget` parses the output structure and builds a navigable tree.
5. **Code Rendering**: Clicking a node in the tree sends the raw Java text to the `CodeEditorWidget`, applying syntax highlighting and rendering the code.

---

## <img src="https://api.iconify.design/heroicons:bolt.svg?color=%233994BC" width="28" align="center" /> Threading & Responsiveness

A major design goal of the Garlic GUI is maintaining a smooth, 60fps responsive interface even when decompiling multi-gigabyte APKs.

* **Qt Event Loop Protection**: Decompilation tasks are strictly kept off the main Qt thread. 
* **Signal-Slot Communication**: The `DecompilerInterface` acts as the bridge, safely marshaling cross-thread updates back to the UI thread using Qt's thread-safe signal/slot mechanism.
* **Non-blocking UI**: Users can cancel operations, browse existing code, and resize windows without any stutter while the engine works in the background.

---

## <img src="https://api.iconify.design/heroicons:document-duplicate.svg?color=%233994BC" width="28" align="center" /> Workspace & Project Management

Managing multiple decompiled files (e.g., loading `classes.dex`, then adding `classes2.dex`) is handled by the `ProjectManager`.

* **State Tracking**: Keeps track of currently open workspaces and prompts the user before clearing the active context.
* **Exporting**: When a user selects "Export Source", the manager delegates the bundling of the active memory workspace into a clean ZIP archive directly to disk.

---

## <img src="https://api.iconify.design/heroicons:wrench-screwdriver.svg?color=%233994BC" width="28" align="center" /> Build System & Qt Toolchain

The project relies on CMake to seamlessly blend C++ GUI code with the pure C backend.

* **CMake Qt Modules**: Automates the compilation of `.ui`, `.moc`, and `.qrc` files.
* **Resource Management**: The `resources.qrc` file bundles all SVG icons, fonts, and dark mode stylesheet definitions directly into the executable, ensuring no missing assets on the user's end.
* **Cross-Platform**: The CMake scripts dynamically adjust linking paths so the GUI builds natively on MSVC/MinGW (Windows), GCC (Linux), and Clang (macOS).

---

## <img src="https://api.iconify.design/heroicons:computer-desktop.svg?color=%233994BC" width="28" align="center" /> Key GUI Components Deep-Dive

The user interface consists of several advanced modular components:

* **`CodeEditorWidget`**: A highly customized text editor widget. It supports active line highlighting, line numbers, cursor position tracking, and advanced syntax coloration.
* **`FindReplaceWidget`**: An integrated search dock that interacts directly with the text document, providing full Regex matching, case sensitivity, and whole-word filtering.
* **`FileTreeWidget`**: A custom hierarchical tree view designed to lazily load and efficiently display thousands of decompiled Java classes.

---

## <img src="https://api.iconify.design/heroicons:users.svg?color=%233994BC" width="28" align="center" /> Contributors

* [Kritik Agarwal](https://github.com/AgarwalKritik)
* [AbhiTheModder](https://github.com/AbhiTheModder)
