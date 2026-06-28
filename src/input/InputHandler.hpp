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

} // namespace input
