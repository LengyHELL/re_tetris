#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "engine/TextButton.hpp"
#include "engine/ImageButton.hpp"
#include "engine/Frame.hpp"
#include "engine/Picker.hpp"
#include "engine/TextBox.hpp"
#include "engine/InputBox.hpp"

struct ActiveBlock {
  Coord pos = Coord(0, 0);
  int matrix[4][4] = {{0, 0, 0, 0},
                      {0, 0, 0, 0},
                      {0, 0, 0, 0},
                      {0, 0, 0, 0}};
  int rotations = 0;
  int size = 0;

  ActiveBlock(char type) {
    switch(type) { //o i j l t s z
      case 'o':
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][1] = 1;
        matrix[2][2] = 1;
        rotations = 0;
        size = 4;
      break;
      case 'i':
        matrix[2][0] = 1;
        matrix[2][1] = 1;
        matrix[2][2] = 1;
        matrix[2][3] = 1;
        rotations = 2;
        size = 4;
      break;
      case 'j':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][2] = 1;
        rotations = 4;
        size = 3;
      break;
      case 'l':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][0] = 1;
        rotations = 4;
        size = 3;
      break;
      case 't':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][1] = 1;
        rotations = 4;
        size = 3;
      break;
      case 's':
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][0] = 1;
        matrix[2][1] = 1;
        rotations = 2;
        size = 3;
      break;
      case 'z':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[2][1] = 1;
        matrix[2][2] = 1;
        rotations = 2;
        size = 3;
      break;
    }
  }

  void write() {
    for(int i = 0; i < size; i++) {
      for(int j = 0; j < size; j++) {
        std::cout << matrix[i][j] << " ";
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }

};

struct PassiveBlock {

};

class Board { //10x20

};

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

  // Test
  ActiveBlock o('o');
  ActiveBlock i('i');
  ActiveBlock j('j');
  ActiveBlock l('l');
  ActiveBlock t('t');
  ActiveBlock s('s');
  ActiveBlock z('z');
  o.write();
  i.write();
  j.write();
  l.write();
  t.write();
  s.write();
  z.write();
  // End Test

  while(!engine.get_exit()) {
    engine.update_inputs();

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
