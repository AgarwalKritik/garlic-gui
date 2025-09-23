# Garlic Decompiler GUI

A modern, cross-platform GUI written in C++ for the [Garlic Decompiler](https://github.com/neocanable/garlic) — the world’s fastest APK/CLASS/JAR/DEX decompiler written in C.

> Garlic Decompiler is a high-performance tool for reconstructing Java source code from .class, .jar, .dex and .apk files.

## Features

- **Fast Decompilation**: Powered by the high-performance Garlic decompiler engine
- **Modern GUI**: Clean, dark-themed interface built with Qt6
- ~~**Cross-Platform**: Runs on Windows and Linux~~
- **Project Management**: Organize and navigate decompiled code with ease
- **Syntax Highlighting**: Java syntax highlighting for better code readability
- **Tabbed Editor**: Open multiple files simultaneously
- **Search Functionality**: Quickly find files in large projects
- **Export Options**: Save decompiled projects to any directory

## Requirements

- CMake 3.16 or later
- Qt6 with Widgets module
- C/C++ compiler (GCC, Clang, Mingw64 or MSVC)

## Building

### Linux

```bash
# Install Qt6
sudo apt install qt6-base-dev cmake build-essential

# Clone and build
git clone <repository-url>
cd garlic-gui
mkdir build && cd build
cmake ..
make -j4
```

### Windows

1. Install Qt6 open source edition from the [official website](https://www.qt.io/download-open-source).
2. CMake and a C++ compiler is already provided with Qt6 add it to the Environment Variables Path ("C:/Qt/6.x.x/mingw_64/bin/")
3. Open Command Prompt or PowerShell:

```cmd
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build . --config Release
windeployqt6 GarlicGUI.exe
```

## Usage

1. **Open APK/CLASS/JAR/DEX File**: Click "Open APK/CLASS/JAR/DEX..." or use Ctrl+O
2. **Wait for Decompilation**: The progress dialog shows decompilation status
3. **Browse Code**: Use the file tree on the left to navigate decompiled classes
4. **View Source**: Click on any Java file to open it in the editor
5. **Save/Export**: Use the File menu to save or export your project

## Project Structure

```
└──garlic-gui/
   └── src/
      ├── main.cpp                           # Software Entry Point
      ├── gui/                               # Qt6 GUI components
      │   ├── MainWindow.cpp/h               # Main Software Window
      │   ├── FileTreeWidget.cpp/h           # File Navigation Tree
      │   ├── CodeEditorWidget.cpp/h         # Tabbed Code Editor
      │   ├── DecompilerInterface.cpp/h      # C++ wrapper for Garlic
      │   ├── ProjectManager.cpp/h           # Project management
      │   └── DecompilerProgressDialog.cpp/h # Progress dialog
      └── garlic/                            # Garlic decompiler integration
         ├── garlic_wrapper.c/h              # C interface wrapper
         ├── # Garlic Decompiler Source files copied directly from (neocanable/garlic)
```

> For Complete Project Structure See [STRUCTURE.md](./STRUCTURE.md)

## To Do:
~~- [x] Direct embedding of Garlic's C source code~~
~~- [x] Use the similar CMake type configuration as in Garlic CLI~~
~~- [x] Support all formats APK, DEX, JAR, and CLASS files with native detection~~
~~- [x] Same compiler flags and optimizations as Garlic~~
- [ ] **Cross-Platform Support**: Windows and Linux support with proper threading
~~- [x] Working GUI for Linux~~
- [ ] Working GUI for Windows
- [ ] Code Editing Support

### Future Scope:
- [ ] Build Garlic entirely into a C++ Project instead of C (if feasible)

### Setup Instructions

1. **Copy Garlic Source**: Copy the entire `src/` directory from Garlic repository to `src/garlic/`:

   ```bash
   # From the Garlic repository root
   cp -r src/ /path/to/garlic-gui/src/garlic/
   ```

2. **Build**: The CMake configuration uses Garlic's exact build system:

   ```bash
   mkdir build && cd build
   cmake ..
   make -j4
   ```

3. **Dependencies**: Same as Garlic CLI - no additional dependencies required

### Supported File Types

- **APK Files**: Android application packages
- **DEX Files**: Dalvik executable files  
- **JAR Files**: Java archive files
- **CLASS Files**: Individual Java class files

All file types are detected using Garlic's native magic number detection system.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on both Windows and Linux
5. Submit a pull request

## License

This project is licensed under the same terms as the [Garlic Decompiler](https://github.com/neocanable/garlic/blob/cc5c8d073dc8a22bbb631491df092a846095853d/LICENSE).

## Acknowledgments

- **Garlic Decompiler**: <https://github.com/neocanable/garlic>
- **JADX**: Inspiration for the GUI Design
- **Qt6**: Cross-platform GUI Framework

## Authors

- [AbhiTheModder](https://github.com/AbhiTheModder)
- [Kritik Agarwal](https://github.com/AgarwalKritik)
