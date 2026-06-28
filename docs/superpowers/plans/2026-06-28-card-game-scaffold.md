# Card-Game Scaffold Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Initialize an empty repo with a runnable raylib + C++ skeleton for a card game — window, placeholder cards, click-to-flip animation — plus modern tooling and clearly-signposted stubs showing where real card/rule/NPC logic goes.

**Architecture:** All game logic lives in a `game_core` static library so both the playable executable and the unit tests link against it. Code is split into small single-purpose modules under `src/<area>/` (cards, rules, render, input, players, app). A runnable "reference slice" demonstrates the data → update → render → input → animate pattern end-to-end; everything game-specific is a `// TODO(you):` stub.

**Tech Stack:** C++20, CMake (FetchContent), raylib 5.5, doctest 2.4.11, clang-format, clang-tidy, pre-commit, GitHub Actions.

## Global Constraints

These apply to **every** task:

- **C++ standard:** C++20 (`CMAKE_CXX_STANDARD 20`, extensions OFF).
- **Dependencies:** Only raylib (`GIT_TAG 5.5`) and doctest (`GIT_TAG v2.4.11`), both via CMake `FetchContent`. No system-installed libraries, no other package managers.
- **Platforms:** Must build on macOS, Linux, Windows. Use only cross-platform raylib/C++ APIs.
- **Namespaces:** Each area uses its own namespace: `cards`, `rules`, `render`, `input`, `players`, `app`.
- **Formatting:** LLVM-based clang-format, `IndentWidth: 4`, `ColumnLimit: 100`, `SortIncludes: false`. The pre-commit hook auto-formats — if a commit aborts because a file was reformatted, `git add -A` and commit again.
- **Signposting (the core goal):** Every file that holds a stub MUST open with a header comment stating *what it is for, what to implement, and how it connects*. Put `// TODO(you):` exactly where game logic belongs. Comments explain intent/design, not C++ syntax.
- **Include style:** Use `#include "raylib.h"` and quoted project headers (e.g. `#include "cards/Card.hpp"`). Because `SortIncludes` is off, keep the order as written in each task.

---

## File Structure

| File | Responsibility | Created in |
|------|----------------|-----------|
| `CMakeLists.txt` | Build, fetch deps, define lib/app/tests | T1 (T3 rewrite) |
| `.gitignore` | Ignore `build/`, editor cruft | T1 |
| `src/main.cpp` | Entry point: create `Game`, run it | T1 |
| `src/app/Game.{hpp,cpp}` | Owns window + main loop (the spine) | T1 (T8 wires slice) |
| `.clang-format` / `.clang-tidy` | Format + lint config | T2 |
| `scripts/run_clang_tidy.py` | Shared tidy runner (task + pre-commit + CI) | T2 |
| `.vscode/extensions.json` | Recommended extensions (clangd, CMake Tools) | T2 |
| `.vscode/settings.json` | Inline clang-tidy + format-on-save | T2 |
| `.vscode/tasks.json` | Build / Tidy / Build+Tidy / Test / Run | T2 |
| `.vscode/launch.json` | Debug the game | T2 |
| `.pre-commit-config.yaml` | Auto-format + clang-tidy check on commit | T2 |
| `.github/workflows/ci.yml` | Build + format check + clang-tidy on push/PR | T2 |
| `src/cards/Card.{hpp,cpp}` | Card data definition | T3 |
| `src/cards/Deck.{hpp,cpp}` | Build/shuffle/draw the deck | T3 |
| `tests/deck_test.cpp` | Example deck tests (doctest main) | T3 |
| `src/render/Animation.{hpp,cpp}` | Flip animation math | T4 |
| `tests/animation_test.cpp` | Flip animation tests | T4 |
| `src/rules/GameState.{hpp,cpp}` | State of one match | T5 |
| `src/rules/Rules.{hpp,cpp}` | Setup + turn logic stubs | T5 |
| `src/players/Player.hpp` | Player interface | T6 |
| `src/players/HumanPlayer.{hpp,cpp}` | Human player stub | T6 |
| `src/players/AIPlayer.{hpp,cpp}` | NPC stub | T6 |
| `src/render/CardRenderer.{hpp,cpp}` | How a card looks | T7 |
| `src/input/InputHandler.{hpp,cpp}` | Mouse control | T7 |
| `assets/README.md` | Where art/fonts go | T9 |
| `README.md` | Build + "Where do I add X?" map | T9 |

**clang-tidy runs locally, three ways.** So the beginner gets lint feedback before pushing (and is never surprised by CI), `scripts/run_clang_tidy.py` is the single runner shared by: (1) the `clangd` editor extension, which shows clang-tidy squiggles live as they type; (2) a one-button VSCode **Build + Tidy** task (the default build); and (3) the pre-commit hook. CI runs the same script. The script always (re)configures the CMake build so `compile_commands.json` stays in sync after files are added, and passes `--warnings-as-errors=*` so the gate is real — while `.clang-tidy` keeps `WarningsAsErrors` empty so the editor shows issues as warnings, not blocking errors. clang-format runs on save (visible immediately) and in pre-commit, so code is never silently reformatted after a push.

---

### Task 1: Project skeleton — a window that opens

**Files:**
- Create: `CMakeLists.txt`
- Create: `.gitignore`
- Create: `src/main.cpp`
- Create: `src/app/Game.hpp`
- Create: `src/app/Game.cpp`

**Interfaces:**
- Produces: `app::Game(int width, int height, const char* title)` and `void app::Game::run()`. Later tasks add private members to `Game`.

- [ ] **Step 1: Create `.gitignore`**

```gitignore
# Build output
/build/

# Compiled objects
*.o
*.obj

# Tooling / editor
.cache/
compile_commands.json
.DS_Store
```

- [ ] **Step 2: Create `CMakeLists.txt` (minimal — just the window app)**

```cmake
cmake_minimum_required(VERSION 3.16)
project(sam_card_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generated for clang-tidy and IDEs.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# raylib is downloaded and built automatically on first configure.
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(sam_card_game
    src/main.cpp
    src/app/Game.cpp
)
target_include_directories(sam_card_game PRIVATE src)
target_link_libraries(sam_card_game PRIVATE raylib)
```

- [ ] **Step 3: Create `src/app/Game.hpp`**

