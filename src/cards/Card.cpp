#include "cards/Card.hpp"

namespace cards {

std::string Card::label() const {
    if (!name.empty()) {
        return name;
    }
    return "Card #" + std::to_string(id);
}

} // namespace cards
