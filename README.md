<div align="center">

# Garlic Decompiler (GUI)

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

**A modern C++/Qt6 GUI front-end for the Garlic Decompiler — bringing blazing-fast APK/DEX/JAR/CLASS decompilation power to your desktop.**

[![License](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNzIuMDk5OTk4NDc0MTIxMSIgaGVpZ2h0PSIzNSIgdmlld0JveD0iMCAwIDI3Mi4wOTk5OTg0NzQxMjExIDM1Ij48cmVjdCB3aWR0aD0iMTU0IiBoZWlnaHQ9IjM1IiBmaWxsPSIjZjVhNjIzIi8+PHJlY3QgeD0iMTU0IiB3aWR0aD0iMTE4LjA5OTk5ODQ3NDEyMTEiIGhlaWdodD0iMzUiIGZpbGw9IiNlYzcwMDAiLz48dGV4dCB4PSI3NyIgeT0iMjEuNSIgZm9udC1zaXplPSIxMiIgZm9udC1mYW1pbHk9IidSb2JvdG8nLCBzYW5zLXNlcmlmIiBmaWxsPSIjZmRmZGZkIiB0ZXh0LWFuY2hvcj0ibWlkZGxlIiBsZXR0ZXItc3BhY2luZz0iMiI+TElDRU5TRUQgVU5ERVI8L3RleHQ+PHRleHQgeD0iMjEzLjA0OTk5OTIzNzA2MDU1IiB5PSIyMS41IiBmb250LXNpemU9IjEyIiBmb250LWZhbWlseT0iJ01vbnRzZXJyYXQnLCBzYW5zLXNlcmlmIiBmaWxsPSIjRkZGRkZGIiB0ZXh0LWFuY2hvcj0ibWlkZGxlIiBmb250LXdlaWdodD0iOTAwIiBsZXR0ZXItc3BhY2luZz0iMiI+QVBBQ0hFIDIuMDwvdGV4dD48L3N2Zz4=)](LICENSE)
[![C++](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxNTcuODMzMzMwMTU0NDE4OTUiIGhlaWdodD0iMzUiIHZpZXdCb3g9IjAgMCAxNTcuODMzMzMwMTU0NDE4OTUgMzUiPjxyZWN0IHdpZHRoPSIxMDYuNjY2NjY0MTIzNTM1MTYiIGhlaWdodD0iMzUiIGZpbGw9IiNmMDAwMDAiLz48cmVjdCB4PSIxMDYuNjY2NjY0MTIzNTM1MTYiIHdpZHRoPSI1MS4xNjY2NjYwMzA4ODM3OSIgaGVpZ2h0PSIzNSIgZmlsbD0iI2M3MWQxZCIvPjx0ZXh0IHg9IjUzLjMzMzMzMjA2MTc2NzU4IiB5PSIyMS41IiBmb250LXNpemU9IjEyIiBmb250LWZhbWlseT0iJ1JvYm90bycsIHNhbnMtc2VyaWYiIGZpbGw9IiNmZGZkZmQiIHRleHQtYW5jaG9yPSJtaWRkbGUiIGxldHRlci1zcGFjaW5nPSIyIj5NQURFIFdJVEg8L3RleHQ+PHRleHQgeD0iMTMyLjI0OTk5NzEzODk3NzA1IiB5PSIyMS41IiBmb250LXNpemU9IjEyIiBmb250LWZhbWlseT0iJ01vbnRzZXJyYXQnLCBzYW5zLXNlcmlmIiBmaWxsPSIjRkZGRkZGIiB0ZXh0LWFuY2hvcj0ibWlkZGxlIiBmb250LXdlaWdodD0iOTAwIiBsZXR0ZXItc3BhY2luZz0iMiI+QysrPC90ZXh0Pjwvc3ZnPg==)](#built-with)
[![Qt6](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxNTkuMTE2NjY0ODg2NDc0NiIgaGVpZ2h0PSIzNSIgdmlld0JveD0iMCAwIDE1OS4xMTY2NjQ4ODY0NzQ2IDM1Ij48cmVjdCB3aWR0aD0iMTA2LjY2NjY2NDEyMzUzNTE2IiBoZWlnaHQ9IjM1IiBmaWxsPSIjNjVmMzMxIi8+PHJlY3QgeD0iMTA2LjY2NjY2NDEyMzUzNTE2IiB3aWR0aD0iNTIuNDUwMDAwNzYyOTM5NDUiIGhlaWdodD0iMzUiIGZpbGw9IiM0ZGQ1MzgiLz48dGV4dCB4PSI1My4zMzMzMzIwNjE3Njc1OCIgeT0iMjEuNSIgZm9udC1zaXplPSIxMiIgZm9udC1mYW1pbHk9IidSb2JvdG8nLCBzYW5zLXNlcmlmIiBmaWxsPSIjZmRmZGZkIiB0ZXh0LWFuY2hvcj0ibWlkZGxlIiBsZXR0ZXItc3BhY2luZz0iMiI+TUFERSBXSVRIPC90ZXh0Pjx0ZXh0IHg9IjEzMi44OTE2NjQ1MDUwMDQ4OCIgeT0iMjEuNSIgZm9udC1zaXplPSIxMiIgZm9udC1mYW1pbHk9IidNb250c2VycmF0Jywgc2Fucy1zZXJpZiIgZmlsbD0iI0ZGRkZGRiIgdGV4dC1hbmNob3I9Im1pZGRsZSIgZm9udC13ZWlnaHQ9IjkwMCIgbGV0dGVyLXNwYWNpbmc9IjIiPlFUNjwvdGV4dD48L3N2Zz4=)](https://www.qt.io/)

</div>

---

## Table of Contents

* [About The Project](#about-the-project)
  * [Built With](#built-with)
  * [Project Structure](#project-structure)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgments](#acknowledgments)
* [Notice](#notice)
* [TL;DR](#tldr)
  * [Usage](#usage)

---

## About The Project

**Garlic Decompiler GUI** is a modern C++/Qt6 graphical interface for the [Garlic Decompiler](https://github.com/neocanable/garlic). It allows you to:

* Open, decompile, browse, and export Java source code from Android/Java binaries
* Work with APK, DEX, JAR, and CLASS files
* Navigate projects using a dark-themed, tabbed editor with syntax highlighting

> Garlic Decompiler is optimized for reconstructing Java source code from `.class`, `.jar`, `.dex`, and `.apk` files.

### Built With

* [C++17](https://isocpp.org/)
* [Qt6](https://www.qt.io/) (GUI Components)
* [Garlic Decompiler](https://github.com/neocanable/garlic)

### Project Structure

```
garlic-gui/
└── src/
    ├── main.cpp                           # Entry point
    ├── gui/                               # Qt6 GUI components
    │   ├── MainWindow.cpp/h               # Main window
    │   ├── FileTreeWidget.cpp/h           # File navigation tree
    │   ├── CodeEditorWidget.cpp/h         # Tabbed code editor
    │   ├── DecompilerInterface.cpp/h      # C++ wrapper for Garlic
    │   ├── ProjectManager.cpp/h           # Project management
    │   └── DecompilerProgressDialog.cpp/h # Progress dialog
    └── garlic/                            # Garlic integration
        ├── garlic_wrapper.c/h             # C interface wrapper
        └── ...                            # Garlic "C" source files (from neocanable/garlic)

```

> For Complete Project Structure See [STRUCTURE.md](STRUCTURE.md)

---

## Getting Started

### Prerequisites

* CMake 3.16 or later
* Qt6 (GUI Components)
* C/C++ compiler (GCC, Clang, MinGW, or MSVC)

### Installation

#### Linux

```bash
# Install Qt6 and build tools
sudo apt install qt6-base-dev cmake build-essential

# Clone the repository
git clone https://github.com/AgarwalKritik/garlic-gui.git && cd garlic-gui

# Create build directory and compile
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run the application
./GarlicGUI
```

#### Windows

1. Install **Qt6 (Open Source edition)** from [Qt website](https://www.qt.io/download-open-source).
2. Add Qt6 compiler (MinGW) to your `PATH`, e.g., `C:/Qt/6.x.x/mingw_64/bin/`.
> **Note**: Replace ```6.x.x``` with the exact version of Qt you downloaded.
3. Open **Command Prompt** or **PowerShell**, then:

```cmd
git clone https://github.com/AgarwalKritik/garlic-gui.git && cd garlic-gui
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build . --config Release
windeployqt6 GarlicGUI.exe
```

4. Run the application:

```cmd
GarlicGUI.exe
```

---

## Roadmap

The roadmap includes both completed and future goals. Here's what we have accomplished and looking forward to:

- [x] Embed Garlic C source code
- [x] Match Garlic CLI CMake configuration
- [x] Support APK, DEX, JAR, CLASS with native detection
- [x] Apply same compiler flags & optimizations as Garlic
- [ ] Cross-platform GUI for Windows & Linux
- [x] Working Linux GUI
- [ ] Windows GUI
- [ ] Code editing support

> **Future Scope**: Build Garlic entirely in C++ if feasible.

---

## Contributers:

- [Kritik Agarwal](https://github.com/AgarwalKritik)
- [AbhiTheModder](https://lin.ky/abhithemodder)

---

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement". Don't forget to give the project a star! Thanks again!

1. Fork the repository
2. Create a feature branch (```git checkout -b feature/AmazingFeature```)
3. Make changes and test on both Windows & Linux
4. Commit your Changes (```git commit -m 'Add some AmazingFeature'```)
5. Push to the Branch (```git push origin feature/AmazingFeature```)
4. Open a Pull Request

---

## License

This project is distributed and licensed under the **Apache License 2.0** — see [LICENSE](LICENSE) for more information.

---

## Contact
If you have any questions or suggestions, feel free to reach out to us:

[![Raise an issue](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI0MDIuMTgzMzM0MzUwNTg1OTQiIGhlaWdodD0iMzUiIHZpZXdCb3g9IjAgMCA0MDIuMTgzMzM0MzUwNTg1OTQgMzUiPjxyZWN0IHdpZHRoPSIyMDUuMTE2NjY4NzAxMTcxODgiIGhlaWdodD0iMzUiIGZpbGw9IiM0YTkwZTIiLz48cmVjdCB4PSIyMDUuMTE2NjY4NzAxMTcxODgiIHdpZHRoPSIxOTcuMDY2NjY1NjQ5NDE0MDYiIGhlaWdodD0iMzUiIGZpbGw9IiMwNDY2ZDgiLz48dGV4dCB4PSIxMDIuNTU4MzM0MzUwNTg1OTQiIHk9IjIxLjUiIGZvbnQtc2l6ZT0iMTIiIGZvbnQtZmFtaWx5PSInUm9ib3RvJywgc2Fucy1zZXJpZiIgZmlsbD0iI2ZkZmRmZCIgdGV4dC1hbmNob3I9Im1pZGRsZSIgbGV0dGVyLXNwYWNpbmc9IjIiPlJBSVNFIEFOIElTU1VFIE9OIFRIRTwvdGV4dD48dGV4dCB4PSIzMDMuNjUwMDAxNTI1ODc4OSIgeT0iMjEuNSIgZm9udC1zaXplPSIxMiIgZm9udC1mYW1pbHk9IidNb250c2VycmF0Jywgc2Fucy1zZXJpZiIgZmlsbD0iI0ZGRkZGRiIgdGV4dC1hbmNob3I9Im1pZGRsZSIgZm9udC13ZWlnaHQ9IjkwMCIgbGV0dGVyLXNwYWNpbmc9IjIiPkdJVEhVQiBSRVBPU0lUT1JZPC90ZXh0Pjwvc3ZnPg==)](https://github.com/AgarwalKritik/garlic-gui/issues/new)
 [![AbhiTheModder](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIzMDEuNzgzMzQwNDU0MTAxNTYiIGhlaWdodD0iMzUiIHZpZXdCb3g9IjAgMCAzMDEuNzgzMzQwNDU0MTAxNTYgMzUiPjxyZWN0IHdpZHRoPSIxMzcuMTE2NjY4NzAxMTcxODgiIGhlaWdodD0iMzUiIGZpbGw9IiNkMDAyMWIiLz48cmVjdCB4PSIxMzcuMTE2NjY4NzAxMTcxODgiIHdpZHRoPSIxNjQuNjY2NjcxNzUyOTI5NyIgaGVpZ2h0PSIzNSIgZmlsbD0iIzllMTAwMiIvPjx0ZXh0IHg9IjY4LjU1ODMzNDM1MDU4NTk0IiB5PSIyMS41IiBmb250LXNpemU9IjEyIiBmb250LWZhbWlseT0iJ1JvYm90bycsIHNhbnMtc2VyaWYiIGZpbGw9IiNmZGZkZmQiIHRleHQtYW5jaG9yPSJtaWRkbGUiIGxldHRlci1zcGFjaW5nPSIyIj5DT05ORUNUIFdJVEg8L3RleHQ+PHRleHQgeD0iMjE5LjQ1MDAwNDU3NzYzNjcyIiB5PSIyMS41IiBmb250LXNpemU9IjEyIiBmb250LWZhbWlseT0iJ01vbnRzZXJyYXQnLCBzYW5zLXNlcmlmIiBmaWxsPSIjRkZGRkZGIiB0ZXh0LWFuY2hvcj0ibWlkZGxlIiBmb250LXdlaWdodD0iOTAwIiBsZXR0ZXItc3BhY2luZz0iMiI+QUJISVRIRU1PRERFUjwvdGV4dD48L3N2Zz4=)](https://lin.ky/abhithemodder)

---

## Acknowledgments

[![Garlic Decompiler](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyMDQuNjY2NjY3OTM4MjMyNDIiIGhlaWdodD0iMzUiIHZpZXdCb3g9IjAgMCAyMDQuNjY2NjY3OTM4MjMyNDIgMzUiPjxyZWN0IHdpZHRoPSI3Ni42NjY2Njc5MzgyMzI0MiIgaGVpZ2h0PSIzNSIgZmlsbD0iIzRhNGE0YSIvPjxyZWN0IHg9Ijc2LjY2NjY2NzkzODIzMjQyIiB3aWR0aD0iMTI4IiBoZWlnaHQ9IjM1IiBmaWxsPSIjMDAwMDAwIi8+PHRleHQgeD0iMzguMzMzMzMzOTY5MTE2MjEiIHk9IjIxLjUiIGZvbnQtc2l6ZT0iMTIiIGZvbnQtZmFtaWx5PSInUm9ib3RvJywgc2Fucy1zZXJpZiIgZmlsbD0iI2ZkZmRmZCIgdGV4dC1hbmNob3I9Im1pZGRsZSIgbGV0dGVyLXNwYWNpbmc9IjIiPkdBUkxJQzwvdGV4dD48dGV4dCB4PSIxNDAuNjY2NjY3OTM4MjMyNDIiIHk9IjIxLjUiIGZvbnQtc2l6ZT0iMTIiIGZvbnQtZmFtaWx5PSInTW9udHNlcnJhdCcsIHNhbnMtc2VyaWYiIGZpbGw9IiNGRkZGRkYiIHRleHQtYW5jaG9yPSJtaWRkbGUiIGZvbnQtd2VpZ2h0PSI5MDAiIGxldHRlci1zcGFjaW5nPSIyIj5ERUNPTVBJTEVSPC90ZXh0Pjwvc3ZnPg==)](https://github.com/neocanable/garlic)
[![QT6](data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIzMTMuNzgzMzY3MTU2OTgyNCIgaGVpZ2h0PSIzNSIgdmlld0JveD0iMCAwIDMxMy43ODMzNjcxNTY5ODI0IDM1Ij48cmVjdCB3aWR0aD0iMjYxLjM4MzM2MTgxNjQwNjI1IiBoZWlnaHQ9IjM1IiBmaWxsPSIjNGE0YTRhIi8+PHJlY3QgeD0iMjYxLjM4MzM2MTgxNjQwNjI1IiB3aWR0aD0iNTIuNDAwMDA1MzQwNTc2MTciIGhlaWdodD0iMzUiIGZpbGw9IiMwMDAwMDAiLz48dGV4dCB4PSIxMzAuNjkxNjgwOTA4MjAzMTIiIHk9IjIxLjUiIGZvbnQtc2l6ZT0iMTIiIGZvbnQtZmFtaWx5PSInUm9ib3RvJywgc2Fucy1zZXJpZiIgZmlsbD0iI2ZkZmRmZCIgdGV4dC1hbmNob3I9Im1pZGRsZSIgbGV0dGVyLXNwYWNpbmc9IjIiPkNST1NTLVBMQVRGT1JNIEZSQU1FV09SSzwvdGV4dD48dGV4dCB4PSIyODcuNTgzMzY0NDg2Njk0MzQiIHk9IjIxLjUiIGZvbnQtc2l6ZT0iMTIiIGZvbnQtZmFtaWx5PSInTW9udHNlcnJhdCcsIHNhbnMtc2VyaWYiIGZpbGw9IiNGRkZGRkYiIHRleHQtYW5jaG9yPSJtaWRkbGUiIGZvbnQtd2VpZ2h0PSI5MDAiIGxldHRlci1zcGFjaW5nPSIyIj5RVDY8L3RleHQ+PC9zdmc+)](https://www.qt.io/)

---

## Notice

* Currently, **Windows GUI is under development**. Linux GUI is functional.
* All decompilation is powered by the Garlic engine; any limitations of Garlic apply here.

---

## TL;DR

Garlic Decompiler GUI is a modern C++/Qt6 graphical interface for the [Garlic Decompiler](https://github.com/neocanable/garlic) — a lightning-fast decompiler for APK, DEX, JAR, and CLASS files.  
It lets you open, decompile, browse, and export Java source code from Android/Java binaries in a clean dark-themed UI. 

### Usage

1. **Open APK/CLASS/JAR/DEX File**: Click "Open APK/CLASS/JAR/DEX..." or use Ctrl+O
2. **Wait for Decompilation**: The progress dialog shows decompilation status
3. **Browse Code**: Use the file tree on the left to navigate decompiled classes
4. **View Source**: Click on any Java file to open it in the editor
5. **Save/Export**: Use the File menu to save or export your project