```cpp
#pragma once

namespace app {

// Owns the application window and the main game loop. This class is the spine
// of the program: every frame it updates state, then draws the scene.
//
// TODO(you): as you build the game, add members here for the deck, players,
// the current GameState, and whatever the screen needs to show.
class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    // Runs the main loop until the user closes the window.
    void run();

private:
    void update(float dt);  // advance state by dt seconds
    void draw() const;      // render the current frame
};

}  // namespace app
```

- [ ] **Step 4: Create `src/app/Game.cpp`**

```cpp
#include "app/Game.hpp"

#include "raylib.h"

namespace app {

Game::Game(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Game::~Game() { CloseWindow(); }

void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        EndDrawing();
    }
}

void Game::update(float /*dt*/) {
    // TODO(you): per-frame game updates (input, animations, turn flow) go here.
}

void Game::draw() const {
    DrawText("Sam's Card Game", 20, 20, 20, DARKGRAY);
    // TODO(you): draw the table, cards, and UI here.
}

}  // namespace app
```

- [ ] **Step 5: Create `src/main.cpp`**

```cpp
#include "app/Game.hpp"

int main() {
    app::Game game(960, 600, "Sam's Card Game");
    game.run();
    return 0;
}
```

- [ ] **Step 6: Configure and build**

Run: `cmake -B build && cmake --build build`
Expected: PASS. First run downloads/builds raylib (slow, a few minutes); subsequent builds are fast. Produces `build/sam_card_game` (or `build/Debug/sam_card_game.exe` on Windows).

- [ ] **Step 7: Run it (manual visual check)**

Run: `./build/sam_card_game`
Expected: A 960×600 window opens titled "Sam's Card Game" with the text "Sam's Card Game" in the corner. Close the window to exit.
*(In a headless environment the build success in Step 6 is the verifiable deliverable.)*

- [ ] **Step 8: Commit**

```bash
git add .gitignore CMakeLists.txt src/
git commit -m "feat: raylib window skeleton with Game loop"
```

---

### Task 2: Code-quality tooling (format, local clang-tidy, VSCode, pre-commit, CI)

**Files:**
- Create: `.clang-format`
- Create: `.clang-tidy`
- Create: `scripts/run_clang_tidy.py`
- Create: `.vscode/extensions.json`
- Create: `.vscode/settings.json`
- Create: `.vscode/tasks.json`
- Create: `.vscode/launch.json`
- Create: `.pre-commit-config.yaml`
- Create: `.github/workflows/ci.yml`

**Interfaces:**
- Produces: `scripts/run_clang_tidy.py` — run as `python3 scripts/run_clang_tidy.py [files...]`. With no file args it checks every `src/**/*.cpp`; with file args (how pre-commit calls it) it checks only the `.cpp` among them. It (re)configures the CMake build first and runs `clang-tidy --warnings-as-errors=*`, exiting non-zero on any finding.

- [ ] **Step 1: Create `.clang-format`**

```yaml
---
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
SortIncludes: false
```

- [ ] **Step 2: Create `.clang-tidy`**

A curated, high-signal check set — small enough to avoid drowning a beginner in
nitpicks, but real enough to catch bugs. `WarningsAsErrors` is empty *here* so
the `clangd` editor extension shows findings as yellow warnings (not red errors);
the shared script adds `--warnings-as-errors=*` on the command line so the
pre-commit and CI gates are still strict.

```yaml
# Curated for a learning codebase: bug-prone + performance + a few high-value
# modernize/readability checks. Editor severity is "warning" (WarningsAsErrors
# empty); the pre-commit/CI gate treats them as errors via the shared script.
#
# TODO(you): add more checks as you get comfortable (e.g. the full modernize-*
# or readability-* groups), or tighten WarningsAsErrors to enforce in-editor.
Checks: >
  -*,
  bugprone-*,
  performance-*,
  modernize-use-override,
  modernize-use-nullptr,
  modernize-use-emplace,
  readability-braces-around-statements,
  readability-misleading-indentation,
  -bugprone-easily-swappable-parameters
WarningsAsErrors: ''
HeaderFilterRegex: 'src/.*'
```

- [ ] **Step 3: Create `scripts/run_clang_tidy.py`**

```python
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
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BUILD = ROOT / "build"
SRC = ROOT / "src"
CPP_SUFFIXES = (".cpp", ".cc", ".cxx")


def configure() -> None:
    """(Re)generate build/compile_commands.json. Cheap if already configured."""
    subprocess.run(
        ["cmake", "-B", str(BUILD), "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"],
        cwd=ROOT,
        check=True,
    )


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
    cmd += [str(f) for f in files]
    print("Running:", " ".join(cmd))
    return subprocess.run(cmd, cwd=ROOT).returncode


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
```

- [ ] **Step 4: Create `.vscode/extensions.json`**

```json
{
  "recommendations": [
    "llvm-vs-code-extensions.vscode-clangd",
    "ms-vscode.cmake-tools",
    "ms-vscode.cpptools"
  ]
}
```

- [ ] **Step 5: Create `.vscode/settings.json`**

`clangd` provides inline clang-tidy and formatting; the C/C++ extension is kept
only for its debugger, so its IntelliSense is disabled to avoid duplicate
squiggles.

```json
{
  "cmake.configureOnOpen": true,
  "cmake.configureSettings": {
    "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
  },
  "clangd.arguments": [
    "--clang-tidy",
    "--compile-commands-dir=${workspaceFolder}/build",
    "--header-insertion=never"
  ],
  "C_Cpp.intelliSenseEngine": "disabled",
  "editor.formatOnSave": true,
  "files.insertFinalNewline": true,
  "files.trimTrailingWhitespace": true,
  "[cpp]": {
    "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
  },
  "[c]": {
    "editor.defaultFormatter": "llvm-vs-code-extensions.vscode-clangd"
  }
}
```

- [ ] **Step 6: Create `.vscode/tasks.json`**

