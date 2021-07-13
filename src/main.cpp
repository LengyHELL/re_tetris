#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "engine/TextButton.hpp"
#include "engine/ImageButton.hpp"
#include "engine/Frame.hpp"
#include "engine/Picker.hpp"
#include "engine/TextBox.hpp"
#include "engine/InputBox.hpp"

#include "Block.hpp"
#include "Board.hpp"

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
  Board board;
  Block block('t');
  bool left_lock = true;
  bool right_lock = true;
  bool down_lock = true;
  bool up_lock = true;
  // End Test

  while(!engine.get_exit()) {
    engine.update_inputs();

    board.draw(engine);
    board.draw_block(engine, block);

    if (engine.keyboard_state[SDL_SCANCODE_LEFT] && !left_lock) {
      left_lock = true;
      block.move(Coord(-1, 0));
    }
    else if (!engine.keyboard_state[SDL_SCANCODE_LEFT]) { left_lock = false; }

    if (engine.keyboard_state[SDL_SCANCODE_RIGHT] && !right_lock) {
      right_lock = true;
      block.move(Coord(1, 0));
    }
    else if (!engine.keyboard_state[SDL_SCANCODE_RIGHT]) { right_lock = false; }

    if (engine.keyboard_state[SDL_SCANCODE_DOWN] && !down_lock) {
      down_lock = true;
      block.move(Coord(0, 1));
    }
    else if (!engine.keyboard_state[SDL_SCANCODE_DOWN]) { down_lock = false; }

    if (engine.keyboard_state[SDL_SCANCODE_UP] && !up_lock) {
      up_lock = true;
      block.rotate();
    }
    else if (!engine.keyboard_state[SDL_SCANCODE_UP]) { up_lock = false; }

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
