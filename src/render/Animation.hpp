#pragma once

namespace render {

// Drives a card-flip animation. Progress runs 0 -> 1 over `duration` seconds.
// The horizontal scale shrinks from 1 to 0 at the midpoint and back to 1, which
// reads as a card flipping. Swap the visible side at the midpoint.
//
// TODO(you): add more animation types as your game needs them (deal, discard,
// hover-lift, ...). Keep them as small, self-contained helpers like this one.
class FlipAnimation {
  public:
    // Starts a flip lasting `durationSeconds`.
    void start(float durationSeconds, float flipDuration);

    // Advances by `dt` seconds. A no-op when no flip is running.
    void update(float dt);

    bool active() const;

    bool flipped() const;

    // Normalized progress in [0, 1].
    float progress() const;

    // Horizontal scale factor in [0, 1] for the current progress.
    float horizontalScale() const;

    // True once progress has reached/passed the midpoint (swap face/back here).
    bool pastMidpoint() const;

  private:
    bool active_ = false;
    float elapsed_ = 0.0f;
    float animationDuration_ = 0.0f;
    float flipTimer_ = 0.0f;    // timer for flip animation
    float flipDuration_ = 0.0f; // time to let the card remain flipped before flipping back
    bool flipped_ = false;      // whether the card has been flipped or not
};

} // namespace render
