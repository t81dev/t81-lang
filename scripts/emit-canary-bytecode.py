#!/usr/bin/env python3
"""Emit a deterministic VM canary artifact (TISC JSON V1)."""

from __future__ import annotations

import json
import sys
from pathlib import Path


PROGRAM = {
    "spec_version": "tisc-json-v1",
    "axion_policy_text": "(policy (tier 2))",
    "insns": [
        {"opcode": "LoadImm", "a": 0, "b": 7, "c": 0},
        {"opcode": "LoadImm", "a": 1, "b": 3, "c": 0},
        {"opcode": "Add", "a": 2, "b": 0, "c": 1},
        {"opcode": "Store", "a": 10, "b": 2, "c": 0},
        {"opcode": "Load", "a": 3, "b": 10, "c": 0},
        {"opcode": "Halt", "a": 0, "b": 0, "c": 0},
    ],
}


def main() -> None:
    if len(sys.argv) != 2:
        raise SystemExit("usage: emit-canary-bytecode.py <out-file>")

    out = Path(sys.argv[1])
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(PROGRAM, indent=2, sort_keys=False) + "\n", encoding="utf-8")
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
