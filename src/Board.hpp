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
  Board(const Coord& pos);

  bool check_block(const Block& block) const;
  int update_rows(const Engine& engine, std::vector<BreakAnimation>& animations, const std::string& style, const Coord& block_size, const bool& get_animations);
  void draw(const Engine& engine, const std::string& style, const Coord& block_size) const;

  bool is_over() const;
  Coord get_pos() const { return pos; }
  int get_hidden() const { return hidden; }

  void set_block(const Block& block);
};

#endif
