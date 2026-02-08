# Ecosystem Alignment

Snapshot date: 2026-02-08
Source: public repositories at `https://github.com/t81dev`

## Direct Dependencies For t81-lang

- `t81-foundation`: TISC IR, HanoiVM contract, Axion policy semantics.
- `duotronic-whitepaper`: upstream semantic boundaries for ternary coprocessor model.
- `t81-constraints`: epistemic constraints and falsifiability boundaries.

## First-Wave Integrations

- `t81-docs`: canonical language docs and onboarding.
- `t81-examples`: runnable language examples and tutorials.
- `t81-python`: bindings and scripting workflows.
- `t81-benchmarks`: compiler and runtime benchmark harness.

## Ecosystem Mapping (all visible repos)

- `ANGELA`: governance/constitutional framing; not a direct compiler dependency.
- `duotronic-computing`: interpretive context; optional explanatory reference.
- `duotronic-thesis`: research framing and negative-knowledge archive.
- `duotronic-whitepaper`: normative upstream semantics reference.
- `llama.cpp`: external upstream fork for inference runtime context.
- `t81-benchmarks`: reproducible performance/correctness baselines.
- `t81-constraints`: constraints and failure-boundary governance.
- `t81-docs`: central documentation hub.
- `t81-examples`: demo and educational programs.
- `t81-foundation`: core runtime/numerics/VM base.
- `t81-hardware`: hardware simulation and verification path.
- `t81-lang`: this repository.
- `t81-python`: high-level integration package.
- `t81-roadmap`: cross-repo strategy and milestone alignment.
- `t81lib`: ternary numerics/quantization primitives.
- `ternary`: balanced ternary LLM quantization implementation.
- `ternary-delta`: adoption delta and transition framing.
- `ternary-fabric`: ternary memory/interconnect coprocessor research.
- `ternary-memory-research`: memory viability experiments.
- `ternary-pager`: semantic compression falsification tooling.
- `ternary-tools`: GGUF inspector/debugger.
- `ternary_gcc_plugin`: C/C++ lowering into ternary helper ABI.
- `trinity`: ternary cryptography suite.
- `trinity-decrypt`: decryption tooling for trinary artifacts.
- `trinity-pow`: ternary proof-of-work experiments.

## Proposed Ownership Split

- Language semantics and compiler implementation: `t81-lang`.
- Runtime behavior and VM execution semantics: `t81-foundation`.
- User-facing tutorials and broad docs: `t81-docs`, `t81-examples`.
- High-level SDK and notebook workflows: `t81-python`.
- Reproducible evidence and claims: `t81-benchmarks`.

## Split Principle

Language evolution should not be blocked by runtime release cadence, and runtime hardening should not require language contributors to navigate unrelated subsystems. The repository boundary is intentionally optimized for independent iteration with explicit contracts.

## Migration Execution Note

An initial migration pull from local `t81-foundation` has been completed for frontend code, language tests, examples, and core language docs/specs. File-level traceability is recorded in `docs/migration/migrated-from-t81-foundation.tsv`, and repeatable sync is provided by `scripts/migrate-from-foundation.sh`.
