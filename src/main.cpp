#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "engine/TextButton.hpp"
#include "engine/ImageButton.hpp"
#include "engine/Frame.hpp"
#include "engine/Picker.hpp"
#include "engine/TextBox.hpp"
#include "engine/InputBox.hpp"

int main(int argc, char** argv) {
  // Initializing engine
  srand(time(NULL));

  Engine engine(640, 480, "csuriz.exxe", false);
  engine.load_font("lhll.ttf");
  engine.load_image("img/bottom_tube.png");
  engine.load_image("img/top_tube.png");
  engine.load_image("img/background.png");
  engine.load_image("img/bird.png");

  engine.load_image("img/basic_style.png");

  // Creating menu elements
  //...

  // Creating game objects
  //...

  while(!engine.get_exit()) {
    engine.update_inputs();

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