"Build + Tidy" is the default build task, so a single Ctrl/Cmd+Shift+B compiles
the project and runs clang-tidy. (On Windows, change `python3` to `python` and
`./build/sam_card_game` to `build\\Debug\\sam_card_game.exe`, or just use the
CMake Tools status-bar buttons.)

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Configure",
      "type": "shell",
      "command": "cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
      "problemMatcher": []
    },
    {
      "label": "Build",
      "type": "shell",
      "command": "cmake --build build",
      "dependsOn": "Configure",
      "group": "build",
      "problemMatcher": "$gcc"
    },
    {
      "label": "Tidy",
      "type": "shell",
      "command": "python3 scripts/run_clang_tidy.py",
      "problemMatcher": []
    },
    {
      "label": "Build + Tidy",
      "dependsOrder": "sequence",
      "dependsOn": ["Build", "Tidy"],
      "group": { "kind": "build", "isDefault": true },
      "problemMatcher": []
    },
    {
      "label": "Test",
      "type": "shell",
      "command": "ctest --test-dir build --output-on-failure",
      "dependsOn": "Build",
      "group": { "kind": "test", "isDefault": true },
      "problemMatcher": []
    },
    {
      "label": "Run",
      "type": "shell",
      "command": "./build/sam_card_game",
      "dependsOn": "Build",
      "problemMatcher": []
    }
  ]
}
```

- [ ] **Step 7: Create `.vscode/launch.json`**

```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug game",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/sam_card_game",
      "args": [],
      "cwd": "${workspaceFolder}",
      "preLaunchTask": "Build",
      "MIMode": "lldb"
    }
  ]
}
```

- [ ] **Step 8: Create `.pre-commit-config.yaml`**

```yaml
# Runs automatically before each commit. clang-format auto-fixes style; the local
# clang-tidy hook runs the same script the editor and CI use, so nothing surprises
# you after pushing. The hygiene hooks catch whitespace/merge-conflict mistakes.
repos:
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v18.1.8
    hooks:
      - id: clang-format
        types_or: [c++, c]
  - repo: https://github.com/pre-commit/pre-commit-hooks
    rev: v4.6.0
    hooks:
      - id: trailing-whitespace
      - id: end-of-file-fixer
      - id: check-yaml
      - id: check-merge-conflict
  - repo: local
    hooks:
      - id: clang-tidy
        name: clang-tidy
        entry: python3 scripts/run_clang_tidy.py
        language: system
        types_or: [c++]
        pass_filenames: true
        require_serial: true
```

- [ ] **Step 9: Create `.github/workflows/ci.yml`**

```yaml
name: CI

on:
  push:
  pull_request:

jobs:
  build:
    name: Build & Test (${{ matrix.os }})
    runs-on: ${{ matrix.os }}
    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
    steps:
      - uses: actions/checkout@v4
      - name: Install Linux deps for raylib
        if: runner.os == 'Linux'
        run: |
          sudo apt-get update
          sudo apt-get install -y libasound2-dev libx11-dev libxrandr-dev libxi-dev \
            libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
            libwayland-dev libxkbcommon-dev
      - name: Configure
        run: cmake -B build -DCMAKE_BUILD_TYPE=Release
      - name: Build
        run: cmake --build build --config Release
      - name: Test
        run: ctest --test-dir build --output-on-failure -C Release

  format:
    name: clang-format check
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install clang-format
        run: sudo apt-get update && sudo apt-get install -y clang-format
      - name: Check formatting
        run: |
          find src tests \( -name '*.cpp' -o -name '*.hpp' \) \
            -print0 | xargs -0 clang-format --dry-run --Werror

  tidy:
    name: clang-tidy
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install deps
        run: |
          sudo apt-get update
          sudo apt-get install -y clang-tidy libasound2-dev libx11-dev libxrandr-dev \
            libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
            libwayland-dev libxkbcommon-dev
      - name: Run clang-tidy (same script as local)
        run: python3 scripts/run_clang_tidy.py
```

- [ ] **Step 10: Install the pre-commit hook locally**

Run: `pre-commit install`
Expected: `pre-commit installed at .git/hooks/pre-commit`.
*(If `pre-commit` is not installed: `pip install pre-commit` or `brew install pre-commit` first.)*

- [ ] **Step 11: Verify formatting passes on existing files**

Run: `find src tests \( -name '*.cpp' -o -name '*.hpp' \) -print0 2>/dev/null | xargs -0 clang-format --dry-run --Werror`
Expected: No output, exit code 0 (existing files already conform). `tests/` may not exist yet — that is fine.

- [ ] **Step 12: Verify the tidy runner works and the scaffold passes**

Run: `python3 scripts/run_clang_tidy.py`
Expected: Configures the build, runs clang-tidy on `src/app/Game.cpp` (the only `.cpp` so far), exit code 0 with no findings.
**If clang-tidy flags something in the scaffold code:** add that specific check to the disabled list in `.clang-tidy` (e.g. `-readability-convert-member-functions-to-static`) and re-run until clean. The goal is a scaffold that passes its own gate; over-eager checks should be disabled, not worked around.

- [ ] **Step 13: Commit**

```bash
git add .clang-format .clang-tidy scripts/ .vscode/ .pre-commit-config.yaml .github/
git commit -m "chore: add formatting, local clang-tidy, VSCode tasks, pre-commit, CI"
```

---

### Task 3: Cards + Deck (and the test scaffold)

This task introduces the `game_core` library, the doctest test runner, and the first TDD cycle.

**Files:**
- Modify: `CMakeLists.txt` (full rewrite — add doctest, `game_core` lib, tests)
- Create: `src/cards/Card.hpp`
- Create: `src/cards/Card.cpp`
- Create: `src/cards/Deck.hpp`
- Create: `src/cards/Deck.cpp`
- Test: `tests/deck_test.cpp`

**Interfaces:**
- Produces:
  - `cards::Facing` enum (`Down`, `Up`).
  - `struct cards::Card { int id; std::string name; Facing facing; std::string label() const; }`.
  - `class cards::Deck` with: `static Deck buildStandardDeck()`, `std::size_t size() const`, `bool empty() const`, `void add(const Card&)`, `Card draw()`, `void shuffle(unsigned int seed)`, `const std::vector<Card>& cards() const`.

- [ ] **Step 1: Create `src/cards/Card.hpp`**

```cpp
#pragma once

#include <string>

namespace cards {

// Which side of a card is showing. Used by rendering and the flip animation.
enum class Facing { Down, Up };

// One card in the game.
//
// TODO(you): add the custom attributes your game's cards need. This is NOT a
// standard 52-card deck, so there is intentionally no suit/rank — design your
// own fields here (e.g. cost, power, card type, effect id).
struct Card {
    int id = 0;            // unique id within a deck
    std::string name;      // display name shown on the face
    Facing facing = Facing::Down;

    // Text drawn on the card face. Falls back to "Card #<id>" if unnamed.
    std::string label() const;
};

}  // namespace cards
```

- [ ] **Step 2: Create `src/cards/Card.cpp`**

```cpp
#include "cards/Card.hpp"

namespace cards {

std::string Card::label() const {
    if (!name.empty()) {
        return name;
    }
    return "Card #" + std::to_string(id);
}

}  // namespace cards
```

- [ ] **Step 3: Create `src/cards/Deck.hpp`**

```cpp
#pragma once

