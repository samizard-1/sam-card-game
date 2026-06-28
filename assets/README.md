# Assets

Put card art, fonts, and sounds here.

Right now the game draws cards as plain rounded rectangles (see
`src/render/CardRenderer.cpp`) and needs no files in this folder. When you are
ready for custom card looks:

1. Drop image files (e.g. `card_back.png`, `dragon.png`) in this folder.
2. Load them in `CardRenderer` with raylib's `LoadTexture(...)`.
3. Draw them with `DrawTexturePro(...)` instead of `DrawRectangleRounded(...)`.

Tip: load each texture once (not every frame) and unload it when the game exits.
