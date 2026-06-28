# Design: raylib C++ Card-Game Scaffold

**Date:** 2026-06-28
**Status:** Approved

## Goal

Initialize an empty repository with a clean, well-organized skeleton for a
raylib + C++ card game. The game will eventually have a **custom card deck**
(not a standard 52-card deck), **game-specific rules** (setup, turn order,
turn-based play), and an **NPC opponent** that can play against humans.

**This task implements no real game logic.** It produces the repo structure,
modern C++ tooling (formatting, linting, code cleanliness), and clearly-marked
placeholders so the actual logic can be filled in later.

The primary success criterion: **someone with little coding experience but some
C++ familiarity can clone the repo, build and run it, and immediately understand
where each kind of game logic belongs** (card definitions, rules, rendering,
animation, mouse input, the NPC).

## Decisions (locked during brainstorming)

| Decision | Choice |
|----------|--------|
| Build & dependency management | CMake + `FetchContent` (auto-downloads raylib & test framework) |
| Target platforms | Cross-platform: macOS, Linux, Windows |
| Reference code level | Runnable reference slice (window + placeholder cards + click-to-flip animation) |
| Formatting | `clang-format` (LLVM-based style) |
| Linting | `clang-tidy` (bug/modernize/readability checks) |
| Git guardrail | `pre-commit` hooks (auto format + lint on commit) |
| CI | GitHub Actions (build + format/lint check on push/PR) |
| Tests | Lightweight: `tests/` wired into CTest with one example test (doctest) |
| C++ standard | C++20 |

## Architecture & Folder Layout

The code is split into small, single-purpose modules. Each subsystem maps to one
concept so a beginner can open a folder and know "this is where X goes."

```
sam-card-game/
├── CMakeLists.txt              # builds everything, fetches raylib + doctest
├── README.md                   # how to build/run + "where do I add X?" map
├── LICENSE
├── .clang-format               # formatting style config
├── .clang-tidy                 # lint check config
├── .pre-commit-config.yaml     # auto-format/lint on commit
├── .gitignore                  # ignores build/ and editor cruft
├── .github/
│   └── workflows/
│       └── ci.yml              # build + format/lint check on push/PR
├── assets/
│   └── README.md               # placeholder; where card art & fonts go
├── docs/
│   └── superpowers/specs/      # this design doc lives here
├── src/
│   ├── main.cpp                # entry point — creates Game, runs the loop
│   ├── app/
│   │   ├── Game.hpp
│   │   └── Game.cpp            # owns the game loop: Update() + Draw()
│   ├── cards/
│   │   ├── Card.hpp
│   │   ├── Card.cpp            # ← CARD DATA: define your custom card here
│   │   ├── Deck.hpp
│   │   └── Deck.cpp            # ← build/shuffle/draw the deck (stubs)
│   ├── rules/
│   │   ├── GameState.hpp
│   │   ├── GameState.cpp       # the state of one match
│   │   ├── Rules.hpp
│   │   └── Rules.cpp           # ← GAME RULES: setup & turn logic (stubs)
│   ├── render/
│   │   ├── CardRenderer.hpp
│   │   ├── CardRenderer.cpp    # ← how a card LOOKS when drawn
│   │   ├── Animation.hpp
│   │   └── Animation.cpp       # ← flip animation (customizable)
│   ├── input/
│   │   ├── InputHandler.hpp
│   │   └── InputHandler.cpp    # ← MOUSE control (easily extendable)
│   └── players/
│       ├── Player.hpp          # interface: human & NPC share this
│       ├── HumanPlayer.hpp
│       ├── HumanPlayer.cpp
│       ├── AIPlayer.hpp
│       └── AIPlayer.cpp        # ← NPC LIVES HERE (stub for later)
└── tests/
    └── deck_test.cpp           # one example test + CTest wiring
```

### Module responsibilities

- **`app/Game`** — Owns the raylib window and the main loop. Each frame it calls
  `Update(dt)` (advance state, animations, input) then `Draw()` (render the
  scene). This is the spine everything else hangs off.
