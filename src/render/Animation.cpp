#include "render/Animation.hpp"

#include <algorithm>
#include <cmath>

namespace render {

void FlipAnimation::start(float durationSeconds) {
    active_ = true;
    elapsed_ = 0.0f;
    duration_ = durationSeconds;
}

void FlipAnimation::update(float dt) {
    if (!active_) {
        return;
    }
    elapsed_ += dt;
    if (elapsed_ >= duration_) {
        elapsed_ = duration_;
        active_ = false;
    }
}

bool FlipAnimation::active() const { return active_; }

float FlipAnimation::progress() const {
    if (duration_ <= 0.0f) {
        return 1.0f;
    }
    return std::clamp(elapsed_ / duration_, 0.0f, 1.0f);
}

float FlipAnimation::horizontalScale() const {
    // 1 at the start, 0 at the midpoint, 1 at the end.
    return std::fabs(1.0f - 2.0f * progress());
}

bool FlipAnimation::pastMidpoint() const { return progress() >= 0.5f; }

} // namespace render
