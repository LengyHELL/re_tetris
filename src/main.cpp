#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "engine/TextButton.hpp"
#include "engine/ImageButton.hpp"
#include "engine/Frame.hpp"
#include "engine/Picker.hpp"
#include "engine/TextBox.hpp"
#include "engine/InputBox.hpp"

#include "Game.hpp"

int main(int argc, char** argv) {
  // Initializing engine
  srand(time(NULL));

  Engine engine(640, 480, "Re:Tetris", false);
  engine.load_font("lhll.ttf");
  engine.load_image("img/no_part.png");

  engine.load_image("img/basic_style.png");

  // Creating menu elements
  Frame yesno("img/basic_style.png", Rect(150, 75, 150, 75));
  TextButton yes("img/basic_style.png", Rect(160, 115, 40, 25), "Yes", 16, {0, 0, 0, 0});
  TextButton no("img/basic_style.png", Rect(250, 115, 40, 25), "No", 16, {0, 0, 0, 0});
  bool exit = false;

  std::string info_text = "Left/Right - Left and Right arrows\nBoost - Down arrow\nRotate - Up arrow";
  TextBox info("img/basic_style.png", Rect(300, 150, 300, 300), info_text, 16, {0, 0, 0, 0});

  // Creating game objects
  //...

  // Test
  //...
  Game game;
  // End Test

  while(!engine.get_exit() && !exit) {
    engine.update_inputs();

    game.update(engine);
    game.draw(engine);
    info.draw(engine);

    if (game.is_over()) {
      yesno.draw(engine);
      yes.draw(engine);
      no.draw(engine);
      engine.draw_text("Restart?", Coord(180, 85), {0, 0, 0, 0}, 16);

      yesno.update(engine);
      yes.update(engine);
      no.update(engine);

      if (yes.selected) {
        game = Game();
      }
      else if (no.selected) {
        exit = true;
      }
    }

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
