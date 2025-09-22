# Garlic Decompiler GUI

A modern, cross-platform GUI for the [Garlic Decompiler](https://github.com/neocanable/garlic) — the world’s fastest APK/CLASS/JAR/DEX decompiler written in C.

> Garlic Decompiler is a high-performance tool for reconstructing Java source code from .class, .jar, .dex and .apk files.

## Features

- **Fast Decompilation**: Powered by the high-performance Garlic decompiler engine
- **Modern GUI**: Clean, dark-themed interface built with Qt6
- **Cross-Platform**: Runs on Windows and Linux
- **Project Management**: Organize and navigate decompiled code with ease
- **Syntax Highlighting**: Java syntax highlighting for better code readability
- **Tabbed Editor**: Open multiple files simultaneously
- **Search Functionality**: Quickly find files in large projects
- **Export Options**: Save decompiled projects to any directory

## Requirements

- CMake 3.16 or later
- Qt6 with Widgets module
- C/C++ compiler (GCC, Clang, or MSVC)

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
2. CMake and a C++ compiler is already provided with Qt6.
3. Open Command Prompt or PowerShell:

```cmd
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/mingw_64"
cmake --build . --config Release
```

## Usage

1. **Open APK/CLASS/JAR/DEX File**: Click "Open APK/CLASS/JAR/DEX..." or use Ctrl+O
2. **Wait for Decompilation**: The progress dialog shows decompilation status
3. **Browse Code**: Use the file tree on the left to navigate decompiled classes
4. **View Source**: Click on any Java file to open it in the editor
5. **Save/Export**: Use the File menu to save or export your project

## Project Structure

```
Directory structure:
└── src/
   ├── main.cpp                    # Application entry point
   ├── gui/                        # Qt6 GUI components
   │   ├── MainWindow.cpp/h        # Main application window
   │   ├── FileTreeWidget.cpp/h    # File navigation tree
   │   ├── CodeEditorWidget.cpp/h  # Tabbed code editor
   │   ├── DecompilerInterface.cpp/h # C++ wrapper for Garlic
   │   ├── ProjectManager.cpp/h    # Project management
   │   └── DecompilerProgressDialog.cpp/h # Progress dialog
   └── garlic/                     # Garlic decompiler integration
      ├── garlic_wrapper.c/h      # C interface wrapper
      ├── garlic.c
      ├── apk/
      │   └── apk.c/h
      ├── common/
      │   ├── debug.h
      │   ├── endian_x.h
      │   ├── file_tools.h
      │   ├── output_tools.h
      │   ├── str_tools.h
      │   └── types.h
      ├── dalvik/
      │   ├── dex_annotation.c/h
      │   ├── dex_class.c/h
      │   ├── dex_decompile.c/h
      │   ├── dex_descriptor.c/h
      │   ├── dex_dump.c/h
      │   ├── dex_exception.c/h
      │   ├── dex_expression_builder.c/h
      │   ├── dex_ins.c/h
      │   ├── dex_ins_action.c/h
      │   ├── dex_ins_helper.h
      │   ├── dex_lambda.c/h
      │   ├── dex_meta_helper.c/h
      │   ├── dex_method.c/h
      │   ├── dex_optimizer.c/h
      │   ├── dex_pre_optimizer.c/h
      │   ├── dex_simulator.c/h
      │   ├── dex_smali.c/h
      │   ├── dex_structure.h
      │   ├── dex_type_analyse.c/h
      │   └── dex_writter.c
      ├── decompiler/
      │   ├── control_flow.c/h
      │   ├── debug_pointer.h
      │   ├── descriptor.c/h
      │   ├── dominator_tree.c/h
      │   ├── exception.c/h
      │   ├── expression.c/h
      │   ├── expression_analyse.c/h
      │   ├── expression_array.c/h
      │   ├── expression_assert.c/h
      │   ├── expression_assign.c/h
      │   ├── expression_branches.c/h
      │   ├── expression_chain.c/h
      │   ├── expression_copy_propgation.c/h
      │   ├── expression_enum.c/h
      │   ├── expression_exception.c/h
      │   ├── expression_goto.c/h
      │   ├── expression_helper.h
      │   ├── expression_if.c/h
      │   ├── expression_inline.c/h
      │   ├── expression_inner_class.c/h
      │   ├── expression_local_variable.c/h
      │   ├── expression_logical.c/h
      │   ├── expression_loop.c/h
      │   ├── expression_loop_type.c/h
      │   ├── expression_new.c/h
      │   ├── expression_node.c/h
      │   ├── expression_node_helper.h
      │   ├── expression_node_param.c/h
      │   ├── expression_remove_useless.c/h
      │   ├── expression_return.c/h
      │   ├── expression_synchronized.c/h
      │   ├── expression_ternary.c/h
      │   ├── expression_visitor.c/h
      │   ├── expression_writter.c/h
      │   ├── field.c/h
      │   ├── instruction.c/h
      │   ├── klass.c/h
      │   ├── method.c/h
      │   ├── scc.c/h
      │   ├── signature.c/h
      │   ├── ssa.c/h
      │   ├── stack.c/h
      │   ├── structure.h
      │   └── transformer/
      │       ├── anonymous.c
      │       ├── array_load.c
      │       ├── array_store.c
      │       ├── arraylength.c
      │       ├── assert.c
      │       ├── assignment.c
      │       ├── assignment_chain.c
      │       ├── athrow.c
      │       ├── cast.c
      │       ├── const.c
      │       ├── declaration.c
      │       ├── define_stack_var.c
      │       ├── enum.c
      │       ├── get_field.c
      │       ├── get_static.c
      │       ├── goto.c
      │       ├── if.c
      │       ├── iinc.c
      │       ├── initialize.c
      │       ├── instanceof.c
      │       ├── invoke.c
      │       ├── invokedynamic.c
      │       ├── invokeinterface.c
      │       ├── invokespecial.c
      │       ├── invokestatic.c
      │       ├── invokevirtual.c
      │       ├── lambda.c
      │       ├── local_variable.c
      │       ├── logic_not.c
      │       ├── loop.c
      │       ├── lvalue.c
      │       ├── monitorenter.c
      │       ├── monitorexit.c
      │       ├── new_array.c
      │       ├── operator.c
      │       ├── put_field.c
      │       ├── put_static.c
      │       ├── return.c
      │       ├── single_list.c
      │       ├── single_operator.c
      │       ├── stack_value.c
      │       ├── stack_var.c
      │       ├── store.c
      │       ├── str_concat.c
      │       ├── switch.c
      │       ├── ternary.c
      │       ├── transformer.c/h
      │       └── uninitialize.c
      ├── jar/
      │   └── jar.c/h
      ├── jvm/
      │   ├── jvm_annotation.c/h
      │   ├── jvm_class.c/h
      │   ├── jvm_decompile.c/h
      │   ├── jvm_descriptor.c/h
      │   ├── jvm_exception.c/h
      │   ├── jvm_expression_builder.c/h
      │   ├── jvm_ins.c/h
      │   ├── jvm_ins_action.c
      │   ├── jvm_ins_helper.h
      │   ├── jvm_lambda.c/h
      │   ├── jvm_method.c/h
      │   ├── jvm_optimizer.c/h
      │   ├── jvm_simulator.c/h
      │   └── jvm_type_analyse.c/h
      ├── libs/
      │   ├── bitset/
      │   │   ├── bitset.c/h
      │   │   └── portability.h
      │   ├── hashmap/
      │   │   ├── hashmap.c/h
      │   │   └── hashmap_tools.c/h
      │   ├── list/
      │   │   └── list.c/h
      │   ├── memory/
      │   │   ├── mem_common.h
      │   │   └── mem_pool.c/h
      │   ├── queue/
      │   │   └── queue.c/h
      │   ├── str/
      │   │   └── str.c/h
      │   ├── threadpool/
      │   │   └── threadpool.c/h
      │   ├── trie/
      │   │   └── trie_tree.c/h
      │   └── zip/
      │       └── zip.h
      └── parser/
         ├── class/
         │   ├── class_structure.h
         │   ├── class_tools.c/h
         │   ├── metadata.c/h
         │   └── opcode.c
         ├── dex/
         │   ├── dex.h
         │   ├── dex_tools.h
         │   ├── metadata.c/h
         │   └── opcode.c
         ├── pe/
         │   ├── metadata.h
         │   ├── pe.h
         │   ├── pe_const.h
         │   └── pe_tools.h
         └── reader/
            └── reader.h
```

## Integration with Garlic

**Integration Status**: ✅ **FULLY INTEGRATED**

The application now includes complete integration with the real Garlic decompiler:

1. **Native Garlic Integration**: Direct embedding of Garlic's C source code
2. **Identical Build System**: Uses the same CMake configuration as Garlic CLI
3. **Full Format Support**: APK, DEX, JAR, and CLASS files with native detection
4. **Optimized Performance**: Same compiler flags and optimizations as Garlic
5. **Cross-Platform**: Windows and Linux support with proper threading

## Future Scope

- [ ] Build Garlic entirely into a C++ Project instead of C. <br>
- [ ] Add Class/Jar support.

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
- **JADX**: Inspiration for the GUI design
- **Qt6**: Cross-platform GUI framework

## Authors

- Kritik Agarwal
- AbhiTheModder
