<div align="center">

# Garlic Decompiler (GUI)

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="256" />

**A modern C++/Qt6 desktop interface for the Garlic Decompiler, bringing blazing-fast APK, DEX, JAR, and CLASS decompilation to Windows, Linux, and macOS.**

[![Open Source](icon/open-source.svg)](https://opensource.org/)
[![LICENSE](icon/license.svg)](LICENSE)
[![C++](icon/cpp.svg)](#-built-with)
[![QT6](icon/powered-by-qt6.svg)](https://www.qt.io/)
[![garlic](icon/garlic-powered.svg)](https://github.com/neocanable/garlic)
[![Kritik Agarwal](icon/developed.svg)](https://github.com/AgarwalKritik)

</div>

---

## <img src="https://api.iconify.design/heroicons:shield-check.svg?color=%233994BC" width="28" align="center" /> Ethical & Legal Use

Garlic Decompiler GUI is intended for legitimate software analysis and reverse engineering, including:

- Security research
- Malware analysis
- Software interoperability
- Digital preservation
- Education and learning
- Reverse engineering of software that you own or are explicitly authorized to analyze

Users are solely responsible for ensuring their use of this software complies with applicable laws, software licenses, intellectual property rights, and organizational policies.

This project does **not** endorse or encourage unauthorized access, software piracy, copyright infringement, or any unlawful activity.

---

## <img src="https://api.iconify.design/heroicons:book-open.svg?color=%233994BC" width="28" align="center" /> Table of Contents

- [About The Project](#-about-the-project)
  - [Key Features](#-key-features)
  - [Built With](#-built-with)
  - [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
  - [Build From Source](#-build-from-source)
  - [Download (Releases)](#-download-releases)
  - [Usage](#-usage)
- [Roadmap](#-roadmap)
- [Contributors](#-contributors)
- [Contributing](#-contributing)
- [License](#-license)
- [Contact & Links](#-contact--links)
- [Credits & Acknowledgements](#-credits--acknowledgements)

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

### <img src="https://api.iconify.design/heroicons:cog-8-tooth.svg?color=%233994BC" width="24" align="center" /> Build From Source

Interested in compiling GarlicGUI yourself or building the native system installers?

Please see our comprehensive **[Build Guide](BUILD.md)** for detailed, step-by-step instructions for Windows, macOS, and Linux.

### <img src="https://api.iconify.design/heroicons:cube.svg?color=%233994BC" width="24" align="center" /> Download (Releases)

You can download the latest compiled releases directly from the [GitHub Releases](https://github.com/AgarwalKritik/garlic-gui/releases) page. We provide pre-built binaries for:

- **Windows**: `.exe` system installer and `.zip` portable versions available.
- **macOS**: `.dmg` disk image and `.app` bundle.
- **Linux**: `.deb`, `.rpm`, `.AppImage`, and `.tar.gz` available.

### <img src="https://api.iconify.design/heroicons:play.svg?color=%233994BC" width="24" align="center" /> Usage

1. **Open APK/CLASS/JAR/DEX File**: Click "Open File..." on the Welcome dashboard or use `Ctrl+O`.
2. **Wait for Decompilation**: The progress bar natively updates you in the status bar footer.
3. **Browse Code**: Use the file tree on the left to navigate decompiled classes.
4. **View Source**: Click on any Java file to open it in the editor.
5. **Search**: Press `Ctrl+F` to open the Find/Replace dock and search with Regex.
6. **Save/Export**: Use the File menu to save or export your project.

---

## <img src="https://api.iconify.design/heroicons:map.svg?color=%233994BC" width="28" align="center" /> Roadmap

Here's what we have accomplished and what we are looking forward to:

- [x] **Embed Garlic C source code**: Directly integrated the core Garlic C decompilation engine into the Qt6 build system for native execution without external binary dependencies.
- [x] **Match Garlic CLI CMake configuration**: Replicated the exact build steps, macros, and include directories used by the original command-line tool.
- [x] **Native file detection**: Automatically parse file headers to seamlessly identify and process APK, DEX, JAR, and CLASS files perfectly.
- [x] **Apply original compiler flags & optimizations**: Maintained the exact `-O3` and aggressive compiler optimization flags used by the core engine to ensure maximum decompilation speed.
- [x] **Cross-platform GUI**: Successfully target and natively compile Windows `.exe`, Linux `.AppImage`/`.deb`/`.rpm`, and macOS `.dmg`/`.app` bundles.
- [x] **Modern Dark IDE UI**: Designed a sleek, VS Code-inspired dark interface with a comprehensive status bar tracking progress and output logs.
- [x] **Full Code editing support**: Integrated a highly responsive text editor featuring active line highlighting, line numbers, and full Regex Find/Replace capabilities.
- [x] **Multiple file explore support**: When opening a new file (like `classes3.dex`) while another is already decompiled, prompt the user to either clear the workspace or keep the current files open to allow simultaneous analysis of multiple files.
- [ ] **Add project workspace configurations**: Allow users to save their current multi-file workspace and layout into a unique configuration file that can be instantly reloaded later.

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

> **Notice**
>
> Garlic Decompiler GUI is a graphical interface for the Garlic decompiler engine. The decompilation engine itself is developed and maintained independently by the Garlic project. Any limitations, supported formats, or decompilation behavior originate from the underlying Garlic engine.

---

## <img src="https://api.iconify.design/heroicons:envelope.svg?color=%233994BC" width="28" align="center" /> Contact & Links

[![Raise an issue](icon/issue.svg)](https://github.com/AgarwalKritik/garlic-gui/issues/new)
[![Kritik Agarwal](icon/agarwalkritik.svg)](https://github.com/AgarwalKritik)
[![AbhiTheModder](icon/abhithemodder.svg)](https://lin.ky/abhithemodder)

---

## <img src="https://api.iconify.design/heroicons:heart.svg?color=%233994BC" width="28" align="center" /> Credits & Acknowledgements

Garlic Decompiler GUI is built upon the excellent work of several open-source projects.

- **[Garlic Decompiler Engine](https://github.com/neocanable/garlic)** — Developed by **[neocanable](https://github.com/neocanable)** and contributors. Garlic GUI embeds the original Garlic C engine without modification to its core decompilation logic.
- **[AbhiTheModder](https://lin.ky/abhithemodder)** — Original concept for a graphical interface, project ideation, testing, and feedback throughout development.
- **[The Qt Company](https://www.qt.io/)** — Cross-platform application framework.
- **[CMake](https://cmake.org/)** — Cross-platform build system.

*If your work is included in this project and has not been acknowledged, please open an issue or pull request so proper attribution can be added.*
