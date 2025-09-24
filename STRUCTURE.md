<div align="center">

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

# Garlic Decompiler (GUI)

*A modern C++/Qt6 GUI front-end for the Garlic Decompiler — bringing its blazing-fast APK/DEX/JAR/CLASS decompilation power to your desktop.*

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-17%2B-blue)](#requirements)
[![Qt6](https://img.shields.io/badge/Qt-6-green)](https://www.qt.io/)

</div>

## Complete Project Directory Structure:
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
        ├── garlic.c                 # From Garlic Decompiler Source (neocanable/garlic)
        ├── apk/                     # From Garlic Decompiler Source (neocanable/garlic)
        │   └── apk.c/h
        ├── common/                  # From Garlic Decompiler Source (neocanable/garlic)
        │   ├── debug.h
        │   ├── endian_x.h
        │   ├── file_tools.h
        │   ├── output_tools.h
        │   ├── str_tools.h
        │   └── types.h
        ├── dalvik/                  # From Garlic Decompiler Source (neocanable/garlic)
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
        ├── decompiler/               # From Garlic Decompiler Source (neocanable/garlic)
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

## License

This project is licensed under the Apache License 2.0 — see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **Garlic Decompiler**: <https://github.com/neocanable/garlic>
- **JADX**: Inspiration for the GUI Design
- **Qt6**: Cross-platform GUI Framework

## Authors

- [AbhiTheModder](https://github.com/AbhiTheModder)
- [Kritik Agarwal](https://github.com/AgarwalKritik)