// Game implementation. This is the spine of the application: it owns the
// window, drives the main loop, and wires together cards, rules, and rendering.
#include "app/Game.hpp"
#include "cards/Deck.hpp"
#include "Config.hpp"

#include "raylib.h"
#include <ctime>

namespace app {

namespace {
constexpr float kFlipDuration = 0.3f;     // seconds for one flip
constexpr float kFlipBackDuration = 2.0f; // seconds for one flip back
} // namespace

Game::Game(int width, int height, const char *title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    deck_.shuffle(static_cast<unsigned int>(time(nullptr))); // shuffle the deck with a random seed
}

Game::~Game() { CloseWindow(); }

Rectangle Game::cardBounds() const {
    return Rectangle{(static_cast<float>(GetScreenWidth()) - Config::kCardWidth) / 2.0f,
                     (static_cast<float>(GetScreenHeight()) - Config::kCardHeight) / 2.0f,
                     Config::kCardWidth, Config::kCardHeight};
}

void Game::run() {
    while (!WindowShouldClose()) {
        switch (menuToDisplay_) {
        case MenuChoice::MAIN_MENU:
            updateMain(GetFrameTime());
            break;
        case MenuChoice::INVENTORY:
            updateInventory(GetFrameTime());
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch (menuToDisplay_) {
        case MenuChoice::MAIN_MENU:
            drawMain();
            break;
        case MenuChoice::INVENTORY:
            drawInventory();
            break;
        }
        EndDrawing();
    }
}

void Game::updateMain(float dt) {

    // only flip cards if there are any left in the deck
    if (!Game::deck_.empty()) {
        // Click the card to start a flip (ignored while one is already running).
        if (!flip_.flipped() && input_.clickedIn(cardBounds())) {
            flip_.start(kFlipDuration, kFlipBackDuration);
            swappedThisFlip_ = false;
            if (!deck_.empty()) {
                this->card = deck_.draw(); // draw a card from the deck when the card is clicked
            } else {
                DrawText("No More Cards in the Deck", cardBounds().x, cardBounds().y, 20, RED);
            }
            // add the card to the inventory if the card has not already
            // been added to the inventory
            if (!this->card.addedToInventory) {
                inventories_[this->card.inventorySlot()].add(this->card);
                this->card.addedToInventory = true;
            }
        }
    }

    flip_.update(dt);

    // Swap which side is visible exactly once, at the midpoint of the flip.
    if (flip_.active() && flip_.pastMidpoint() && !swappedThisFlip_) {
        showFace_ = !showFace_;
        swappedThisFlip_ = true;
    }

    // flip back after flipDuration is over
    if (!flip_.flipped()) {
        showFace_ = false;
    }

    updateInventoryClickState();
}

void Game::updateInventory(float dt) {
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, inventoryExitRec_) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        menuToDisplay_ = MenuChoice::MAIN_MENU; // switch back to main menu when exit is clicked

        // set all inventory buttons to not clicked
        AButton_.clicked = false;
        BButton_.clicked = false;
        CButton_.clicked = false;
        DButton_.clicked = false;
        EButton_.clicked = false;
        FButton_.clicked = false;
        GButton_.clicked = false;
        HButton_.clicked = false;
        IButton_.clicked = false;
        JButton_.clicked = false;
        KButton_.clicked = false;
        LButton_.clicked = false;
        MButton_.clicked = false;
        NButton_.clicked = false;
        OButton_.clicked = false;
        PButton_.clicked = false;
        QButton_.clicked = false;
        RButton_.clicked = false;
        SButton_.clicked = false;
        TButton_.clicked = false;
        UButton_.clicked = false;
        VButton_.clicked = false;
        WButton_.clicked = false;
        XButton_.clicked = false;
        YButton_.clicked = false;
        ZButton_.clicked = false;
    }

    // TODO: add what happens when a user clicks on a card in the inventory.
}

