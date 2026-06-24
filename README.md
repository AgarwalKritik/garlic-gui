<div align="center">

# Garlic Decompiler (GUI)

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

**A modern C++/Qt6 desktop interface for the Garlic Decompiler, bringing blazing-fast APK, DEX, JAR, and CLASS decompilation to Windows, Linux, and macOS.**

[![Open Source](icon/open-source.svg)](https://opensource.org/)
[![LICENSE](icon/license.svg)](LICENSE)
[![C++](icon/cpp.svg)](#built-with)
[![QT6](icon/powered-by-qt6.svg)](https://www.qt.io/)
[![garlic](icon/garlic-powered.svg)](https://github.com/neocanable/garlic)
[![Kritik Agarwal](icon/developed.svg)](https://github.com/AgarwalKritik)

</div>

---

## <img src="https://api.iconify.design/heroicons:book-open.svg?color=%233994BC" width="28" align="center" /> Table of Contents

- [About The Project](#about-the-project)
  - [Key Features](#key-features)
  - [Built With](#built-with)
  - [Project Structure](#project-structure)
- [Getting Started](#getting-started)
  - [Usage](#usage)
  - [Download (Releases)](#download-releases)
  - [Build From Source](#build-from-source)
- [Roadmap](#roadmap)
- [Contributors](#contributors)
- [Contributing](#contributing)
- [License](#license)
- [Contact & Links](#contact--links)
- [Credits & Acknowledgements](#credits--acknowledgements)

---

## <img src="https://api.iconify.design/heroicons:light-bulb.svg?color=%233994BC" width="28" align="center" /> About The Project

**Garlic Decompiler GUI** is a modern C++/Qt6 graphical interface for the [Garlic Decompiler](https://github.com/neocanable/garlic). It allows you to:

- Open, decompile, browse, and export Java source code from Android/Java binaries **seamlessly**.
- Work with `APK`, `DEX`, `JAR`, and `CLASS` files at lightning speed.
- Navigate projects using a VS Code-inspired tabbed editor with precise syntax highlighting.

> Garlic Decompiler is a high-performance, ultra-fast decompilation tool for reconstructing Java source code directly from raw binaries without needing external Java runtimes.

---

### <img src="https://api.iconify.design/heroicons:sparkles.svg?color=%233994BC" width="24" align="center" /> Key Features

| Feature | Description |
| ------- | ----------- |
| <img src="https://api.iconify.design/heroicons:paint-brush.svg?color=%233994BC" width="20" align="center" /> **Modern IDE Aesthetic** | Built with a beautiful VS Code Dark 2026 theme, complete with a minimal status bar, cursor position tracking (line/column indicators), and advanced syntax highlighting. |
| <img src="https://api.iconify.design/heroicons:magnifying-glass.svg?color=%233994BC" width="20" align="center" /> **Advanced Code Editor** | Fully featured code editor with an integrated Find/Replace dock supporting Regular Expressions (Regex), Match Case, and Whole Word searches. |
| <img src="https://api.iconify.design/heroicons:rocket-launch.svg?color=%233994BC" width="20" align="center" /> **Multithreaded Extraction** | Decompile massive APKs without freezing the UI, thanks to a robust C/C++ multithreading bridge. |
| <img src="https://api.iconify.design/heroicons:chart-bar.svg?color=%233994BC" width="20" align="center" /> **Real-time Progress** | Watch decompilation happening live with a native status bar and progress tracking system. |

---

### <img src="https://api.iconify.design/heroicons:wrench-screwdriver.svg?color=%233994BC" width="24" align="center" /> Built With

- **C++17** — The robust backend language for the GUI shell.
- **Qt6** — Industry-standard cross-platform framework for rendering the modern interface.
- **Garlic Decompiler Engine** — The core pure-C decompilation logic.

---

### <img src="https://api.iconify.design/heroicons:folder-open.svg?color=%233994BC" width="24" align="center" /> Project Structure

Want to dive into the code? The project is carefully separated into a C++ UI layer and a C Core layer.

> For the full, detailed architectural tree, please see our **[STRUCTURE.md](STRUCTURE.md)** guide!

---

## <img src="https://api.iconify.design/heroicons:rocket-launch.svg?color=%233994BC" width="28" align="center" /> Getting Started

### <img src="https://api.iconify.design/heroicons:play.svg?color=%233994BC" width="24" align="center" /> Usage

1. **Open APK/CLASS/JAR/DEX File**: Click "Open File..." on the Welcome dashboard or use `Ctrl+O`.
2. **Wait for Decompilation**: The progress bar natively updates you in the status bar footer.
3. **Browse Code**: Use the file tree on the left to navigate decompiled classes.
4. **View Source**: Click on any Java file to open it in the editor.
5. **Search**: Press `Ctrl+F` to open the Find/Replace dock and search with Regex.
6. **Save/Export**: Use the File menu to save or export your project.

### <img src="https://api.iconify.design/heroicons:cube.svg?color=%233994BC" width="24" align="center" /> Download (Releases)

You can download the latest compiled releases directly from the [GitHub Releases](https://github.com/AgarwalKritik/garlic-gui/releases) page. We provide pre-built binaries for:

- **Windows**: `.msi` system installer and `.zip` portable versions available.
- **macOS**: `.dmg` disk image.
- **Linux**: `.tar.gz` archive.

### <img src="https://api.iconify.design/heroicons:cog-8-tooth.svg?color=%233994BC" width="24" align="center" /> Build From Source

<details>
<summary><b>Click to show/hide Build Instructions</b></summary>

#### Prerequisites

- CMake 3.16 or later

- Qt6 (GUI Components)
- C/C++ compiler (GCC, Clang, MinGW)

#### <img src="https://api.iconify.design/simple-icons:linux.svg?color=%233994BC" width="20" align="center" /> Linux (All Distros)

```bash
# Install Qt6 and required build tools
sudo apt update && sudo apt install -y qt6-base-dev cmake build-essential

# Clone the repository and enter the directory
git clone https://github.com/AgarwalKritik/garlic-gui.git && cd garlic-gui

# Create build directory and generate build files
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Compile the application using CMake's build tool wrapper
cmake --build build -j$(nproc)

# Run the application
./build/GarlicGUI
```

#### <img src="https://api.iconify.design/simple-icons:apple.svg?color=%233994BC" width="20" align="center" /> macOS

```bash
# Install Qt6 and CMake using Homebrew
brew install qt6 cmake

# Clone the repository and enter the directory
git clone https://github.com/AgarwalKritik/garlic-gui.git && cd garlic-gui

# Create build directory and configure CMake (tell CMake where Qt6 is)
cmake -B build -S . -DCMAKE_PREFIX_PATH="$(brew --prefix qt6)" -DCMAKE_BUILD_TYPE=Release

# Compile the application
cmake --build build -j$(sysctl -n hw.ncpu)

# Run the application
./build/GarlicGUI.app/Contents/MacOS/GarlicGUI
```

#### <img src="https://api.iconify.design/simple-icons:windows.svg?color=%233994BC" width="20" align="center" /> Windows

1. Install **Qt6 (Open Source edition)** from [Qt website](https://www.qt.io/download-open-source).
2. Add Qt6 compiler (MinGW) to your `PATH`, e.g., `C:/Qt/6.x.x/mingw_64/bin/`.
   > **Note**: Replace ```6.x.x``` with the exact version of Qt you downloaded.

3. Open **Command Prompt** or **PowerShell**, then:

```powershell
# Clone the repository and enter the directory
git clone https://github.com/AgarwalKritik/garlic-gui.git
cd garlic-gui

# Configure the build and point CMake to your Qt6 installation
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64" -DCMAKE_BUILD_TYPE=Release

# Compile the application
cmake --build build -j4
```

1. Run the application:

```powershell
# Run the executable
.\build\GarlicGUI.exe
```

</details>

---

## <img src="https://api.iconify.design/heroicons:map.svg?color=%233994BC" width="28" align="center" /> Roadmap

Here's what we have accomplished and what we are looking forward to:

- [x] Embed Garlic C source code
- [x] Match Garlic CLI CMake configuration
- [x] Support APK, DEX, JAR, CLASS with native detection
- [x] Apply same compiler flags & optimizations as Garlic
- [x] Cross-platform GUI for Windows & Linux
- [x] Modern Dark IDE UI with full status bar tracking
- [x] Full Code editing support with Regex Find/Replace
- [ ] Add project workspace configurations

---

## <img src="https://api.iconify.design/heroicons:users.svg?color=%233994BC" width="28" align="center" /> Contributors

- [Kritik Agarwal](https://github.com/AgarwalKritik) - Designed and developed the Garlic GUI.
- [AbhiTheModder](https://lin.ky/abhithemodder) - Conceived the idea for the Garlic GUI and assisted with testing, feedback, and crash reporting.

---

## <img src="https://api.iconify.design/heroicons:document-text.svg?color=%233994BC" width="28" align="center" /> Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Make changes and test on Windows, Linux, or macOS.
4. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
5. Push to the Branch (`git push origin feature/AmazingFeature`)
6. Open a Pull Request

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
