# Spec Coverage Matrix

Snapshot date: 2026-02-09

| Area | Status | Notes | Primary checks |
| --- | --- | --- | --- |
| Generic syntax `[...]` and legacy `<...>` rejection | Implemented | Angle-bracket generic syntax is parser-rejected. | `tests/syntax/frontend_parser_generics_test.cpp`, `tests/syntax/frontend_parser_legacy_rejection_test.cpp` |
| Logical precedence `&&` / `||` | Implemented | Parser precedence and IR short-circuit lowering are both wired. | `tests/syntax/frontend_parser_module_import_effect_test.cpp`, `tests/roundtrip/frontend_ir_generator_logical_short_circuit_test.cpp` |
| Module/import declarations | Implemented (MVP) | Parsed and semantically validated within file scope. | `tests/syntax/frontend_parser_module_import_effect_test.cpp`, `tests/semantics/semantic_analyzer_module_import_effect_test.cpp` |
| Module graph loading + missing/cycle checks | Implemented (CLI MVP) | `t81-lang check` recursively resolves imports, reports missing modules and cycles. | `scripts/check-module-graph.sh` |
| CLI compile/emit surface (`emit-ir`, `emit-bytecode`, `build`) | Implemented (MVP) | Emits deterministic IR text and `tisc-json-v1` artifacts from `.t81` sources. | `scripts/check-cli-compile.sh` |
| Teaching examples as compile-verified curriculum | Implemented (MVP) | Numbered lessons in `examples/` are build-checked in CI lanes. | `scripts/check-examples-build.sh`, `examples/README.md` |
| Structural annotations `@schema` / `@module` | Implemented | Applied to `record`/`enum` and emitted into type-alias metadata. | `tests/roundtrip/cli_structural_types_test.cpp` |
| Function annotations `@effect` / `@tier(n)` parse + semantic validation | Implemented | Parsed on functions; tier positivity validated. | `tests/syntax/frontend_parser_module_import_effect_test.cpp`, `tests/semantics/semantic_analyzer_module_import_effect_test.cpp` |
| Pure/effect call boundary | Implemented | Pure functions cannot call effectful functions. | `tests/semantics/semantic_analyzer_module_import_effect_test.cpp` |
| Effect/tier metadata emission | Implemented | Function metadata carried in IR (`FunctionMetadata`). | `tests/roundtrip/frontend_ir_generator_logical_short_circuit_test.cpp` |
| Option/Result constructors + exhaustive match checks | Implemented | Constructor context checks and exhaustive `match` arm validation. | `tests/semantics/semantic_analyzer_option_result_test.cpp`, `tests/semantics/semantic_analyzer_match_test.cpp` |
| Deterministic AST snapshots | Implemented | Canonical AST output locked with goldens. | `scripts/check-parser-determinism.sh` |
| Cross-module symbol resolution through imports | Planned | Current module graph checks resolve files/cycles, but symbol linking across modules is not yet implemented. | N/A |
