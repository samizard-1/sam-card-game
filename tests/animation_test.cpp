#include <doctest/doctest.h>

#include "render/Animation.hpp"

TEST_CASE("a flip starts at full width, before the midpoint, and active") {
    render::FlipAnimation flip;
    flip.start(1.0f, 2.0f);
    CHECK(flip.horizontalScale() == doctest::Approx(1.0f));
    CHECK_FALSE(flip.pastMidpoint());
    CHECK(flip.active());
}

TEST_CASE("a flip is squashed to zero width at the midpoint") {
    render::FlipAnimation flip;
    flip.start(1.0f, 2.0f);
    flip.update(0.5f);
    CHECK(flip.horizontalScale() == doctest::Approx(0.0f));
    CHECK(flip.pastMidpoint());
}

TEST_CASE("a flip returns to full width and finishes at the end") {
    render::FlipAnimation flip;
    flip.start(1.0f, 2.0f);
    flip.update(2.0f);
    CHECK(flip.horizontalScale() == doctest::Approx(1.0f));
    CHECK_FALSE(flip.active());
}
