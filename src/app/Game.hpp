#pragma once

#include "raylib.h"

#include "cards/Deck.hpp"
#include "cards/Card.hpp"
#include "cards/Inventory.hpp"
#include "input/InputHandler.hpp"
#include "render/Animation.hpp"
#include "render/CardRenderer.hpp"
#include "render/InventoryRenderer.hpp"

#include <unordered_map>

namespace app {

enum class MenuChoice { MAIN_MENU, INVENTORY };

struct InventoryButton {
    Rectangle bounds;
    std::string inventoryName;
    bool clicked = false;
};

// Owns the application window and the main game loop. This class is the spine of
// the program: every frame it updates state, then draws the scene.
//
// TODO(you): grow this into the real game — replace the single demo card with a
// deck, a GameState, and players, and call into rules/ each turn.
class Game {
  public:
    Game(int width, int height, const char *title);
    ~Game();
    // Runs the main loop until the user closes the window.
    void run();

    // the currently active card in the game, drawn from the deck when the card is clicked
    cards::Card card;

  private:
    void updateMain(float dt);        // advance state by dt seconds
    void updateInventory(float dt);   // advance state by dt seconds
    void drawMain() const;            // render the current frame of the main window
    void drawInventory() const;       // render the current frame of the inventory window
    void updateInventoryClickState(); // handle the click state of the inventory slots
    Rectangle cardBounds() const;     // where the demo card sits on screen
    cards::Deck deck_ = cards::Deck::buildStandardDeck(); // the deck of cards for the game
    bool showFace_ = false;                               // is the face (vs back) currently up?
    bool swappedThisFlip_ = false;                        // have we swapped sides during this flip?
    render::FlipAnimation flip_;
    render::CardRenderer cardRenderer_;
    render::InventoryRenderer inventoryRenderer_;
    input::InputHandler input_;

    // intialize all inventories
    std::unordered_map<std::string, cards::Inventory> inventories_{
        {"A", cards::Inventory{}}, {"B", cards::Inventory{}}, {"C", cards::Inventory{}},
        {"D", cards::Inventory{}}, {"E", cards::Inventory{}}, {"F", cards::Inventory{}},
        {"G", cards::Inventory{}}, {"H", cards::Inventory{}}, {"I", cards::Inventory{}},
        {"J", cards::Inventory{}}, {"K", cards::Inventory{}}, {"L", cards::Inventory{}},
        {"M", cards::Inventory{}}, {"N", cards::Inventory{}}, {"O", cards::Inventory{}},
        {"P", cards::Inventory{}}, {"Q", cards::Inventory{}}, {"R", cards::Inventory{}},
        {"S", cards::Inventory{}}, {"T", cards::Inventory{}}, {"U", cards::Inventory{}},
        {"V", cards::Inventory{}}, {"W", cards::Inventory{}}, {"X", cards::Inventory{}},
        {"Y", cards::Inventory{}}, {"Z", cards::Inventory{}}};
    MenuChoice menuToDisplay_ = MenuChoice::MAIN_MENU; // the current menu being displayed

    // inventory buttons
    InventoryButton AButton_{{0, 550, 30, 50}, "A"};
    InventoryButton BButton_{{30, 550, 30, 50}, "B"};
    InventoryButton CButton_{{60, 550, 30, 50}, "C"};
    InventoryButton DButton_{{90, 550, 30, 50}, "D"};
    InventoryButton EButton_{{120, 550, 30, 50}, "E"};
    InventoryButton FButton_{{150, 550, 30, 50}, "F"};
    InventoryButton GButton_{{180, 550, 30, 50}, "G"};
    InventoryButton HButton_{{210, 550, 30, 50}, "H"};
    InventoryButton IButton_{{240, 550, 30, 50}, "I"};
    InventoryButton JButton_{{270, 550, 30, 50}, "J"};
    InventoryButton KButton_{{300, 550, 30, 50}, "K"};
    InventoryButton LButton_{{330, 550, 30, 50}, "L"};
    InventoryButton MButton_{{360, 550, 30, 50}, "M"};
    InventoryButton NButton_{{390, 550, 30, 50}, "N"};
    InventoryButton OButton_{{420, 550, 30, 50}, "O"};
    InventoryButton PButton_{{450, 550, 30, 50}, "P"};
    InventoryButton QButton_{{480, 550, 30, 50}, "Q"};
    InventoryButton RButton_{{510, 550, 30, 50}, "R"};
    InventoryButton SButton_{{540, 550, 30, 50}, "S"};
    InventoryButton TButton_{{570, 550, 30, 50}, "T"};
    InventoryButton UButton_{{600, 550, 30, 50}, "U"};
    InventoryButton VButton_{{630, 550, 30, 50}, "V"};
    InventoryButton WButton_{{660, 550, 30, 50}, "W"};
    InventoryButton XButton_{{690, 550, 30, 50}, "X"};
    InventoryButton YButton_{{720, 550, 30, 50}, "Y"};
    InventoryButton ZButton_{{750, 550, 30, 50}, "Z"};

    Rectangle inventoryExitRec_ = {0, 0, 170, 50};
};

} // namespace app
