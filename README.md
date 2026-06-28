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
