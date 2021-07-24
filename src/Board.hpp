#ifndef BOARD_H
#define BOARD_H

#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "Block.hpp"
#include "Animation.hpp"

class Board {
  Coord pos = Coord(0, 0);
  int width = 10;
  int height = 24;
  std::vector<std::vector<int>> matrix;
  std::vector<std::vector<SDL_Color>> color_matrix;

  int hidden = 4;

  SDL_Color blank = {255, 255, 255, 255};

public:
  Board(const Coord& pos) : pos(pos) {

    for (int i = 0; i < width; i++) {
      matrix.push_back(std::vector<int>(height, 0));
      color_matrix.push_back(std::vector<SDL_Color>(height, blank));
    }
  }

  bool check_block(const Block& block) const {

    for (const auto& p : block.get_parts()) {
      if (int(p.y) >= height) { return false; }
      else if (int(p.x) < 0) { return false; }
      else if (int(p.x) >= width) { return false; }
      else if (matrix[int(p.x)][int(p.y)] == 1) { return false; }
    }
    return true;
  }

  int update_rows(const Engine& engine, std::vector<BreakAnimation>& animations, const std::string& style, const float& block_size) {
    int ret = 0;

    for (int j = 0; j < height; j++) {
      bool remove = true;
      for (int i = 0; (i < width) && remove; i++) {
        if (matrix[i][j] == 0) { remove = false; }
      }
      if (remove) {
        ret += 1;
        for (int i = 0; i < width; i++) {
          animations.push_back(BreakAnimation(engine, style, Coord(pos.x + (i * block_size), pos.y + ((j - hidden) * block_size)), block_size, color_matrix[i][j]));
          matrix[i].erase(matrix[i].begin() + j);
          color_matrix[i].erase(color_matrix[i].begin() + j);
          matrix[i].insert(matrix[i].begin(), 0);
          color_matrix[i].insert(color_matrix[i].begin(), blank);
        }
      }
    }
    return ret;
  }

  bool is_over() const {
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < hidden; j++) {
        if (matrix[i][j] == 1) { return true; }
      }
    }
    return false;
  }

  void set_block(const Block& block) {

    for (const auto& p : block.get_parts()) {
      matrix[int(p.x)][int(p.y)] = 1;
      color_matrix[int(p.x)][int(p.y)] = block.get_color();
    }
  }

  void draw(const Engine& engine, const std::string& style, const float& block_size) const {

    for (unsigned i = 0; i < matrix.size(); i++) {
      for (unsigned j = 0; j < matrix[0].size() - hidden; j++) {
        Rect draw_rect(pos.x + (i * block_size), pos.y + (j * block_size), block_size, block_size);
        engine.draw_image(style, draw_rect, 0, color_matrix[i][j + hidden]);
      }
    }
  }

  Coord get_pos() const { return pos; }
  int get_hidden() const { return hidden; }
};

#endif