#include <cstddef>
#include <vector>

#include "cards/Card.hpp"

namespace cards {

// A collection of cards you can build, shuffle, and draw from.
class Deck {
public:
    // Builds the full deck for this game.
    // TODO(you): replace the placeholder cards with your real deck definition.
    static Deck buildStandardDeck();

    std::size_t size() const;
    bool empty() const;

    void add(const Card& card);

    // Removes and returns the top card. Precondition: !empty().
    Card draw();

    // Shuffles in place. The seed makes shuffles repeatable (handy for tests).
    void shuffle(unsigned int seed);

    const std::vector<Card>& cards() const;

private:
    std::vector<Card> cards_;
};

}  // namespace cards
```

- [ ] **Step 4: Create `tests/deck_test.cpp` (the failing test)**

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "cards/Deck.hpp"

TEST_CASE("a freshly built deck has the expected number of cards") {
    cards::Deck deck = cards::Deck::buildStandardDeck();
    CHECK(deck.size() == 5);
}

TEST_CASE("drawing a card reduces the deck size by one") {
    cards::Deck deck = cards::Deck::buildStandardDeck();
    const std::size_t before = deck.size();
    deck.draw();
    CHECK(deck.size() == before - 1);
}
```

- [ ] **Step 5: Replace `CMakeLists.txt` with the library + tests version**

```cmake
cmake_minimum_required(VERSION 3.16)
project(sam_card_game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generated for clang-tidy and IDEs.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# raylib is downloaded and built automatically on first configure.
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# doctest: a tiny header-only unit-test framework.
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

# All game logic lives here so the app AND the tests can use it.
# TODO(you): add each new .cpp file to this list as you create it.
add_library(game_core
    src/app/Game.cpp
    src/cards/Card.cpp
    src/cards/Deck.cpp
)
target_include_directories(game_core PUBLIC src)
target_link_libraries(game_core PUBLIC raylib)

# The playable game.
add_executable(sam_card_game src/main.cpp)
target_link_libraries(sam_card_game PRIVATE game_core)

# The unit tests.
enable_testing()
add_executable(unit_tests
    tests/deck_test.cpp
)
target_link_libraries(unit_tests PRIVATE game_core doctest::doctest)
add_test(NAME unit_tests COMMAND unit_tests)
```

- [ ] **Step 6: Reconfigure and build — verify the test FAILS to build**

Run: `cmake -B build && cmake --build build`
Expected: FAIL — link/compile error referencing `cards::Deck::buildStandardDeck` (declared in the header, not yet defined). This confirms the test is exercising real, missing code.

- [ ] **Step 7: Create `src/cards/Deck.cpp` (make it pass)**

```cpp
#include "cards/Deck.hpp"

#include <algorithm>
#include <random>

namespace cards {

Deck Deck::buildStandardDeck() {
    Deck deck;
    // TODO(you): replace these placeholder cards with your game's real deck.
    // If you change the count here, update the test in tests/deck_test.cpp.
    for (int i = 1; i <= 5; ++i) {
        Card card;
        card.id = i;
        card.name = "Card " + std::to_string(i);
        deck.add(card);
    }
    return deck;
}

std::size_t Deck::size() const { return cards_.size(); }

bool Deck::empty() const { return cards_.empty(); }

void Deck::add(const Card& card) { cards_.push_back(card); }

Card Deck::draw() {
    Card top = cards_.back();
    cards_.pop_back();
    return top;
}

void Deck::shuffle(unsigned int seed) {
    std::mt19937 rng(seed);
    std::shuffle(cards_.begin(), cards_.end(), rng);
}

const std::vector<Card>& Deck::cards() const { return cards_; }

}  // namespace cards
```

- [ ] **Step 8: Build and run the tests — verify they PASS**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS — `unit_tests` passes (2 test cases).

- [ ] **Step 9: Commit**

```bash
git add CMakeLists.txt src/cards/ tests/deck_test.cpp
git commit -m "feat: add Card and Deck with doctest test scaffold"
```

---

### Task 4: Flip animation (pure, testable math)

**Files:**
- Create: `src/render/Animation.hpp`
- Create: `src/render/Animation.cpp`
- Test: `tests/animation_test.cpp`
- Modify: `CMakeLists.txt` (add the two new files)

**Interfaces:**
- Produces: `class render::FlipAnimation` with `void start(float durationSeconds)`, `void update(float dt)`, `bool active() const`, `float progress() const`, `float horizontalScale() const`, `bool pastMidpoint() const`.

- [ ] **Step 1: Create `src/render/Animation.hpp`**

```cpp
#pragma once

namespace render {

// Drives a card-flip animation. Progress runs 0 -> 1 over `duration` seconds.
// The horizontal scale shrinks from 1 to 0 at the midpoint and back to 1, which
// reads as a card flipping. Swap the visible side at the midpoint.
//
// TODO(you): add more animation types as your game needs them (deal, discard,
// hover-lift, ...). Keep them as small, self-contained helpers like this one.
class FlipAnimation {
public:
    // Starts a flip lasting `durationSeconds`.
    void start(float durationSeconds);

    // Advances by `dt` seconds. A no-op when no flip is running.
    void update(float dt);

    bool active() const;

    // Normalized progress in [0, 1].
    float progress() const;

    // Horizontal scale factor in [0, 1] for the current progress.
    float horizontalScale() const;

    // True once progress has reached/passed the midpoint (swap face/back here).
    bool pastMidpoint() const;

private:
    bool active_ = false;
    float elapsed_ = 0.0f;
    float duration_ = 0.0f;
};

}  // namespace render
```

- [ ] **Step 2: Create `tests/animation_test.cpp` (failing test)**

Note: this file must NOT define `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` — `deck_test.cpp` already provides the test `main()` for the shared `unit_tests` executable.

```cpp
#include <doctest/doctest.h>

#include "render/Animation.hpp"

TEST_CASE("a flip starts at full width, before the midpoint, and active") {
    render::FlipAnimation flip;
    flip.start(1.0f);
    CHECK(flip.horizontalScale() == doctest::Approx(1.0f));
    CHECK_FALSE(flip.pastMidpoint());
    CHECK(flip.active());
}

TEST_CASE("a flip is squashed to zero width at the midpoint") {
    render::FlipAnimation flip;
    flip.start(1.0f);
    flip.update(0.5f);
    CHECK(flip.horizontalScale() == doctest::Approx(0.0f));
    CHECK(flip.pastMidpoint());
}

TEST_CASE("a flip returns to full width and finishes at the end") {
    render::FlipAnimation flip;
    flip.start(1.0f);
    flip.update(1.0f);
    CHECK(flip.horizontalScale() == doctest::Approx(1.0f));
    CHECK_FALSE(flip.active());
}
```

