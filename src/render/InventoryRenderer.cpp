// InventoryRenderer implementation. This is the single place that controls how an
// inventory looks on screen: colors, shapes, and eventually custom art.
#include "render/InventoryRenderer.hpp"
#include "render/CardRenderer.hpp"
#include "cards/Card.hpp"
#include "Config.hpp"

#include "raylib.h"

namespace render {

void InventoryRenderer::draw(const cards::Inventory &inventory, const Rectangle &bounds) const {
    // Implementation for drawing the entire inventory

    // For now, we will draw each card in the inventory in a grid layout
    const int topSpace = 50;    // Space from the top of the screen
    const int slotPadding = 15; // Padding between cards
    const int cardWidth = static_cast<int>(Config::kCardWidth);
    const int cardHeight = static_cast<int>(Config::kCardHeight);
    const int cardsPerRow = static_cast<int>(bounds.width) / (cardWidth + slotPadding);
    const int cardsPerColumn =
        static_cast<int>((bounds.height - topSpace) / (cardHeight + slotPadding));

    if (inventory.empty()) {
        DrawText("Inventory is empty", static_cast<int>(bounds.x + bounds.width / 2 - 100),
                 static_cast<int>(bounds.y + bounds.height / 2 - 10), 20, DARKGRAY);
        return;
    }

    render::CardRenderer cardRenderer;
    for (size_t i = 0; i < inventory.size(); ++i) {
        const cards::Card &card = inventory.cards()[i];
        int row = static_cast<int>(i) / cardsPerRow;    // cardsPerRow cards per row
        int col = static_cast<int>(i) % cardsPerColumn; // cardsPerColumn cards per column
        Rectangle slot = {bounds.x + static_cast<float>(col) * (cardWidth + slotPadding),
                          bounds.y + static_cast<float>(topSpace) +
                              static_cast<float>(row) * (cardHeight + slotPadding),
                          static_cast<float>(cardWidth), static_cast<float>(cardHeight)};
        cardRenderer.draw(card, slot, 1.0f, true);
    }
}

} // namespace render
