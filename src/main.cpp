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
#include <time.h>

int main(int argc, char** argv) {
  // Initializing engine
  srand(time(NULL));

  Engine engine(400, 500, "Re:Tetris", false);
  engine.set_fps_cap(120);
  engine.load_font("lhll.ttf");
  engine.load_image("img/no_part.png");
  engine.load_image("img/blank.png");
  engine.load_image("img/shadow.png");

  engine.load_image("img/basic_style.png");

  // Creating menu elements
  Rect yn_rect(engine.get_width() / 2 - (150 / 2), 150, 150, 75);
  Frame yesno("img/basic_style.png", yn_rect);
  TextButton yes("img/basic_style.png", Rect(yn_rect.x + 10, yn_rect.y + yn_rect.h - (25 + 10), 40, 25), "Yes", 16, {0, 0, 0, 0});
  TextButton no("img/basic_style.png", Rect(yn_rect.x + yn_rect.w - (40 + 10), yn_rect.y + yn_rect.h - (25 + 10), 40, 25), "No", 16, {0, 0, 0, 0});
  TextButton start("img/basic_style.png", Rect(((engine.get_width() / 4) * 3) - 25, (engine.get_width() / 10) * 9, 50, 25), "Start", 16, {0, 0, 0, 0});
  bool exit = false;

  // Creating game objects
  //...

  // Test
  //...
  Game game(engine);
  // End Test

  while(!engine.get_exit() && !exit) {
    engine.update_inputs();

    if (start.selected) {
      game.update(engine);
    }
    game.draw(engine);

    if (!start.selected) {
      start.draw(engine);
      start.update(engine);
    }

    if (game.is_over()) {
      yesno.draw(engine);
      yes.draw(engine);
      no.draw(engine);
      engine.draw_text("Restart?", Coord(yn_rect.x + 45, yn_rect.y + 10), {0, 0, 0, 0}, 16);

      yesno.update(engine);
      yes.update(engine);
      no.update(engine);

      if (yes.selected) {
        game = Game(engine);
      }
      else if (no.selected) {
        exit = true;
      }
    }

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {64, 64, 64, 0}, 16);
    engine.render();
  }
  return 0;
}
