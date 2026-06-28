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
    void draw(const cards::Card &card, Rectangle bounds, float horizontalScale,
              bool showFace) const;
};

} // namespace render
