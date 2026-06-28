# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A raylib + C++20 **scaffold** for a card game with a custom (non-standard) deck, turn-based rules, and an NPC opponent. It is intentionally a skeleton: it builds and runs (a window with a placeholder card that flips on click), but the deck contents, rules, and AI are `// TODO(you):` stubs meant to be filled in. The audience is a developer with some C++ but little project experience, so clarity and signposting are first-class goals — preserve them when editing.

## Commands

```bash
# Build (first run downloads + compiles raylib via FetchContent — several minutes)
cmake -B build && cmake --build build

# Run the game
./build/sam_card_game

# Run all tests
ctest --test-dir build --output-on-failure

# Run a single doctest case (the test binary is build/unit_tests)
./build/unit_tests --test-case="drawing a card reduces the deck size by one"

# Lint (the one gate that matters — see Tooling below). Exits non-zero on any finding.
python3 scripts/run_clang_tidy.py            # all src/**/*.cpp
python3 scripts/run_clang_tidy.py src/foo.cpp # specific files

# Formatting check (what CI runs)
find src tests \( -name '*.cpp' -o -name '*.hpp' \) -print0 | xargs -0 clang-format --dry-run --Werror
```

In VSCode, **Ctrl/Cmd+Shift+B** runs the default "Build + Tidy" task; other tasks (Tidy, Test, Run) are under *Terminal → Run Task*.

## Architecture

Everything compiles into a `game_core` static library; both `sam_card_game` (just `src/main.cpp`) and `unit_tests` link against it. **When you add a `.cpp` under `src/`, add it to the `game_core` source list in `CMakeLists.txt`** — there is no globbing, and a comment in that list marks where.

Modules under `src/` are split by responsibility, one concept per folder:

- `app/Game` — owns the raylib window and the main loop (`run()` → `update(dt)` then `draw()`). The spine; everything hangs off it.
- `cards/` — `Card` (data; `buildStandardDeck()` in `Deck.cpp` defines the placeholder deck) and `Deck`.
- `rules/` — `GameState` (match state) and free functions `setup/advanceTurn/isGameOver` (the rulebook, stubbed).
- `render/` — `CardRenderer` (how a card looks) and `FlipAnimation` (in `Animation.{hpp,cpp}`, not `FlipAnimation.hpp`).
- `input/InputHandler` — mouse polling.
- `players/` — `Player` interface with `HumanPlayer` and `AIPlayer`. **The NPC lives in `AIPlayer.cpp`.** The interface exists so adding real AI needs no restructuring.

Key cross-cutting points:
- `Player.hpp` uses a forward declaration of `rules::GameState` (the `.cpp` files include the full header) — keep it that way to avoid a header cycle.
- The flip is a small state machine in `Game::update`: a click starts `FlipAnimation`, and the visible side swaps exactly once at the animation's midpoint (`pastMidpoint()` + a `swappedThisFlip_` guard). The reference slice tracks face/back via `Game::showFace_`, not `Card::facing` (which is a not-yet-wired stub field).
- Namespaces match folders: `app`, `cards`, `rules`, `render`, `input`, `players`.

## Tooling conventions (non-obvious)

- **clang-tidy strictness is in the script, not the config.** `.clang-tidy` keeps `WarningsAsErrors: ''` so `clangd` shows findings as editor warnings, while `scripts/run_clang_tidy.py` adds `--warnings-as-errors=*` so the pre-commit hook and CI gate are strict. To change what blocks a commit, edit the script; to change editor severity, edit `.clang-tidy`. This script is the single source of truth shared by editor, pre-commit, and CI.
- The check set is deliberately curated (high-signal, low-noise). `performance-enum-size` is disabled because it fires on raylib's headers under `--warnings-as-errors=*` (clang-tidy can't scope error-promotion away from transitive includes — see the comment in `.clang-tidy`). If a curated check fires on **project** code, fix the code; only disable a check when it fires on third-party headers you can't change.
- `scripts/run_clang_tidy.py` always re-runs `cmake -B build` to keep `compile_commands.json` in sync after new files are added, and has macOS-only PATH/sysroot fallbacks guarded to be no-ops on Linux/CI.
- pre-commit clang-format auto-fixes on commit; if it reformats and aborts, `git add -A` and commit again. It will normalize style away from any hand-written formatting (e.g. east-side `&`), which is expected.
- Only `tests/deck_test.cpp` defines `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`; other test files must not (it would duplicate the test `main`). New test files also go in the `unit_tests` source list in `CMakeLists.txt`.
- Graphics/input code (`render/`, `input/`, `app/`) is verified by build, not unit tests; logic (`cards/`, `rules/`, animation math) is unit-tested.

## Conventions to preserve

Every stub file opens with a header comment (what it's for / what to implement / how it connects) and carries `// TODO(you):` markers where logic goes. `README.md` has a "Where do I add…?" table mapping tasks to files — keep it accurate when you move or rename things. Comments explain intent, not C++ syntax.

Design spec and the task-by-task implementation plan live in `docs/superpowers/`.
