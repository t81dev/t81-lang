# Core Datatype Support Matrix

Snapshot date: 2026-02-09

Source inventory: `t81-foundation/include/t81/core/README.md`

This matrix maps the canonical `t81-foundation` core datatype list to current `t81-lang` compiler/teaching status.

## Status Keys

- `Compile-verified`: included in the current `t81-lang build` teaching lane (`scripts/check-examples-build.sh`).
- `Compile-verified (flow MVP)`: first-class type-name acceptance with compile-verified typed flow (declarations, parameter/return passing, and assignment patterns); richer constructors/operators/runtime semantics may still be expanding.
- `Frontend-tested`: covered by parser/semantic/IR tests, but not promoted to compile-verified tutorial examples.
- `Runtime/core only`: defined in foundation/runtime C++ core surface; not currently exposed as a compile-verified language feature in this repo.

## Matrix

| Data Type | Status | Notes / Evidence |
| --- | --- | --- |
| `T81Int<N>` | Compile-verified | Surface integer forms (`i2/i8/i16/i32`) are compile-verified across examples/tests; core-name coverage is in `examples/15_all_core_datatypes_mvp.t81`. |
| `T81UInt<N>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/16_trit_numeric_extensions.t81`. |
| `T81BigInt` | Compile-verified | Arithmetic compile coverage in `examples/08_bigint_basics.t81`. |
| `T81Float<M,E>` | Compile-verified | Arithmetic compile coverage in `examples/09_float_basics.t81`. |
| `T81Fixed<I,F>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/16_trit_numeric_extensions.t81`. |
| `T81Fraction<N>` | Compile-verified | Arithmetic compile coverage in `examples/10_fraction_basics.t81`. |
| `T81Complex<M>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/16_trit_numeric_extensions.t81`. |
| `T81Quaternion` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/16_trit_numeric_extensions.t81`; capstone expression demo remains in `examples/quaternion_demo.t81`. |
| `T81Prob` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/16_trit_numeric_extensions.t81`. |
| `T81Polynomial` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/16_trit_numeric_extensions.t81`. |
| `T81Symbol` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/17_symbol_bytes_base81_flow.t81`. |
| `T81String` | Compile-verified | Compile-verified via `examples/00_hello_world.t81` and `examples/string_demo.t81` in `scripts/check-examples-build.sh`. |
| `T81Bytes` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/17_symbol_bytes_base81_flow.t81`. |
| `Base81String` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/17_symbol_bytes_base81_flow.t81`. |
| `T81List<E>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/18_collections_flow_mvp.t81`. |
| `T81Map<K,V>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/18_collections_flow_mvp.t81`. |
| `T81Set<T>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/18_collections_flow_mvp.t81`. |
| `T81Tree<T>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/18_collections_flow_mvp.t81`. |
| `T81Stream<T>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/18_collections_flow_mvp.t81`. |
| `T81Graph` | Compile-verified (MVP) | First-class `T81Graph[...]` syntax compile-verified via `examples/14_graph_basics.t81` (MVP vector-literal coercion path). |
| `T81Vector<N,S>` | Compile-verified (MVP) | First-class `T81Vector[...]` syntax compile-verified via `examples/11_vector_basics.t81`; higher-rank/vector-shape semantics still expanding. |
| `T81Matrix<S,R,C>` | Compile-verified (MVP) | First-class `T81Matrix[...]` syntax compile-verified via `examples/12_matrix_basics.t81` (MVP vector-literal coercion path). |
| `T81Tensor<E,R,Dims...>` | Compile-verified (MVP) | First-class `T81Tensor[...]` syntax compile-verified via `examples/13_tensor_basics.t81` (MVP vector-literal coercion path). |
| `Option<T>` | Compile-verified | Compile-verified via `examples/option_result_match.t81` in `scripts/check-examples-build.sh`; parser/semantic/IR tests already present. |
| `Result<T,E>` | Compile-verified | Compile-verified via `examples/option_result_match.t81` in `scripts/check-examples-build.sh`; parser/semantic/IR tests already present. |
| `T81Maybe<T>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/19_maybe_result_promise_flow.t81`; language-facing teaching alias remains `Option<T>`. |
| `T81Result<T,E>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/19_maybe_result_promise_flow.t81`; language-facing teaching alias remains `Result<T,E>`. |
| `T81Promise<T>` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/19_maybe_result_promise_flow.t81`. |
| `T81Agent` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Entropy` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Time` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81IOStream` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`; legacy I/O syntax in `examples/hello_world.t81` remains aspirational. |
| `T81Thread` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Network` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Discovery` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Qutrit` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Category` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Proof` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `T81Reflection` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `Cell` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `CanonicalId` | Compile-verified (flow MVP) | Typed flow compile coverage in `examples/20_runtime_handles_flow_mvp.t81`. |
| `all.hpp` | Runtime/core only | C++ include convenience; not a language-level datatype. |

## Promotion Plan

1. Keep full core inventory coverage stable via `examples/15_all_core_datatypes_mvp.t81` and the flow lessons (`examples/16_*` through `examples/20_*`).
2. Promote flow-MVP datatypes to constructor/operator semantics where missing (`T81UInt`, `T81Fixed`, `T81Complex`, `T81Quaternion`, `T81Prob`, `T81Polynomial`).
3. Strengthen `T81Vector`/`T81Matrix`/`T81Tensor`/`T81Graph` from MVP compile-verified support to richer shape/layout semantics.
4. Add semantic and IR regression tests as each datatype graduates from flow-MVP to behavior-complete support.
