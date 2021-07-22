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

class GameHud {
  Frame top;
  Frame bottom;
  Frame middle;
  std::string info_text = "Left/Right - Left and Right arrows\nBoost - Down arrow\nRotate - Up arrow";
  TextBox info;

public:
  GameHud(const Engine& engine) {
    top = Frame("img/basic_style.png", Rect(0, 0, engine.get_width(), engine.get_height() / 10));
    bottom = Frame("img/basic_style.png", Rect(0, engine.get_height() - (engine.get_height() / 10), engine.get_width(), engine.get_height() / 10));
    middle = Frame("img/basic_style.png", Rect(engine.get_width() / 2, engine.get_height() / 10, engine.get_width() / 2, engine.get_height() - (engine.get_height() / 5)));
    info = TextBox("img/basic_style.png", Rect(210, 250, 180, 100), info_text, 16, {0, 0, 0, 0});
  }

  void draw(const Engine& engine, Game& game) {
    engine.draw_image("img/blank.png", Rect(0, 0, engine.get_width(), engine.get_height()));
    bottom.draw(engine);
    middle.draw(engine);
    game.draw(engine);
    top.draw(engine);
    info.draw(engine);
    engine.draw_text("Score: " + std::to_string(game.get_score()), Coord(210, 60), {0, 0, 0, 0}, 16);
    engine.draw_text("Level: " + std::to_string(game.get_level()), Coord(210, 80), {0, 0, 0, 0}, 16);
  }
};

int main(int argc, char** argv) {
  // Initializing engine
  srand(time(NULL));

  Engine engine(400, 500, "Re:Tetris", false);
  engine.set_fps_cap(120);
  engine.load_font("lhll.ttf");
  engine.load_image("img/no_part.png");
  engine.load_image("img/blank.png");

  engine.load_image("img/basic_style.png");

  // Creating menu elements
  Rect yn_rect(engine.get_width() / 2 - (150 / 2), 150, 150, 75);
  Frame yesno("img/basic_style.png", yn_rect);
  TextButton yes("img/basic_style.png", Rect(yn_rect.x + 10, yn_rect.y + yn_rect.h - (25 + 10), 40, 25), "Yes", 16, {0, 0, 0, 0});
  TextButton no("img/basic_style.png", Rect(yn_rect.x + yn_rect.w - (40 + 10), yn_rect.y + yn_rect.h - (25 + 10), 40, 25), "No", 16, {0, 0, 0, 0});
  bool exit = false;

  // Creating game objects
  //...

  // Test
  //...
  Game game(engine);
  GameHud hud(engine);
  // End Test

  while(!engine.get_exit() && !exit) {
    engine.update_inputs();

    game.update(engine);
    hud.draw(engine, game);

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

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
