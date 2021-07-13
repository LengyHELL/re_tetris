#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "engine/TextButton.hpp"
#include "engine/ImageButton.hpp"
#include "engine/Frame.hpp"
#include "engine/Picker.hpp"
#include "engine/TextBox.hpp"
#include "engine/InputBox.hpp"

struct Block {
  Coord pos = Coord(0, 3);
  int rotation = 0;
  int matrix[4][4] = {{0, 0, 0, 0},
                      {0, 0, 0, 0},
                      {0, 0, 0, 0},
                      {0, 0, 0, 0}};
  int rotations = 0;
  int size = 0;
  SDL_Color color = {128, 0, 0, 255};

  Block(const char& type) {
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

  void rotate_matrix() {
    for (int i = 0; i < size / 2; i++) {
      for (int j = i; j < size - i - 1; j++) {
        int k = size - i - 1;
        int l = size - j - 1;
        int temp = matrix[i][j];
        matrix[i][j] = matrix[l][i];
        matrix[l][i] = matrix[k][l];
        matrix[k][l] = matrix[j][k];
        matrix[j][k] = temp;
      }
    }
  }

  void rotate() {
    if (rotations == 4) {
      rotation += 1;
      if (rotation == 4) { rotation = 0; }
      rotate_matrix();
    }
    else if (rotations == 2) {
      if (rotation == 0) {
        rotation = 1;
        for (int i = 0; i < 3; i++) {
          rotate_matrix();
        }
      }
      else {
        rotation = 0;
        rotate_matrix();
      }
    }
  }

};

class Board { //10x20
  Coord pos = Coord(50, 50);
  int matrix[24][10];
  SDL_Color color_matrix[24][10];
  int block_size = 20;

public:
  Board() {
    SDL_Color blank = {255, 255, 255, 255};
    for (int i = 0; i < 24; i++) {
      for (int j = 0; j < 10; j++) {
        matrix[i][j] = 0;
        color_matrix[i][j] = blank;
      }
    }
  }

  void set_block(const Block& block) {
    for (int i = 0; i < block.size; i++) {
      for (int j = 0; j < block.size; j++) {
        if (block.matrix[i][j] == 1) {
          matrix[int(block.pos.y) + i][int(block.pos.x) + j] = block.matrix[i][j];
          color_matrix[int(block.pos.y) + i][int(block.pos.x) + j] = block.color;
        }
      }
    }
  }

  void draw(const Engine& engine) const {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 10; j++) {
        Rect draw_rect(pos.x + (j * block_size), pos.y + (i * block_size), block_size, block_size);
        engine.draw_image("img/no_part.png", draw_rect, 0, color_matrix[i + 4][j]);
      }
    }
  }

  void draw_block(const Engine& engine, const Block& block) const {
    for (int i = 0; i < block.size; i++) {
      for (int j = 0; j < block.size; j++) {
        int x = j + int(block.pos.x);
        int y = (i - 4) + int(block.pos.y);

        if ((y >= 0) && (block.matrix[i][j] == 1)) {
          Rect draw_rect(pos.x + (x * block_size), pos.y + (y * block_size), block_size, block_size);
          engine.draw_image("img/no_part.png", draw_rect, 0, block.color);
        }
      }
    }
  }
};

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
  // End Test

  while(!engine.get_exit()) {
    engine.update_inputs();

    board.draw(engine);
    board.draw_block(engine, block);

    engine.draw_text(std::to_string(engine.get_fps()) + " fps", Coord(5, 5), {255, 0, 0, 0}, 16);
    engine.render();
  }
  return 0;
}
