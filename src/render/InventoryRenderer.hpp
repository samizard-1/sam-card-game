#pragma once

#include "raylib.h"

#include "cards/Inventory.hpp"

namespace render {

// Draws an entire inventory.
class InventoryRenderer {
  public:
    // Draws the entire inventory.
    void draw(const cards::Inventory &inventory, const Rectangle &bounds) const;

  private:
    Rectangle slotOne() const;
    Rectangle slotTwo() const;
};

} // namespace render