void Game::drawMain() const {
    DrawText("Click the card to flip it.", 20, 20, 20, DARKGRAY);
    const float scale = flip_.active() ? flip_.horizontalScale() : 1.0f;
    if (!(Game::deck_.empty() & (Game::showFace_ == false))) {
        cardRenderer_.draw(this->card, Game::cardBounds(), scale, showFace_);
    }

    DrawRectangleRec(AButton_.bounds, YELLOW);
    DrawText("A", 10, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(BButton_.bounds, MAROON);
    DrawText("B", 40, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(CButton_.bounds, DARKGRAY);
    DrawText("C", 70, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(DButton_.bounds, RED);
    DrawText("D", 100, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(EButton_.bounds, BLUE);
    DrawText("E", 130, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(FButton_.bounds, PURPLE);
    DrawText("F", 160, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(GButton_.bounds, ORANGE);
    DrawText("G", 190, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(HButton_.bounds, PINK);
    DrawText("H", 220, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(IButton_.bounds, BLACK);
    DrawText("I", 250, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(JButton_.bounds, SKYBLUE);
    DrawText("J", 280, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(KButton_.bounds, LIME);
    DrawText("K", 310, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(LButton_.bounds, BROWN);
    DrawText("L", 340, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(MButton_.bounds, DARKGRAY);
    DrawText("M", 370, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(NButton_.bounds, VIOLET);
    DrawText("N", 400, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(OButton_.bounds, MAROON);
    DrawText("O", 430, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(PButton_.bounds, GOLD);
    DrawText("P", 460, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(QButton_.bounds, DARKBLUE);
    DrawText("Q", 490, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(RButton_.bounds, DARKGREEN);
    DrawText("R", 520, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(SButton_.bounds, DARKPURPLE);
    DrawText("S", 550, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(TButton_.bounds, DARKBROWN);
    DrawText("T", 580, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(UButton_.bounds, BEIGE);
    DrawText("U", 610, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(VButton_.bounds, LIGHTGRAY);
    DrawText("V", 640, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(WButton_.bounds, DARKGRAY);
    DrawText("W", 670, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(XButton_.bounds, GREEN);
    DrawText("X", 700, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(YButton_.bounds, YELLOW);
    DrawText("Y", 730, GetScreenHeight() - 40, 20, GRAY);

    DrawRectangleRec(ZButton_.bounds, LIGHTGRAY);
    DrawText("Z", 760, GetScreenHeight() - 40, 20, GRAY);
}

void Game::drawInventory() const {
    DrawRectangleRec(inventoryExitRec_, LIGHTGRAY);
    DrawText("EXIT", 15, 10, 40, RED);

    Rectangle screenBounds = {0, 0, static_cast<float>(GetScreenWidth()),
                              static_cast<float>(GetScreenHeight())};
    if (AButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("A"), screenBounds);
    };

    if (BButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("B"), screenBounds);
    };

    if (CButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("C"), screenBounds);
    };

    if (DButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("D"), screenBounds);
    };

    if (EButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("E"), screenBounds);
    };

    if (FButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("F"), screenBounds);
    };

    if (GButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("G"), screenBounds);
    };

    if (HButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("H"), screenBounds);
    };

    if (IButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("I"), screenBounds);
    };

    if (JButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("J"), screenBounds);
    };

    if (KButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("K"), screenBounds);
    };

    if (LButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("L"), screenBounds);
    };

    if (MButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("M"), screenBounds);
    };

    if (NButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("N"), screenBounds);
    };

    if (OButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("O"), screenBounds);
    };

    if (PButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("P"), screenBounds);
    };

    if (QButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("Q"), screenBounds);
    };

    if (RButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("R"), screenBounds);
    };

    if (SButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("S"), screenBounds);
    };

    if (TButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("T"), screenBounds);
    };

    if (UButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("U"), screenBounds);
    };

    if (VButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("V"), screenBounds);
    };

    if (WButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("W"), screenBounds);
    };

    if (XButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("X"), screenBounds);
    };

    if (YButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("Y"), screenBounds);
    };

    if (ZButton_.clicked == true) {
        inventoryRenderer_.draw(inventories_.at("Z"), screenBounds);
    };
}

