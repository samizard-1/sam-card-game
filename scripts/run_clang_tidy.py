#!/usr/bin/env python3
"""Run clang-tidy over the project's own source files.

This is the single tidy runner shared by the VSCode "Tidy" task, the pre-commit
hook, and CI, so the linter behaves identically everywhere. It always refreshes
the CMake build (to keep compile_commands.json in sync after files are added),
then runs clang-tidy with warnings treated as errors.

Usage:
  python3 scripts/run_clang_tidy.py            # check every src/**/*.cpp
  python3 scripts/run_clang_tidy.py a.cpp b.hpp # check only the .cpp given
"""
import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"
SRC = ROOT / "src"
CPP_SUFFIXES = (".cpp", ".cc", ".cxx")

# clang-tidy is keg-only on macOS (brew install llvm); add its bin dir to PATH
# so the tool is found both in interactive shells and in pre-commit / CI.
# Check Apple Silicon (/opt/homebrew) first, then Intel Mac (/usr/local).
for _BREW_LLVM_BIN in (Path("/opt/homebrew/opt/llvm/bin"), Path("/usr/local/opt/llvm/bin")):
    if _BREW_LLVM_BIN.is_dir() and not shutil.which("clang-tidy"):
        os.environ["PATH"] = str(_BREW_LLVM_BIN) + os.pathsep + os.environ.get("PATH", "")
        break


def configure() -> None:
    """(Re)generate build/compile_commands.json. Cheap if already configured."""
    subprocess.run(
        ["cmake", "-B", str(BUILD), "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
        cwd=ROOT,
        check=True,
    )


def _macos_sysroot_args() -> list[str]:
    """Return --extra-arg=-isysroot<path> when running on macOS with a brew
    LLVM clang-tidy.  Brew LLVM doesn't know the Xcode/CLT SDK path, so
    standard C++ headers (e.g. <string>) are invisible to it unless we pass
    -isysroot explicitly.  The xcrun call is fast (~10 ms) and is a no-op on
    Linux / CI where xcrun doesn't exist."""
    if not shutil.which("xcrun"):
        return []
    result = subprocess.run(
        ["xcrun", "--show-sdk-path"],
        capture_output=True,
        text=True,
    )
    sdk = result.stdout.strip()
    if sdk:
        return [f"--extra-arg=-isysroot{sdk}"]
    return []


def main(argv: list[str]) -> int:
    if argv:
        # Called with explicit files (e.g. by pre-commit): only check the .cpp.
        files = [Path(a) for a in argv if a.endswith(CPP_SUFFIXES)]
        if not files:
            return 0  # nothing of ours to check
    else:
        files = sorted(SRC.rglob("*.cpp"))
        if not files:
            print("No source files to check yet.")
            return 0

    configure()
    cmd = ["clang-tidy", "-p", str(BUILD), "--warnings-as-errors=*"]
    cmd += _macos_sysroot_args()
    cmd += [str(f) for f in files]
    print("Running:", " ".join(cmd))
    return subprocess.run(cmd, cwd=ROOT).returncode


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