- [ ] **Step 3: Add the new files to `CMakeLists.txt`**

In the `add_library(game_core ...)` list, add `src/render/Animation.cpp`:

```cmake
add_library(game_core
    src/app/Game.cpp
    src/cards/Card.cpp
    src/cards/Deck.cpp
    src/render/Animation.cpp
)
```

In the `add_executable(unit_tests ...)` list, add `tests/animation_test.cpp`:

```cmake
add_executable(unit_tests
    tests/deck_test.cpp
    tests/animation_test.cpp
)
```

- [ ] **Step 4: Build — verify it FAILS**

Run: `cmake -B build && cmake --build build`
Expected: FAIL — undefined references to `render::FlipAnimation` methods (declared, not defined).

- [ ] **Step 5: Create `src/render/Animation.cpp`**

```cpp
#include "render/Animation.hpp"

#include <algorithm>
#include <cmath>

namespace render {

void FlipAnimation::start(float durationSeconds) {
    active_ = true;
    elapsed_ = 0.0f;
    duration_ = durationSeconds;
}

void FlipAnimation::update(float dt) {
    if (!active_) {
        return;
    }
    elapsed_ += dt;
    if (elapsed_ >= duration_) {
        elapsed_ = duration_;
        active_ = false;
    }
}

bool FlipAnimation::active() const { return active_; }

float FlipAnimation::progress() const {
    if (duration_ <= 0.0f) {
        return 1.0f;
    }
    return std::clamp(elapsed_ / duration_, 0.0f, 1.0f);
}

float FlipAnimation::horizontalScale() const {
    // 1 at the start, 0 at the midpoint, 1 at the end.
    return std::fabs(1.0f - 2.0f * progress());
}

bool FlipAnimation::pastMidpoint() const { return progress() >= 0.5f; }

}  // namespace render
```

- [ ] **Step 6: Build and test — verify PASS**

Run: `cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS — all test cases in `unit_tests` pass.

- [ ] **Step 7: Commit**

```bash
git add CMakeLists.txt src/render/Animation.hpp src/render/Animation.cpp tests/animation_test.cpp
git commit -m "feat: add tested flip-animation helper"
```

---

### Task 5: GameState + Rules stubs

**Files:**
- Create: `src/rules/GameState.hpp`
- Create: `src/rules/GameState.cpp`
- Create: `src/rules/Rules.hpp`
- Create: `src/rules/Rules.cpp`
- Modify: `CMakeLists.txt` (add the two new `.cpp`)

**Interfaces:**
- Produces:
  - `struct rules::GameState { cards::Deck drawPile; std::vector<cards::Card> table; int currentPlayer; void reset(); }`.
  - `void rules::setup(GameState&)`, `void rules::advanceTurn(GameState&)`, `bool rules::isGameOver(const GameState&)`.

- [ ] **Step 1: Create `src/rules/GameState.hpp`**

```cpp
#pragma once

#include <vector>

#include "cards/Card.hpp"
#include "cards/Deck.hpp"

namespace rules {

// All the state of a single match. Keep this as plain data — the rules logic in
// Rules.hpp reads and changes it.
//
// TODO(you): add the fields your game tracks (player hands, discard pile,
// scores, current phase, ...).
struct GameState {
    cards::Deck drawPile;             // cards left to draw
    std::vector<cards::Card> table;   // cards currently shown on the table
    int currentPlayer = 0;            // index of the player whose turn it is

    // Clears everything back to an empty match.
    void reset();
};

}  // namespace rules
```

- [ ] **Step 2: Create `src/rules/GameState.cpp`**

```cpp
#include "rules/GameState.hpp"

namespace rules {

void GameState::reset() {
    drawPile = cards::Deck{};
    table.clear();
    currentPlayer = 0;
}

}  // namespace rules
```

- [ ] **Step 3: Create `src/rules/Rules.hpp`**

```cpp
#pragma once

#include "rules/GameState.hpp"

namespace rules {

// The rulebook for the game: setup and turn flow. Everything here is a stub.
//
// TODO(you): this is where your game's RULES live. Implement these functions in
// Rules.cpp, and add new ones (e.g. isLegalMove, applyMove, scoreRound).

// Deals the opening table and chooses the first player.
void setup(GameState& state);

// Advances to the next player's turn.
void advanceTurn(GameState& state);

// Returns true when the match is over.
bool isGameOver(const GameState& state);

}  // namespace rules
```

- [ ] **Step 4: Create `src/rules/Rules.cpp`**

```cpp
#include "rules/Rules.hpp"

namespace rules {

void setup(GameState& state) {
    // TODO(you): deal the opening hand/table and choose the first player.
    state.reset();
    state.drawPile = cards::Deck::buildStandardDeck();
}

void advanceTurn(GameState& state) {
    // TODO(you): implement real turn order (number of players, phases, skips).
    // Placeholder: toggle between two players.
    state.currentPlayer = 1 - state.currentPlayer;
}

bool isGameOver(const GameState& state) {
    // TODO(you): replace with your real end/win condition.
    return state.drawPile.empty();
}

}  // namespace rules
```

- [ ] **Step 5: Add the new files to `CMakeLists.txt`**

Add `src/rules/GameState.cpp` and `src/rules/Rules.cpp` to the `game_core` source list:

```cmake
add_library(game_core
    src/app/Game.cpp
    src/cards/Card.cpp
    src/cards/Deck.cpp
    src/render/Animation.cpp
    src/rules/GameState.cpp
    src/rules/Rules.cpp
)
```

- [ ] **Step 6: Build and test — verify PASS**

Run: `cmake -B build && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS — everything compiles and existing tests still pass.

- [ ] **Step 7: Commit**

```bash
git add CMakeLists.txt src/rules/
git commit -m "feat: add GameState and Rules stubs"
```

---

### Task 6: Player interface + Human and AI (NPC) stubs

**Files:**
- Create: `src/players/Player.hpp`
- Create: `src/players/HumanPlayer.hpp`
- Create: `src/players/HumanPlayer.cpp`
- Create: `src/players/AIPlayer.hpp`
- Create: `src/players/AIPlayer.cpp`
- Modify: `CMakeLists.txt` (add the two new `.cpp`)

