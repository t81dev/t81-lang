#!/usr/bin/env python3
"""Cross-repo compatibility check against t81-vm contract."""

from __future__ import annotations

import json
import os
import subprocess
import sys
from pathlib import Path


REQUIRED_OPCODES = {
    "Nop",
    "Halt",
    "LoadImm",
    "Load",
    "Store",
    "Add",
    "Sub",
    "Mul",
    "Div",
    "Mod",
    "Jump",
    "JumpIfZero",
    "JumpIfNotZero",
    "Cmp",
    "Trap",
}


def run(cmd: list[str], cwd: Path) -> None:
    result = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if result.returncode != 0:
        print(result.stdout)
        raise SystemExit(f"Command failed ({result.returncode}): {' '.join(cmd)}")


def check_contract(vm_dir: Path) -> None:
    local_contract_path = Path(__file__).resolve().parent.parent / "contracts/runtime-contract.json"
    if not local_contract_path.exists():
        raise SystemExit(f"Missing local runtime contract marker: {local_contract_path}")
    local_contract = json.loads(local_contract_path.read_text(encoding="utf-8"))

    contract_path = vm_dir / "docs/contracts/vm-compatibility.json"
    if not contract_path.exists():
        raise SystemExit(f"Missing VM contract file: {contract_path}")

    contract = json.loads(contract_path.read_text(encoding="utf-8"))
    vm_contract_version = str(contract.get("contract_version", "")).strip()
    expected_contract_version = str(local_contract.get("contract_version", "")).strip()
    if vm_contract_version != expected_contract_version:
        raise SystemExit(
            "VM contract version mismatch: "
            f"vm={vm_contract_version!r} local={expected_contract_version!r}"
        )

    lane = os.environ.get("VM_COMPAT_LANE", "").strip().lower()
    if lane == "pinned":
        expected_pin = str(local_contract.get("vm_main_pin", "")).strip()
        if not expected_pin:
            raise SystemExit("Pinned lane requires vm_main_pin in contracts/runtime-contract.json")
        vm_head = (
            subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=vm_dir, text=True).strip()
        )
        if vm_head != expected_pin:
            raise SystemExit(
                "Pinned lane VM commit mismatch: "
                f"vm={vm_head} expected={expected_pin}"
            )

    opcodes = set(contract.get("supported_opcodes", []))
    missing = sorted(REQUIRED_OPCODES - opcodes)
    if missing:
        raise SystemExit(f"VM contract missing required opcodes: {', '.join(missing)}")

    formats = {item.get("name") for item in contract.get("accepted_program_formats", [])}
    for required_format in ("TextV1", "TiscJsonV1"):
        if required_format not in formats:
            raise SystemExit(f"VM contract missing accepted format: {required_format}")


def smoke_run(vm_dir: Path) -> None:
    run(["make", "build-check"], cwd=vm_dir)

    vm_bin = vm_dir / "build/t81vm"
    if not vm_bin.exists():
        raise SystemExit(f"Expected VM binary not found: {vm_bin}")

    ok_program = vm_dir / "tests/harness/test_vectors/arithmetic.t81"
    fault_program = vm_dir / "tests/harness/test_vectors/faults.t81"

    ok = subprocess.run(
        [str(vm_bin), "--trace", "--snapshot", str(ok_program)],
        cwd=vm_dir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    if ok.returncode != 0:
        raise SystemExit(f"Expected success running {ok_program.name}: {ok.stderr}")
    if "STATE_HASH " not in ok.stdout:
        raise SystemExit("Expected STATE_HASH in VM snapshot output")

    bad = subprocess.run(
        [str(vm_bin), "--trace", "--snapshot", str(fault_program)],
        cwd=vm_dir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    if bad.returncode == 0:
        raise SystemExit(f"Expected fault running {fault_program.name}")
    if "FAULT" not in bad.stderr:
        raise SystemExit("Expected FAULT marker in stderr for fault vector")


def main() -> None:
    root = Path(__file__).resolve().parent.parent
    vm_dir = Path(os.environ.get("T81_VM_DIR", str((root / "../t81-vm").resolve())))
    if not vm_dir.exists():
        raise SystemExit(f"t81-vm path not found: {vm_dir}")

    check_contract(vm_dir)
    smoke_run(vm_dir)
    print("t81-lang vm compatibility check: ok")


if __name__ == "__main__":
    main()
