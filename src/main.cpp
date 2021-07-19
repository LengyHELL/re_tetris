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
  //...

  // Creating game objects
  //...

  // Test
  //...
  Game game;
  // End Test

  while(!engine.get_exit()) {
    engine.update_inputs();

    game.update(engine);
    game.draw(engine);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