**Interfaces:**
- Consumes: `rules::GameState` (from Task 5).
- Produces:
  - `class players::Player` (abstract): `virtual const std::string& name() const = 0;`, `virtual void takeTurn(rules::GameState&) = 0;`.
  - `class players::HumanPlayer` / `class players::AIPlayer`, each constructed with `explicit (std::string name)`.

- [ ] **Step 1: Create `src/players/Player.hpp`**

```cpp
#pragma once

#include <string>

namespace rules {
struct GameState;  // forward declaration — full definition in rules/GameState.hpp
}

namespace players {

// Common interface for every participant — human or NPC. The turn loop calls
// takeTurn() without caring which kind of player it is, so adding the computer
// opponent later needs no changes elsewhere.
class Player {
public:
    virtual ~Player() = default;

    // Display name of this player.
    virtual const std::string& name() const = 0;

    // Decide and apply this player's move for the current turn.
    // TODO(you): define what a "move" is and how it changes the GameState.
    virtual void takeTurn(rules::GameState& state) = 0;
};

}  // namespace players
```

- [ ] **Step 2: Create `src/players/HumanPlayer.hpp`**

```cpp
#pragma once

#include <string>

#include "players/Player.hpp"

namespace players {

// A human player. Their moves ultimately come from mouse/keyboard input.
class HumanPlayer : public Player {
public:
    explicit HumanPlayer(std::string name);

    const std::string& name() const override;
    void takeTurn(rules::GameState& state) override;

private:
    std::string name_;
};

}  // namespace players
```

- [ ] **Step 3: Create `src/players/HumanPlayer.cpp`**

```cpp
#include "players/HumanPlayer.hpp"

#include <utility>

#include "rules/GameState.hpp"

namespace players {

HumanPlayer::HumanPlayer(std::string name) : name_(std::move(name)) {}

const std::string& HumanPlayer::name() const { return name_; }

void HumanPlayer::takeTurn(rules::GameState& /*state*/) {
    // TODO(you): a human's move comes from input. When you build real turns,
    // read the chosen card/action from InputHandler and apply it to `state`.
}

}  // namespace players
```

- [ ] **Step 4: Create `src/players/AIPlayer.hpp`**

```cpp
#pragma once

#include <string>

#include "players/Player.hpp"

namespace players {

// The NPC / computer opponent.
class AIPlayer : public Player {
public:
    explicit AIPlayer(std::string name);

    const std::string& name() const override;
    void takeTurn(rules::GameState& state) override;

private:
    std::string name_;
};

}  // namespace players
```

- [ ] **Step 5: Create `src/players/AIPlayer.cpp`**

```cpp
#include "players/AIPlayer.hpp"

#include <utility>

#include "rules/GameState.hpp"

namespace players {

AIPlayer::AIPlayer(std::string name) : name_(std::move(name)) {}

const std::string& AIPlayer::name() const { return name_; }

void AIPlayer::takeTurn(rules::GameState& /*state*/) {
    // TODO(you): THIS IS THE NPC. Implement the computer opponent's decision-
    // making here: read `state`, pick a legal move per Rules, and apply it.
    // Start simple (e.g. play the first legal move) before getting clever.
}

}  // namespace players
```

- [ ] **Step 6: Add the new files to `CMakeLists.txt`**

Add `src/players/HumanPlayer.cpp` and `src/players/AIPlayer.cpp` to `game_core`:

```cmake
add_library(game_core
    src/app/Game.cpp
    src/cards/Card.cpp
    src/cards/Deck.cpp
    src/render/Animation.cpp
    src/rules/GameState.cpp
    src/rules/Rules.cpp
    src/players/HumanPlayer.cpp
    src/players/AIPlayer.cpp
)
```

- [ ] **Step 7: Build and test — verify PASS**

Run: `cmake -B build && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS.

- [ ] **Step 8: Commit**

```bash
git add CMakeLists.txt src/players/
git commit -m "feat: add Player interface with Human and AI (NPC) stubs"
```

---

### Task 7: CardRenderer + InputHandler

**Files:**
- Create: `src/render/CardRenderer.hpp`
- Create: `src/render/CardRenderer.cpp`
- Create: `src/input/InputHandler.hpp`
- Create: `src/input/InputHandler.cpp`
- Modify: `CMakeLists.txt` (add the two new `.cpp`)

**Interfaces:**
- Consumes: `cards::Card`, raylib `Rectangle`/`Color`.
- Produces:
  - `class render::CardRenderer` with `void draw(const cards::Card& card, Rectangle bounds, float horizontalScale, bool showFace) const;`.
  - `class input::InputHandler` with `bool clickedIn(Rectangle bounds) const;` and `bool hovering(Rectangle bounds) const;`.

- [ ] **Step 1: Create `src/render/CardRenderer.hpp`**

```cpp
#pragma once

#include "raylib.h"

#include "cards/Card.hpp"

namespace render {

// Draws a single card. For now a card is a rounded rectangle with text; later
// you can swap this for textured art loaded from assets/.
//
// TODO(you): this is WHERE A CARD LOOKS the way it does. Customize colors,
// corners, fonts, and (eventually) card images here.
class CardRenderer {
public:
    // Draws `card` inside `bounds`. `horizontalScale` (1.0 = full width) lets a
    // flip animation squash the card horizontally. `showFace` chooses the face
    // (true) or the back (false).
    void draw(const cards::Card& card, Rectangle bounds, float horizontalScale,
              bool showFace) const;
};

}  // namespace render
```

- [ ] **Step 2: Create `src/render/CardRenderer.cpp`**

```cpp
#include "render/CardRenderer.hpp"

#include <string>

namespace render {

void CardRenderer::draw(const cards::Card& card, Rectangle bounds, float horizontalScale,
                        bool showFace) const {
    // Squash horizontally around the card's center to fake a 3D flip.
    const float squashedWidth = bounds.width * horizontalScale;
    Rectangle drawn = bounds;
    drawn.x = bounds.x + (bounds.width - squashedWidth) / 2.0f;
    drawn.width = squashedWidth;

    // TODO(you): replace these flat colors with your card art / styling.
    const Color faceColor = Color{245, 245, 245, 255};
    const Color backColor = Color{40, 90, 150, 255};
    const Color border = DARKGRAY;

    DrawRectangleRounded(drawn, 0.1f, 8, showFace ? faceColor : backColor);
    DrawRectangleLinesEx(drawn, 2.0f, border);

    // Only draw text once the card is wide enough to read (not mid-flip).
    if (showFace && horizontalScale > 0.5f) {
        const std::string label = card.label();
        const int fontSize = 20;
        const int textWidth = MeasureText(label.c_str(), fontSize);
        const int textX = static_cast<int>(drawn.x + (drawn.width - textWidth) / 2.0f);
        const int textY = static_cast<int>(drawn.y + drawn.height / 2.0f - fontSize / 2.0f);
        DrawText(label.c_str(), textX, textY, fontSize, DARKGRAY);
    }
}

}  // namespace render
```

- [ ] **Step 3: Create `src/input/InputHandler.hpp`**

```cpp
#pragma once

