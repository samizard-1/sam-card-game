#include "cards/Card.hpp"

namespace cards {

std::string Card::label() const {
    if (!name.empty()) {
        return name;
    }
    return "Card #" + std::to_string(id);
}

std::string Card::inventorySlot() const {
    // Assuming the inventory slot is determined by the first character of the card's name
    if (!name.empty()) {
        return std::string(1, std::toupper(static_cast<unsigned char>(name[0])));
    }
    return "";
}

} // namespace cards