- **`cards/Card`** — The data definition for a single card: identity, display
  name, and whatever custom attributes this game needs. *This is where card
  definitions go.*
- **`cards/Deck`** — Builds the custom deck, shuffles, and draws cards. Stubbed.
- **`rules/GameState`** — Holds the state of one match (whose turn, piles, hands,
  score). Plain data plus simple accessors.
- **`rules/Rules`** — Setup and turn-based logic: deal, validate moves, advance
  turns, detect win conditions. *This is where the game rules go.* Stubbed.
- **`render/CardRenderer`** — Draws a card's visual appearance. In the slice it
  draws a rounded rectangle + text placeholder; later it draws custom art.
- **`render/Animation`** — A small, reusable animation helper. Drives the
  card-flip (horizontal scale 1→0→1, swap face/back at the midpoint) and is
  written so new animations are easy to add.
- **`input/InputHandler`** — Reads mouse position/clicks and reports which card
  is hovered/clicked. *This is where mouse control is extended.*
- **`players/Player`** — An interface both `HumanPlayer` and `AIPlayer`
  implement, so the turn loop treats any player uniformly.
- **`players/AIPlayer`** — The NPC. Stubbed now; the interface means adding real
  AI later requires no restructuring. *This is where the NPC lives.*

The `Player` interface exists from day one specifically so the eventual NPC just
fills in `AIPlayer` without touching the rest of the architecture.

## The Runnable Reference Slice

What works the moment you build and run, before any real logic is added:

1. `main.cpp` creates a `Game`; `Game` opens a raylib window and runs the loop.
2. `CardRenderer` draws one or two **placeholder cards** as rounded rectangles
   with text — no image assets required yet.
3. `InputHandler` detects mouse hover and clicks over a card.
4. Clicking a card triggers `Animation` to **flip it**: horizontal scale shrinks
   to zero and back, swapping the visible face/back at the midpoint.

Everything game-specific — real card definitions, deck contents, rules, turn
flow, AI decisions — is a clearly-marked stub that returns placeholder values.
The point is that the beginner sees the full pattern (data → update → render →
input → animate) working end-to-end and copies it when filling in real logic.

## Tooling & Conventions

- **Build:** CMake with C++20. `FetchContent` pulls raylib and doctest at
  configure time, so a fresh clone builds with:
  `cmake -B build && cmake --build build` — nothing to pre-install.
- **Formatting:** `.clang-format` (LLVM-based style).
- **Linting:** `.clang-tidy` enabling bug-prone, modernize, and readability
  check groups (tuned to avoid noise for a learning codebase).
- **pre-commit:** `.pre-commit-config.yaml` runs clang-format and clang-tidy
  before each commit so unformatted/unlinted code never lands.
- **CI:** GitHub Actions workflow builds on macOS, Linux, and Windows and fails
  the build on formatting or lint drift.
- **Tests:** `tests/deck_test.cpp` wired into CTest using doctest, with one
  example assertion, demonstrating where to test rules.

## Signposting for Beginners (core goal)

This is the feature that matters most, so it is explicit:

- Every stub file opens with a short header comment stating **what the file is
  for, what to implement, and how it connects** to the rest.
- `// TODO(you):` markers sit exactly where logic belongs.
- The README contains a **"Where do I add…?" table** mapping each common task
  (add a new card, change a rule, change how cards look, add a mouse
  interaction, implement the NPC) to the specific file.
- Comments explain *intent and design*, not C++ syntax — the reader knows C++,
  they need to know *this codebase*.

## Out of Scope

- Any real card definitions, deck contents, or game rules.
- Any real AI/NPC decision-making.
- Real card artwork or sound assets (placeholders only).
- Networking / multiplayer.
- Save/load, settings menus, or other meta features.

## Success Criteria

1. Fresh clone builds and runs cross-platform with two CMake commands.
2. A window opens showing placeholder cards; clicking a card flips it with an
   animation.
3. `clang-format`, `clang-tidy`, pre-commit, and CI are all configured and pass.
4. CTest runs the example test successfully.
5. A C++-literate beginner can read the README map + file headers and correctly
   identify where to add a card, a rule, a visual change, and the NPC.