#include "raylib.h"

namespace input {

// Reads the mouse. This is the single place mouse logic lives, so it is easy to
// extend.
//
// TODO(you): add drag-and-drop, right-click menus, hover tooltips, or keyboard
// handling here as your game grows.
class InputHandler {
public:
    // True on the frame the left mouse button is pressed inside `bounds`.
    bool clickedIn(Rectangle bounds) const;

    // True while the mouse hovers over `bounds`.
    bool hovering(Rectangle bounds) const;
};

}  // namespace input
```

- [ ] **Step 4: Create `src/input/InputHandler.cpp`**

```cpp
#include "input/InputHandler.hpp"

namespace input {

bool InputHandler::clickedIn(Rectangle bounds) const {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), bounds);
}

bool InputHandler::hovering(Rectangle bounds) const {
    return CheckCollisionPointRec(GetMousePosition(), bounds);
}

}  // namespace input
```

- [ ] **Step 5: Add the new files to `CMakeLists.txt`**

Add `src/render/CardRenderer.cpp` and `src/input/InputHandler.cpp` to `game_core`:

```cmake
add_library(game_core
    src/app/Game.cpp
    src/cards/Card.cpp
    src/cards/Deck.cpp
    src/render/Animation.cpp
    src/render/CardRenderer.cpp
    src/rules/GameState.cpp
    src/rules/Rules.cpp
    src/players/HumanPlayer.cpp
    src/players/AIPlayer.cpp
    src/input/InputHandler.cpp
)
```

- [ ] **Step 6: Build and test — verify PASS**

Run: `cmake -B build && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS — compiles and existing tests still pass.

- [ ] **Step 7: Commit**

```bash
git add CMakeLists.txt src/render/CardRenderer.hpp src/render/CardRenderer.cpp src/input/
git commit -m "feat: add CardRenderer and mouse InputHandler"
```

---

### Task 8: Wire the interactive slice into Game

This connects card data, rendering, input, and animation so clicking a card flips it. No CMake change (`Game.cpp` is already built).

**Files:**
- Modify: `src/app/Game.hpp`
- Modify: `src/app/Game.cpp`

**Interfaces:**
- Consumes: `cards::Card`, `render::CardRenderer`, `render::FlipAnimation`, `input::InputHandler`, raylib `Rectangle`.

- [ ] **Step 1: Replace `src/app/Game.hpp` with the wired version**

```cpp
#pragma once

#include "raylib.h"

#include "cards/Card.hpp"
#include "input/InputHandler.hpp"
#include "render/Animation.hpp"
#include "render/CardRenderer.hpp"

namespace app {

// Owns the application window and the main game loop. This class is the spine of
// the program: every frame it updates state, then draws the scene.
//
// TODO(you): grow this into the real game — replace the single demo card with a
// deck, a GameState, and players, and call into rules/ each turn.
class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    // Runs the main loop until the user closes the window.
    void run();

private:
    void update(float dt);            // advance state by dt seconds
    void draw() const;                // render the current frame
    Rectangle cardBounds() const;     // where the demo card sits on screen

    cards::Card demoCard_;            // the one card in the reference slice
    bool showFace_ = false;           // is the face (vs back) currently up?
    bool swappedThisFlip_ = false;    // have we swapped sides during this flip?

    render::FlipAnimation flip_;
    render::CardRenderer renderer_;
    input::InputHandler input_;
};

}  // namespace app
```

- [ ] **Step 2: Replace `src/app/Game.cpp` with the wired version**

```cpp
#include "app/Game.hpp"

#include "raylib.h"

namespace app {

namespace {
constexpr float kFlipDuration = 0.4f;  // seconds for one flip
constexpr float kCardWidth = 180.0f;
constexpr float kCardHeight = 260.0f;
}  // namespace

Game::Game(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    // TODO(you): build the real deck and deal a hand instead of one demo card.
    demoCard_.id = 1;
    demoCard_.name = "Hello Card";
}

Game::~Game() { CloseWindow(); }

Rectangle Game::cardBounds() const {
    return Rectangle{(GetScreenWidth() - kCardWidth) / 2.0f,
                     (GetScreenHeight() - kCardHeight) / 2.0f, kCardWidth, kCardHeight};
}

void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        EndDrawing();
    }
}

void Game::update(float dt) {
    // Click the card to start a flip (ignored while one is already running).
    if (!flip_.active() && input_.clickedIn(cardBounds())) {
        flip_.start(kFlipDuration);
        swappedThisFlip_ = false;
    }

    flip_.update(dt);

    // Swap which side is visible exactly once, at the midpoint of the flip.
    if (flip_.active() && flip_.pastMidpoint() && !swappedThisFlip_) {
        showFace_ = !showFace_;
        swappedThisFlip_ = true;
    }
}

void Game::draw() const {
    DrawText("Click the card to flip it.", 20, 20, 20, DARKGRAY);

    const float scale = flip_.active() ? flip_.horizontalScale() : 1.0f;
    renderer_.draw(demoCard_, cardBounds(), scale, showFace_);
}

}  // namespace app
```

- [ ] **Step 3: Build and test — verify PASS**

Run: `cmake -B build && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS — compiles and all tests pass.

- [ ] **Step 4: Run it (manual visual check)**

Run: `./build/sam_card_game`
Expected: A window shows a single card centered on screen. Clicking the card plays a flip animation (it squashes horizontally to nothing and back) and reveals the other side ("Hello Card" face vs blue back). Close to exit.
*(In a headless environment, Step 3's build + test success is the verifiable deliverable.)*

- [ ] **Step 5: Commit**

```bash
git add src/app/Game.hpp src/app/Game.cpp
git commit -m "feat: wire click-to-flip reference slice into Game"
```

---

### Task 9: README, assets placeholder, and beginner signposting

**Files:**
- Create: `assets/README.md`
- Create: `README.md`

**Interfaces:** None (docs only).

- [ ] **Step 1: Create `assets/README.md`**

```markdown
# Assets

