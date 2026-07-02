// CardRenderer implementation. This is the single place that controls how a
// card looks on screen: colors, shapes, and eventually custom art.
#include "render/CardRenderer.hpp"
#include "Config.hpp"

#include <string>

namespace render {

void CardRenderer::draw(const cards::Card &card, Rectangle bounds, float horizontalScale,
                        bool showFace) const {
    // Squash horizontally around the card's center to fake a 3D flip.
    const float squashedWidth = Config::kCardWidth * horizontalScale;
    Rectangle drawn = bounds;
    drawn.x = bounds.x + (Config::kCardWidth - squashedWidth) / 2.0f;
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
        const int textX =
            static_cast<int>(drawn.x + (drawn.width - static_cast<float>(textWidth)) / 2.0f);
        const int textY = static_cast<int>(drawn.y + drawn.height / 2.0f - fontSize / 2.0f);
        DrawText(label.c_str(), textX, textY, fontSize, DARKGRAY);
    }
}

} // namespace render
