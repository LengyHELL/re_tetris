#ifndef BOARD_H
#define BOARD_H

class Board {
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

  bool check_block(const Block& block) {
    for (int i = 0; i < block.get_size(); i++) {
      for (int j = 0; j < block.get_size(); j++) {

        if (block.get_matrix()[i][j] == 1) {
          if ((int(block.get_pos().y) + i) >= 24) { return false; }
          else if ((int(block.get_pos().x) + j) < 0) { return false; }
          else if ((int(block.get_pos().x) + j) >= 10) { return false; }
          else if (matrix[int(block.get_pos().y) + i][int(block.get_pos().x) + j] == 1) { return false; }
        }
      }
    }
    return true;
  }

  void set_block(const Block& block) {
    for (int i = 0; i < block.get_size(); i++) {
      for (int j = 0; j < block.get_size(); j++) {
        if (block.get_matrix()[i][j] == 1) {
          matrix[int(block.get_pos().y) + i][int(block.get_pos().x) + j] = block.get_matrix()[i][j];
          color_matrix[int(block.get_pos().y) + i][int(block.get_pos().x) + j] = block.get_color();
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
    for (int i = 0; i < block.get_size(); i++) {
      for (int j = 0; j < block.get_size(); j++) {
        int x = j + int(block.get_pos().x);
        int y = (i - 4) + int(block.get_pos().y);

        if ((y >= 0) && (block.get_matrix()[i][j] == 1)) {
          Rect draw_rect(pos.x + (x * block_size), pos.y + (y * block_size), block_size, block_size);
          engine.draw_image("img/no_part.png", draw_rect, 0, block.get_color());
        }
      }
    }
  }
};

#endif