Put card art, fonts, and sounds here.

Right now the game draws cards as plain rounded rectangles (see
`src/render/CardRenderer.cpp`) and needs no files in this folder. When you are
ready for custom card looks:

1. Drop image files (e.g. `card_back.png`, `dragon.png`) in this folder.
2. Load them in `CardRenderer` with raylib's `LoadTexture(...)`.
3. Draw them with `DrawTexturePro(...)` instead of `DrawRectangleRounded(...)`.

Tip: load each texture once (not every frame) and unload it when the game exits.
```

- [ ] **Step 2: Create `README.md`**

```markdown
# Sam's Card Game

A card game built with [raylib](https://www.raylib.com/) and C++. This repo is a
**scaffold**: it runs and shows a card you can click to flip, but the real deck,
rules, and computer opponent are left as clearly-marked `// TODO(you):` stubs for
you to fill in.

## Build & run

You need CMake and a C++20 compiler. raylib and the test framework are downloaded
automatically the first time you configure.

```bash
cmake -B build
cmake --build build
./build/sam_card_game        # on Windows: build\Debug\sam_card_game.exe
```

Click the card on screen to see it flip.

## Run the tests

```bash
ctest --test-dir build --output-on-failure
```

## Where do I add…?

| I want to… | Edit this file |
|------------|----------------|
| Define a new card / card fields | `src/cards/Card.hpp` |
| Change what cards are in the deck | `src/cards/Deck.cpp` (`buildStandardDeck`) |
| Write the game's rules (setup, turns, win condition) | `src/rules/Rules.cpp` |
| Track new match state (hands, scores, piles) | `src/rules/GameState.hpp` |
| Change how a card looks (colors, art) | `src/render/CardRenderer.cpp` |
| Change/add animations (flip, deal, …) | `src/render/Animation.{hpp,cpp}` |
| Handle mouse/keyboard input | `src/input/InputHandler.cpp` |
| Implement the computer opponent (NPC) | `src/players/AIPlayer.cpp` |
| Change the window, loop, or what's on screen | `src/app/Game.cpp` |
| Add a test for your rules | `tests/` (copy `deck_test.cpp` as a template) |

## Project layout

```
src/
  app/      window + main loop (the spine)
  cards/    card data and the deck
  rules/    game state and the rulebook
  render/   how cards look + animations
  input/    mouse/keyboard handling
  players/  Human and AI (NPC) players
tests/      unit tests (doctest)
assets/     card art / fonts (placeholders for now)
```

## Code quality

Formatting and linting run **locally**, so nothing is changed behind your back
after you push.

- **In VSCode (recommended):** open the folder and accept the recommended
  extensions. You then get clang-tidy warnings live as you type (via `clangd`)
  and format-on-save. Press **Ctrl/Cmd+Shift+B** to **Build + Tidy** in one
  step; other tasks (Tidy, Test, Run) are under *Terminal → Run Task*.
- **Format:** `clang-format` (config in `.clang-format`).
- **Lint:** `clang-tidy` (config in `.clang-tidy`). Run it anytime with
  `python3 scripts/run_clang_tidy.py`.
- **Pre-commit:** run `pre-commit install` once. Commits then auto-format and run
  clang-tidy, so issues are caught before they reach CI.
- **CI:** GitHub Actions builds on macOS/Linux/Windows, checks formatting, and
  runs the same clang-tidy script.
```

- [ ] **Step 3: Verify the README's build instructions actually work from clean**

Run: `rm -rf build && cmake -B build && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: PASS — clean configure, build, and tests all succeed.

- [ ] **Step 4: Signposting review (the core goal)**

Confirm by inspection that **every** stub file below opens with a header comment and has a `// TODO(you):` where logic goes:
`src/cards/Card.hpp`, `src/cards/Deck.cpp`, `src/rules/Rules.cpp`, `src/rules/GameState.hpp`, `src/render/CardRenderer.cpp`, `src/render/Animation.hpp`, `src/input/InputHandler.hpp`, `src/players/AIPlayer.cpp`, `src/app/Game.cpp`.
Expected: All present. Fix any that are missing before committing.

- [ ] **Step 5: Commit**

```bash
git add README.md assets/README.md
git commit -m "docs: add README with where-to-add map and assets placeholder"
```

---

## Self-Review

**1. Spec coverage:**
- CMake + FetchContent → T1, T3. ✓
- Cross-platform → T1 (portable APIs), T2 (CI matrix macOS/Linux/Windows). ✓
- Runnable reference slice (window, placeholder card, click-to-flip animation) → T1, T4, T7, T8. ✓
- clang-format / clang-tidy / pre-commit / CI → T2. ✓
- clang-tidy local (editor via clangd + one-button VSCode task + pre-commit) → T2 (`.vscode/`, `scripts/run_clang_tidy.py`, local pre-commit hook). ✓
- Lightweight tests (CTest + doctest, example test) → T3, T4. ✓
- Module layout (cards/rules/render/input/players/app) → T3–T8. ✓
- Player interface + AIPlayer (NPC) placeholder → T6. ✓
- Beginner signposting (file headers, `TODO(you)`, README "where do I add X" table) → all stub tasks + T9. ✓
- assets placeholder → T9. ✓

**2. Placeholder scan:** No "TBD"/"implement later" left in the plan itself; every code step contains complete code. `// TODO(you):` markers are intentional product features (signposts in delivered source), not plan placeholders.

**3. Type consistency:** `Deck` API (`buildStandardDeck`, `size`, `empty`, `add`, `draw`, `shuffle`, `cards`) consistent across T3 header/impl/tests. `FlipAnimation` API (`start`, `update`, `active`, `progress`, `horizontalScale`, `pastMidpoint`) consistent across T4 and its use in T8. `GameState` fields (`drawPile`, `table`, `currentPlayer`, `reset`) consistent across T5/T6. `CardRenderer::draw(card, bounds, horizontalScale, showFace)` and `InputHandler::clickedIn/hovering` signatures match between T7 definition and T8 use. `Card::label()` defined T3, used T7. ✓

**Risk logged:** the scaffold must pass its own `clang-tidy --warnings-as-errors=*` gate. The check set is curated to be conservative, and T2 Step 12 explicitly verifies this and disables any over-eager check inline — so execution cannot be blocked by the linter walling the scaffold.