void Game::updateInventoryClickState() {

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, AButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on A!", 10, GetScreenHeight() - 70, 20, GRAY);
        AButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when A is clicked
    }
    if (CheckCollisionPointRec(mousePos, BButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on B!", 40, GetScreenHeight() - 70, 20, GRAY);
        BButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when B is clicked
    }
    if (CheckCollisionPointRec(mousePos, CButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on C!", 70, GetScreenHeight() - 70, 20, GRAY);
        CButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when C is clicked
    }
    if (CheckCollisionPointRec(mousePos, DButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on D!", 100, GetScreenHeight() - 70, 20, GRAY);
        DButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when D is clicked
    }
    if (CheckCollisionPointRec(mousePos, EButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on E!", 130, GetScreenHeight() - 70, 20, GRAY);
        EButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when E is clicked
    }
    if (CheckCollisionPointRec(mousePos, FButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on F!", 160, GetScreenHeight() - 70, 20, GRAY);
        FButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when F is clicked
    }
    if (CheckCollisionPointRec(mousePos, GButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on G!", 190, GetScreenHeight() - 70, 20, GRAY);
        GButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when G is clicked
    }
    if (CheckCollisionPointRec(mousePos, HButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on H!", 220, GetScreenHeight() - 70, 20, GRAY);
        HButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when H is clicked
    }
    if (CheckCollisionPointRec(mousePos, IButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on I!", 250, GetScreenHeight() - 70, 20, GRAY);
        IButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when I is clicked
    }
    if (CheckCollisionPointRec(mousePos, JButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on J!", 280, GetScreenHeight() - 70, 20, GRAY);
        JButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when J is clicked
    }
    if (CheckCollisionPointRec(mousePos, KButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on K!", 310, GetScreenHeight() - 70, 20, GRAY);
        KButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when K is clicked
    }
    if (CheckCollisionPointRec(mousePos, LButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on L!", 340, GetScreenHeight() - 70, 20, GRAY);
        LButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when L is clicked
    }
    if (CheckCollisionPointRec(mousePos, MButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on M!", 370, GetScreenHeight() - 70, 20, GRAY);
        MButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when M is clicked
    }
    if (CheckCollisionPointRec(mousePos, NButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on N!", 400, GetScreenHeight() - 70, 20, GRAY);
        NButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when N is clicked
    }
    if (CheckCollisionPointRec(mousePos, OButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on O!", 430, GetScreenHeight() - 70, 20, GRAY);
        OButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when O is clicked
    }
    if (CheckCollisionPointRec(mousePos, PButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on P!", 460, GetScreenHeight() - 70, 20, GRAY);
        PButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when P is clicked
    }
    if (CheckCollisionPointRec(mousePos, QButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on Q!", 490, GetScreenHeight() - 70, 20, GRAY);
        QButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when Q is clicked
    }
    if (CheckCollisionPointRec(mousePos, RButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on R!", 520, GetScreenHeight() - 70, 20, GRAY);
        RButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when R is clicked
    }
    if (CheckCollisionPointRec(mousePos, SButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on S!", 550, GetScreenHeight() - 70, 20, GRAY);
        SButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when S is clicked
    }
    if (CheckCollisionPointRec(mousePos, TButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on T!", 580, GetScreenHeight() - 70, 20, GRAY);
        TButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when T is clicked
    }
    if (CheckCollisionPointRec(mousePos, UButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on U!", 610, GetScreenHeight() - 70, 20, GRAY);
        UButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when U is clicked
    }
    if (CheckCollisionPointRec(mousePos, VButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on V!", 640, GetScreenHeight() - 70, 20, GRAY);
        VButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when V is clicked
    }
    if (CheckCollisionPointRec(mousePos, WButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on W!", 670, GetScreenHeight() - 70, 20, GRAY);
        WButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when W is clicked
    }
    if (CheckCollisionPointRec(mousePos, XButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on X!", 700, GetScreenHeight() - 70, 20, GRAY);
        XButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when X is clicked
    }
    if (CheckCollisionPointRec(mousePos, YButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on Y!", 730, GetScreenHeight() - 70, 20, GRAY);
        YButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when Y is clicked
    }
    if (CheckCollisionPointRec(mousePos, ZButton_.bounds) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        DrawText("You clicked on Z!", 760, GetScreenHeight() - 70, 20, GRAY);
        ZButton_.clicked = true;
        menuToDisplay_ = MenuChoice::INVENTORY; // switch to inventory menu when Z is clicked
    }
}
} // namespace app
