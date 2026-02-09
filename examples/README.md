# Examples Curriculum

These examples are organized as a teaching path from first compile to advanced language features.

`examples/hello_world.t81` is intentionally preserved as a legacy/aspirational sample from the earlier foundation flow. `examples/00_hello_world.t81` is the traditional "Hello World" teaching sample, and it is compile-verified in the examples lane.

## Prerequisite

```bash
make cli
```

## Learning Path

1. `examples/00_hello_world.t81`
- Learn: traditional "Hello World" shape.
- Command:
```bash
./build/bin/t81-lang build examples/00_hello_world.t81 -o build/examples/00_hello_world.tisc.json
```

2. `examples/01_variables_and_arithmetic.t81`
- Learn: variables, inference-style bindings, arithmetic.
- Command:
```bash
./build/bin/t81-lang build examples/01_variables_and_arithmetic.t81 -o build/examples/01_variables_and_arithmetic.tisc.json
```

3. `examples/02_conditionals_and_loops.t81`
- Learn: `while`, `if`, assignment, comparison.
- Command:
```bash
./build/bin/t81-lang build examples/02_conditionals_and_loops.t81 -o build/examples/02_conditionals_and_loops.tisc.json
```

4. `examples/03_functions_and_calls.t81`
- Learn: boolean relations and logical short-circuit conditions.
- Command:
```bash
./build/bin/t81-lang build examples/03_functions_and_calls.t81 -o build/examples/03_functions_and_calls.tisc.json
```

5. `examples/04_logical_short_circuit.t81`
- Learn: `&&`/`||` short-circuit boolean control flow.
- Command:
```bash
./build/bin/t81-lang build examples/04_logical_short_circuit.t81 -o build/examples/04_logical_short_circuit.tisc.json
```

6. `examples/05_modules_imports/main.t81`
- Learn: module declarations, imports, entrypoint checking across a small module graph.
- Commands:
```bash
./build/bin/t81-lang check examples/05_modules_imports/main.t81
./build/bin/t81-lang build examples/05_modules_imports/main.t81 -o build/examples/05_modules_imports_main.tisc.json
```

7. `examples/06_effect_and_tier.t81`
- Learn: `@effect` and `@tier(n)` annotations.
- Command:
```bash
./build/bin/t81-lang build examples/06_effect_and_tier.t81 -o build/examples/06_effect_and_tier.tisc.json
```

8. `examples/07_records_enums.t81`
- Learn: `record` declarations, `enum` declarations, and record literals.
- Command:
```bash
./build/bin/t81-lang build examples/07_records_enums.t81 -o build/examples/07_records_enums.tisc.json
```

9. `examples/08_bigint_basics.t81`
- Learn: `T81BigInt` bindings and arithmetic.
- Command:
```bash
./build/bin/t81-lang build examples/08_bigint_basics.t81 -o build/examples/08_bigint_basics.tisc.json
```

10. `examples/09_float_basics.t81`
- Learn: `T81Float` bindings and arithmetic.
- Command:
```bash
./build/bin/t81-lang build examples/09_float_basics.t81 -o build/examples/09_float_basics.tisc.json
```

11. `examples/10_fraction_basics.t81`
- Learn: `T81Fraction` bindings and arithmetic.
- Command:
```bash
./build/bin/t81-lang build examples/10_fraction_basics.t81 -o build/examples/10_fraction_basics.tisc.json
```

12. `examples/11_vector_basics.t81`
- Learn: first-class `T81Vector[...]` type syntax and vector literals.
- Command:
```bash
./build/bin/t81-lang build examples/11_vector_basics.t81 -o build/examples/11_vector_basics.tisc.json
```

13. `examples/12_matrix_basics.t81`
- Learn: first-class `T81Matrix[...]` type syntax (MVP vector-literal coercion path).
- Command:
```bash
./build/bin/t81-lang build examples/12_matrix_basics.t81 -o build/examples/12_matrix_basics.tisc.json
```

