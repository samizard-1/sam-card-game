#include "render/Animation.hpp"

#include <algorithm>
#include <cmath>

namespace render {

void FlipAnimation::start(float animationDuration, float flipDuration) {
    active_ = true;
    flipped_ = true;
    elapsed_ = 0.0f;
    animationDuration_ = animationDuration;
    flipTimer_ = 0;
    flipDuration_ = flipDuration;
    // randomly choose the card that will be flipped from all available cards in the deck
    // chosenCard = deck.choose()
}

void FlipAnimation::update(float dt) {
    flipTimer_ += dt;
    if (flipTimer_ >= flipDuration_) {
        flipTimer_ = 0;
        flipped_ = false;
    }

    if (!active_) {
        return;
    }
    elapsed_ += dt;
    if (elapsed_ >= animationDuration_) {
        elapsed_ = animationDuration_;
        active_ = false;
    }
}

bool FlipAnimation::active() const { return active_; }

bool FlipAnimation::flipped() const { return flipped_; }

float FlipAnimation::progress() const {
    if (animationDuration_ <= 0.0f) {
        return 1.0f;
    }
    return std::clamp(elapsed_ / animationDuration_, 0.0f, 1.0f);
}

float FlipAnimation::horizontalScale() const {
    // 1 at the start, 0 at the midpoint, 1 at the end.
    return std::fabs(1.0f - 2.0f * progress());
}

bool FlipAnimation::pastMidpoint() const { return progress() >= 0.5f; }

} // namespace render
