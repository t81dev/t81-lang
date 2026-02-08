# STATUS

Snapshot date: 2026-02-08

## Current Phase

Repository foundation complete; migration execution in progress.

## Completed

- Baseline project documentation scaffold.
- Cross-repo alignment draft covering all public `t81dev` repositories.
- Initial compiler architecture and language scope draft.
- GitHub API sync script for ecosystem metadata.
- Initial split rationale captured: language ownership separated from runtime ownership.
- Initial migration import completed from local `t81-foundation`:
  - frontend headers/sources
  - language `.t81` examples
  - language docs/spec/RFCs
  - language-focused tests and vectors
- Migration automation script added: `scripts/migrate-from-foundation.sh`.
- Migration traceability manifest added: `docs/migration/migrated-from-t81-foundation.tsv`.

## In Progress

- Dependency isolation plan so migrated language tests can run without bundling full runtime internals.
- Runtime-coupled test surface reduction (`tests/roundtrip/runtime-coupled-tests.txt`).
- Compatibility matrix enforcement between emitted artifacts and `t81-vm` runtime contract expectations.
- IR ownership decision for `t81/tisc/ir.hpp` and related interfaces (tracked in `t81-lang#3`).

## Blockers

- No infra blockers; CI baseline is green.
- VM parity P0 work in `t81-vm` is required before deeper integration lanes can expand (`t81-vm#2`, `t81-vm#3`).
- Some migrated roundtrip tests still depend on runtime/CLI headers; decoupling remains pending (`t81-lang#3`).

## Exit Criteria For Foundation Phase

- Milestone M0 accepted.
- CI skeleton added for lint/test/docs validation.
- First runnable `t81-lang parse` command merged.
- Repository split checklist accepted and migration backlog prioritized.
- Compatibility matrix adopted and referenced by both repos.
