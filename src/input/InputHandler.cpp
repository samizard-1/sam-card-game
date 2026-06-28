#include "input/InputHandler.hpp"

namespace input {

bool InputHandler::clickedIn(Rectangle bounds) const {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(), bounds);
}

bool InputHandler::hovering(Rectangle bounds) const {
    return CheckCollisionPointRec(GetMousePosition(), bounds);
}

} // namespace input
