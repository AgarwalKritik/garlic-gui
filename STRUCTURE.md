<div align="center">

# Garlic Decompiler (GUI)

<img src="icon/garlic.png" alt="GarlicGUI Icon" width="128" />

**A modern C++/Qt6 GUI front-end for the Garlic Decompiler — bringing blazing-fast APK/DEX/JAR/CLASS decompilation power to your desktop.**

[![License](icon/license.svg)](LICENSE)
[![C++](icon/cpp.svg)](#built-with)
[![Qt6](icon/qt.svg)](https://www.qt.io/)

</div>

## Complete Project Directory Structure

```
garlic-gui/
└── src/
    ├── main.cpp                                # Entry point
    ├── gui/                                    # Qt6 GUI components
    │   ├── MainWindow.cpp/h                    # Main window
    │   ├── FileTreeWidget.cpp/h                # File navigation tree
    │   ├── CodeEditorWidget.cpp/h              # Tabbed code editor
    │   ├── DecompilerInterface.cpp/h           # C++ wrapper for Garlic
    │   ├── ProjectManager.cpp/h                # Project management
    │   └── DecompilerProgressDialog.cpp/h      # Progress dialog
    └── garlic/                                 # Garlic integration
        ├── garlic_wrapper.c/h                  # C interface wrapper
        ├── garlic.c                            # Original Garlic Decompiler Engine (C source)
        ├── apk/                                # Original Garlic Decompiler Engine (C source)
        │   └── apk.c/h                         # Original Garlic Decompiler Engine (C source)
        ├── common/                             # Original Garlic Decompiler Engine (C source)
        │   ├── debug.h                         # Original Garlic Decompiler Engine (C source)
        │   ├── endian_x.h                      # Original Garlic Decompiler Engine (C source)
        │   ├── file_tools.h                    # Original Garlic Decompiler Engine (C source)
        │   ├── output_tools.h                  # Original Garlic Decompiler Engine (C source)
        │   ├── str_tools.h                     # Original Garlic Decompiler Engine (C source)
        │   └── types.h                         # Original Garlic Decompiler Engine (C source)
        ├── dalvik/                             # Original Garlic Decompiler Engine (C source)
        │   ├── dex_annotation.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── dex_class.c/h                   # Original Garlic Decompiler Engine (C source)
        │   ├── dex_decompile.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── dex_descriptor.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── dex_dump.c/h                    # Original Garlic Decompiler Engine (C source)
        │   ├── dex_exception.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── dex_expression_builder.c/h      # Original Garlic Decompiler Engine (C source)
        │   ├── dex_ins.c/h                     # Original Garlic Decompiler Engine (C source)
        │   ├── dex_ins_action.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── dex_ins_helper.h                # Original Garlic Decompiler Engine (C source)
        │   ├── dex_lambda.c/h                  # Original Garlic Decompiler Engine (C source)
        │   ├── dex_meta_helper.c/h             # Original Garlic Decompiler Engine (C source)
        │   ├── dex_method.c/h                  # Original Garlic Decompiler Engine (C source)
        │   ├── dex_optimizer.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── dex_pre_optimizer.c/h           # Original Garlic Decompiler Engine (C source)
        │   ├── dex_simulator.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── dex_smali.c/h                   # Original Garlic Decompiler Engine (C source)
        │   ├── dex_structure.h                 # Original Garlic Decompiler Engine (C source)
        │   ├── dex_type_analyse.c/h            # Original Garlic Decompiler Engine (C source)
        │   └── dex_writter.c                   # Original Garlic Decompiler Engine (C source)
        ├── decompiler/                         # Original Garlic Decompiler Engine (C source)
        │   ├── control_flow.c/h                # Original Garlic Decompiler Engine (C source)
        │   ├── debug_pointer.h                 # Original Garlic Decompiler Engine (C source)
        │   ├── descriptor.c/h                  # Original Garlic Decompiler Engine (C source)
        │   ├── dominator_tree.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── exception.c/h                   # Original Garlic Decompiler Engine (C source)
        │   ├── expression.c/h                  # Original Garlic Decompiler Engine (C source)
        │   ├── expression_analyse.c/h          # Original Garlic Decompiler Engine (C source)
        │   ├── expression_array.c/h            # Original Garlic Decompiler Engine (C source)
        │   ├── expression_assert.c/h           # Original Garlic Decompiler Engine (C source)
        │   ├── expression_assign.c/h           # Original Garlic Decompiler Engine (C source)
        │   ├── expression_branches.c/h         # Original Garlic Decompiler Engine (C source)
        │   ├── expression_chain.c/h            # Original Garlic Decompiler Engine (C source)
        │   ├── expression_copy_propgation.c/h  # Original Garlic Decompiler Engine (C source)
        │   ├── expression_enum.c/h             # Original Garlic Decompiler Engine (C source)
        │   ├── expression_exception.c/h        # Original Garlic Decompiler Engine (C source)
        │   ├── expression_goto.c/h             # Original Garlic Decompiler Engine (C source)
        │   ├── expression_helper.h             # Original Garlic Decompiler Engine (C source)
        │   ├── expression_if.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── expression_inline.c/h           # Original Garlic Decompiler Engine (C source)
        │   ├── expression_inner_class.c/h      # Original Garlic Decompiler Engine (C source)
        │   ├── expression_local_variable.c/h   # Original Garlic Decompiler Engine (C source)
        │   ├── expression_logical.c/h          # Original Garlic Decompiler Engine (C source)
        │   ├── expression_loop.c/h             # Original Garlic Decompiler Engine (C source)
        │   ├── expression_loop_type.c/h        # Original Garlic Decompiler Engine (C source)
        │   ├── expression_new.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── expression_node.c/h             # Original Garlic Decompiler Engine (C source)
        │   ├── expression_node_helper.h        # Original Garlic Decompiler Engine (C source)
        │   ├── expression_node_param.c/h       # Original Garlic Decompiler Engine (C source)
        │   ├── expression_remove_useless.c/h   # Original Garlic Decompiler Engine (C source)
        │   ├── expression_return.c/h           # Original Garlic Decompiler Engine (C source)
        │   ├── expression_synchronized.c/h     # Original Garlic Decompiler Engine (C source)
        │   ├── expression_ternary.c/h          # Original Garlic Decompiler Engine (C source)
        │   ├── expression_visitor.c/h          # Original Garlic Decompiler Engine (C source)
        │   ├── expression_writter.c/h          # Original Garlic Decompiler Engine (C source)
        │   ├── field.c/h                       # Original Garlic Decompiler Engine (C source)
        │   ├── instruction.c/h                 # Original Garlic Decompiler Engine (C source)
        │   ├── klass.c/h                       # Original Garlic Decompiler Engine (C source)
        │   ├── method.c/h                      # Original Garlic Decompiler Engine (C source)
        │   ├── scc.c/h                         # Original Garlic Decompiler Engine (C source)
        │   ├── signature.c/h                   # Original Garlic Decompiler Engine (C source)
        │   ├── ssa.c/h                         # Original Garlic Decompiler Engine (C source)
        │   ├── stack.c/h                       # Original Garlic Decompiler Engine (C source)
        │   ├── structure.h                     # Original Garlic Decompiler Engine (C source)
        │   └── transformer/                    # Original Garlic Decompiler Engine (C source)
        │       ├── anonymous.c                 # Original Garlic Decompiler Engine (C source)
        │       ├── array_load.c                # Original Garlic Decompiler Engine (C source)
        │       ├── array_store.c               # Original Garlic Decompiler Engine (C source)
        │       ├── arraylength.c               # Original Garlic Decompiler Engine (C source)
        │       ├── assert.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── assignment.c                # Original Garlic Decompiler Engine (C source)
        │       ├── assignment_chain.c          # Original Garlic Decompiler Engine (C source)
        │       ├── athrow.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── cast.c                      # Original Garlic Decompiler Engine (C source)
        │       ├── const.c                     # Original Garlic Decompiler Engine (C source)
        │       ├── declaration.c               # Original Garlic Decompiler Engine (C source)
        │       ├── define_stack_var.c          # Original Garlic Decompiler Engine (C source)
        │       ├── enum.c                      # Original Garlic Decompiler Engine (C source)
        │       ├── get_field.c                 # Original Garlic Decompiler Engine (C source)
        │       ├── get_static.c                # Original Garlic Decompiler Engine (C source)
        │       ├── goto.c                      # Original Garlic Decompiler Engine (C source)
        │       ├── if.c                        # Original Garlic Decompiler Engine (C source)
        │       ├── iinc.c                      # Original Garlic Decompiler Engine (C source)
        │       ├── initialize.c                # Original Garlic Decompiler Engine (C source)
        │       ├── instanceof.c                # Original Garlic Decompiler Engine (C source)
        │       ├── invoke.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── invokedynamic.c             # Original Garlic Decompiler Engine (C source)
        │       ├── invokeinterface.c           # Original Garlic Decompiler Engine (C source)
        │       ├── invokespecial.c             # Original Garlic Decompiler Engine (C source)
        │       ├── invokestatic.c              # Original Garlic Decompiler Engine (C source)
        │       ├── invokevirtual.c             # Original Garlic Decompiler Engine (C source)
        │       ├── lambda.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── local_variable.c            # Original Garlic Decompiler Engine (C source)
        │       ├── logic_not.c                 # Original Garlic Decompiler Engine (C source)
        │       ├── loop.c                      # Original Garlic Decompiler Engine (C source)
        │       ├── lvalue.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── monitorenter.c              # Original Garlic Decompiler Engine (C source)
        │       ├── monitorexit.c               # Original Garlic Decompiler Engine (C source)
        │       ├── new_array.c                 # Original Garlic Decompiler Engine (C source)
        │       ├── operator.c                  # Original Garlic Decompiler Engine (C source)
        │       ├── put_field.c                 # Original Garlic Decompiler Engine (C source)
        │       ├── put_static.c                # Original Garlic Decompiler Engine (C source)
        │       ├── return.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── single_list.c               # Original Garlic Decompiler Engine (C source)
        │       ├── single_operator.c           # Original Garlic Decompiler Engine (C source)
        │       ├── stack_value.c               # Original Garlic Decompiler Engine (C source)
        │       ├── stack_var.c                 # Original Garlic Decompiler Engine (C source)
        │       ├── store.c                     # Original Garlic Decompiler Engine (C source)
        │       ├── str_concat.c                # Original Garlic Decompiler Engine (C source)
        │       ├── switch.c                    # Original Garlic Decompiler Engine (C source)
        │       ├── ternary.c                   # Original Garlic Decompiler Engine (C source)
        │       ├── transformer.c/h             # Original Garlic Decompiler Engine (C source)
        │       └── uninitialize.c              # Original Garlic Decompiler Engine (C source)
        ├── jar/                                # Original Garlic Decompiler Engine (C source)
        │   └── jar.c/h                         # Original Garlic Decompiler Engine (C source)
        ├── jvm/                                # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_annotation.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_class.c/h                   # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_decompile.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_descriptor.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_exception.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_expression_builder.c/h      # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_ins.c/h                     # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_ins_action.c                # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_ins_helper.h                # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_lambda.c/h                  # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_method.c/h                  # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_optimizer.c/h               # Original Garlic Decompiler Engine (C source)
        │   ├── jvm_simulator.c/h               # Original Garlic Decompiler Engine (C source)
        │   └── jvm_type_analyse.c/h            # Original Garlic Decompiler Engine (C source)
        ├── libs/                               # Original Garlic Decompiler Engine (C source)
        │   ├── bitset/                         # Original Garlic Decompiler Engine (C source)
        │   │   ├── bitset.c/h                  # Original Garlic Decompiler Engine (C source)
        │   │   └── portability.h               # Original Garlic Decompiler Engine (C source)
        │   ├── hashmap/                        # Original Garlic Decompiler Engine (C source)
        │   │   ├── hashmap.c/h                 # Original Garlic Decompiler Engine (C source)
        │   │   └── hashmap_tools.c/h           # Original Garlic Decompiler Engine (C source)
        │   ├── list/                           # Original Garlic Decompiler Engine (C source)
        │   │   └── list.c/h                    # Original Garlic Decompiler Engine (C source)
        │   ├── memory/                         # Original Garlic Decompiler Engine (C source)
        │   │   ├── mem_common.h                # Original Garlic Decompiler Engine (C source)
        │   │   └── mem_pool.c/h                # Original Garlic Decompiler Engine (C source)
        │   ├── queue/                          # Original Garlic Decompiler Engine (C source)
        │   │   └── queue.c/h                   # Original Garlic Decompiler Engine (C source)
        │   ├── str/                            # Original Garlic Decompiler Engine (C source)
        │   │   └── str.c/h                     # Original Garlic Decompiler Engine (C source)
        │   ├── threadpool/                     # Original Garlic Decompiler Engine (C source)
        │   │   └── threadpool.c/h              # Original Garlic Decompiler Engine (C source)
        │   ├── trie/                           # Original Garlic Decompiler Engine (C source)
        │   │   └── trie_tree.c/h               # Original Garlic Decompiler Engine (C source)
        │   └── zip/                            # Original Garlic Decompiler Engine (C source)
        │       └── zip.h                       # Original Garlic Decompiler Engine (C source)
        └── parser/                             # Original Garlic Decompiler Engine (C source)
            ├── class/                          # Original Garlic Decompiler Engine (C source)
            │   ├── class_structure.h           # Original Garlic Decompiler Engine (C source)
            │   ├── class_tools.c/h             # Original Garlic Decompiler Engine (C source)
            │   ├── metadata.c/h                # Original Garlic Decompiler Engine (C source)
            │   └── opcode.c                    # Original Garlic Decompiler Engine (C source)
            ├── dex/                            # Original Garlic Decompiler Engine (C source)
            │   ├── dex.h                       # Original Garlic Decompiler Engine (C source)
            │   ├── dex_tools.h                 # Original Garlic Decompiler Engine (C source)
            │   ├── metadata.c/h                # Original Garlic Decompiler Engine (C source)
            │   └── opcode.c                    # Original Garlic Decompiler Engine (C source)
            ├── pe/                             # Original Garlic Decompiler Engine (C source)
            │   ├── metadata.h                  # Original Garlic Decompiler Engine (C source)
            │   ├── pe.h                        # Original Garlic Decompiler Engine (C source)
            │   ├── pe_const.h                  # Original Garlic Decompiler Engine (C source)
            │   └── pe_tools.h                  # Original Garlic Decompiler Engine (C source)
            └── reader/                         # Original Garlic Decompiler Engine (C source)
                └── reader.h                    # Original Garlic Decompiler Engine (C source)
```

## License

This project is distributed and licensed under the **Apache License 2.0** — see [LICENSE](LICENSE) for more information.

---

## Contact

If you have any questions or suggestions, feel free to reach out to us:

[![Raise an issue](icon/issue.svg)](https://github.com/AgarwalKritik/garlic-gui/issues/new)
[![AbhiTheModder](icon/abhithemodder.svg)](https://lin.ky/abhithemodder)

---

## Acknowledgments

[![Garlic Decompiler](icon/garlic.svg)](https://github.com/neocanable/garlic)
[![QT6](icon/qt6.svg)](https://www.qt.io/)

---

## Contributors

- [Kritik Agarwal](https://github.com/AgarwalKritik)
- [AbhiTheModder](https://github.com/AbhiTheModder)