14. `examples/13_tensor_basics.t81`
- Learn: first-class `T81Tensor[...]` type syntax (MVP vector-literal coercion path).
- Command:
```bash
./build/bin/t81-lang build examples/13_tensor_basics.t81 -o build/examples/13_tensor_basics.tisc.json
```

15. `examples/14_graph_basics.t81`
- Learn: first-class `T81Graph[...]` type syntax (MVP vector-literal coercion path).
- Command:
```bash
./build/bin/t81-lang build examples/14_graph_basics.t81 -o build/examples/14_graph_basics.tisc.json
```

16. `examples/15_all_core_datatypes_mvp.t81`
- Learn: declaration-level compile coverage for the full core datatype inventory.
- Command:
```bash
./build/bin/t81-lang build examples/15_all_core_datatypes_mvp.t81 -o build/examples/15_all_core_datatypes_mvp.tisc.json
```

17. `examples/16_trit_numeric_extensions.t81`
- Learn: flow-level behavior for `T81UInt`, `T81Fixed`, `T81Complex`, `T81Quaternion`, `T81Prob`, and `T81Polynomial`.
- Command:
```bash
./build/bin/t81-lang build examples/16_trit_numeric_extensions.t81 -o build/examples/16_trit_numeric_extensions.tisc.json
```

18. `examples/17_symbol_bytes_base81_flow.t81`
- Learn: flow-level behavior for `T81Symbol`, `T81Bytes`, and `Base81String` through typed function boundaries.
- Command:
```bash
./build/bin/t81-lang build examples/17_symbol_bytes_base81_flow.t81 -o build/examples/17_symbol_bytes_base81_flow.tisc.json
```

19. `examples/18_collections_flow_mvp.t81`
- Learn: flow-level behavior for `T81List`, `T81Map`, `T81Set`, `T81Tree`, `T81Stream`, `T81Vector`, `T81Matrix`, `T81Tensor`, and `T81Graph`.
- Command:
```bash
./build/bin/t81-lang build examples/18_collections_flow_mvp.t81 -o build/examples/18_collections_flow_mvp.tisc.json
```

20. `examples/19_maybe_result_promise_flow.t81`
- Learn: flow-level behavior for `T81Maybe`, `T81Result`, and `T81Promise`.
- Command:
```bash
./build/bin/t81-lang build examples/19_maybe_result_promise_flow.t81 -o build/examples/19_maybe_result_promise_flow.tisc.json
```

21. `examples/20_runtime_handles_flow_mvp.t81`
- Learn: flow-level behavior for runtime handle types (`T81Agent`, `T81Entropy`, `T81Time`, `T81IOStream`, `T81Thread`, `T81Network`, `T81Discovery`, `T81Qutrit`, `T81Category`, `T81Proof`, `T81Reflection`, `Cell`, `CanonicalId`).
- Command:
```bash
./build/bin/t81-lang build examples/20_runtime_handles_flow_mvp.t81 -o build/examples/20_runtime_handles_flow_mvp.tisc.json
```

22. `examples/string_demo.t81`
- Learn: `T81String` values and string expressions.
- Command:
```bash
./build/bin/t81-lang build examples/string_demo.t81 -o build/examples/string_demo.tisc.json
```

23. `examples/option_result_match.t81`
- Learn: `Option`/`Result` constructors and nested `match`.
- Command:
```bash
./build/bin/t81-lang build examples/option_result_match.t81 -o build/examples/option_result_match.tisc.json
```

## Capstones

After the teaching track, explore the existing capstone demos:

- `examples/hello_world.t81` (legacy aspirational style)
- `examples/tensor_demo.t81`
- `examples/matrix_demo.t81`
- `examples/quaternion_demo.t81`
- `examples/high_rank_tensor_demo.t81`
- `examples/advanced_datatypes_showcase.t81`
- `examples/weights_load_demo.t81`
