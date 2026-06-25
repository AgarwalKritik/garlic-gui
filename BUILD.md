<div align="center">

# Garlic Decompiler (GUI)

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

**A modern C++/Qt6 desktop interface for the Garlic Decompiler, bringing blazing-fast APK, DEX, JAR, and CLASS decompilation to Windows, Linux, and macOS.**

[![LICENSE](icon/license.svg)](LICENSE)

</div>

---

# <img src="https://api.iconify.design/heroicons:cog-8-tooth.svg?color=%233994BC" width="28" align="center" /> Build From Source

Follow these detailed instructions to compile and build GarlicGUI natively on your system.

## Prerequisites

- **CMake** (`v3.16` or newer): Used to configure the cross-platform build environment.
- **Qt6** (`v6.9.2` or newer): Required for compiling the modern GUI components.
- **C/C++ Compiler** (GCC, Clang, or MinGW): Must support the `C++17` & `C99` standard or newer. *(Note: On Windows, MinGW is highly preferred over MSVC for compatibility).*

---

## <img src="https://api.iconify.design/simple-icons:linux.svg?color=%233994BC" width="20" align="center" /> Linux (All Distros)

### 1. Install Dependencies

```bash
# Install Qt6 and required build tools
sudo apt update && sudo apt install -y qt6-base-dev cmake build-essential
```

### 2. Build the Application

```bash
# Clone the repository
git clone https://github.com/AgarwalKritik/garlic-gui.git 
cd garlic-gui

# Create build directory and generate build files
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Compile the application using CMake's build tool wrapper
cmake --build build -j$(nproc)
```

### 3. Run

```bash
# Run the compiled executable
./build/GarlicGUI
```

---

## <img src="https://api.iconify.design/simple-icons:apple.svg?color=%233994BC" width="20" align="center" /> macOS

### 1. Install Dependencies

```bash
# Install Qt6 and CMake using Homebrew
brew install qt6 cmake
```

### 2. Build the Application

```bash
# Clone the repository
git clone https://github.com/AgarwalKritik/garlic-gui.git 
cd garlic-gui

# Create build directory and configure CMake (tell CMake where Qt6 is)
cmake -B build -S . -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" -DCMAKE_BUILD_TYPE=Release

# Compile the application
cmake --build build -j$(sysctl -n hw.ncpu)
```

### 3. Run

```bash
# Run the application bundle
./build/GarlicGUI.app/Contents/MacOS/GarlicGUI
```

---

## <img src="https://api.iconify.design/simple-icons:windows.svg?color=%233994BC" width="20" align="center" /> Windows

### 1. Install Dependencies

1. Install **Qt6 (Open Source edition)** from the [Qt website](https://www.qt.io/download-open-source).
2. Add the Qt6 MinGW compiler to your `PATH` environment variable. For example, add: `C:/Qt/6.x.x/mingw_64/bin/`.
   > **Note**: Replace `6.x.x` with the exact version of Qt you downloaded.

### 2. Build the Application

Open **Command Prompt** or **PowerShell**:

```powershell
# Clone the repository
git clone https://github.com/AgarwalKritik/garlic-gui.git
cd garlic-gui

# Configure the build and point CMake to your Qt6 installation
cmake -B build -S . -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64" -DCMAKE_BUILD_TYPE=Release

# Compile the application
cmake --build build -j4
```

### 3. Run

```powershell
# Run the executable
.\build\GarlicGUI.exe
```

### 4. Build the Windows Installer (.exe) (Optional)

If you want to package your freshly built binary into a professional `.exe` system installer (complete with Desktop shortcuts and PATH integration):

1. **Download Inno Setup**: Download and install Inno Setup from [jrsoftware.org](https://jrsoftware.org/isdl.php).
2. **Add to PATH**: Ensure `iscc.exe` is in your system's `PATH`. It is usually located at `C:\Program Files\Inno Setup 6\`.
3. **Compile the Installer**: Run the following commands in the root of the repository:

```powershell
# Deploy all dependencies and binaries into a staging 'install_dir'
cmake --install build --prefix install_dir

# Run the Inno Setup Compiler against the provided script
iscc windows/installer.iss
```

Your fully packaged installer will be generated in `windows/Output/Garlic-GUI-Windows-installer-*.exe`!

---

## <img src="https://api.iconify.design/heroicons:scale.svg?color=%233994BC" width="28" align="center" /> License

#### This project is licensed under the <img src="https://api.iconify.design/simple-icons:apache.svg?color=%233994BC" width="18" align="center" /><span style="color:#3994BC">**Apache License 2.0**</span> — see [LICENSE](LICENSE) for more information  

> **Notice:** All decompilation is powered by the Garlic engine; any limitations of Garlic apply here.

---

## <img src="https://api.iconify.design/heroicons:envelope.svg?color=%233994BC" width="28" align="center" /> Contact & Links

[![Raise an issue](icon/issue.svg)](https://github.com/AgarwalKritik/garlic-gui/issues/new)
[![Kritik Agarwal](icon/agarwalkritik.svg)](https://github.com/AgarwalKritik)
[![AbhiTheModder](icon/abhithemodder.svg)](https://lin.ky/abhithemodder)

---

## <img src="https://api.iconify.design/heroicons:heart.svg?color=%233994BC" width="28" align="center" /> Credits & Acknowledgements

This project would not be possible without the incredible work of the open-source community. We would like to extend our deepest gratitude to the following projects, developers, and tools:

- **[Garlic Decompiler Engine](https://github.com/neocanable/garlic)**: Special thanks to **[neocanable](https://github.com/neocanable)** and the original Garlic contributors for creating the blazing-fast C-based decompilation engine that serves as the foundation of this application and makes it possible.
- **[AbhiTheModder](https://lin.ky/abhithemodder)**: For leading the ideation and vision of the Garlic GUI.
- **[The Qt Company](https://www.qt.io/)**: For creating and maintaining the Qt6 cross-platform framework that powers this application.
- **[CMake](https://cmake.org/)**: For providing robust cross-platform build automation.

> *If your open-source work is used in this project and we missed you, please open an issue so we can properly add you to this list!*
