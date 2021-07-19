#ifndef BOARD_H
#define BOARD_H

#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include "Block.hpp"

struct BlockAnimation {
  std::string image = "img/no_part.png";
  Coord pos = Coord(400, 300);
  float block_size = 40;
  bool over = false;

  int res = 5;

  std::vector<Rect> src_parts;
  std::vector<Rect> dst_parts;
  std::vector<Coord> forces;

  Coord scale = Coord(block_size, block_size) / 20;
  SDL_Color color = {255, 255, 255, 0};

  BlockAnimation(const Engine& engine, const std::string& image, const Coord& pos, const float& block_size, const SDL_Color& color)
  : image(image), pos(pos), block_size(block_size), color(color) {

    float x = float((rand() % (res - 1)) + 1) / res;
    float y = float((rand() % (res - 1)) + 1) / res;
    Rect size = engine.size_image(image);
    src_parts.push_back(Rect(0, 0, size.w * x, size.h * y));
    src_parts.push_back(Rect(size.w * x, 0, size.w * (1 - x), size.h * y));
    src_parts.push_back(Rect(0, size.h * y, size.w * x, size.h * (1 - y)));
    src_parts.push_back(Rect(size.w * x, size.h * y, size.w * (1 - x), size.h * (1 - y)));

    dst_parts.push_back(Rect(pos.x, pos.y, block_size * x, block_size * y));
    dst_parts.push_back(Rect(pos.x + block_size * x, pos.y, block_size * (1 - x), block_size * y));
    dst_parts.push_back(Rect(pos.x, pos.y + block_size * y, block_size * x, block_size * (1 - y)));
    dst_parts.push_back(Rect(pos.x + block_size * x, pos.y + block_size * y, block_size * (1 - x), block_size * (1 - y)));

    for (int i = 0; i < 4; i++) {
      x = 0.05 + float((rand() % 10) + 1) / 100;
      y = 0.05 + float((rand() % 10) + 1) / 100;
      x *= scale.x;
      y *= scale.y;
      forces.push_back(Coord(x, y));
    }

    forces[0] *= Coord(-1, -1);
    forces[1] *= Coord(1, -1);
    forces[2] *= Coord(-1, 1);
    forces[3] *= Coord(1, 1);
  }

  void update(const Engine& engine) {
    if (!over) {
      for (auto& f : forces) {
        f += Coord(0, 0.001) * scale;
      }

      over = true;
      for (unsigned i = 0; i < dst_parts.size(); i++) {
        dst_parts[i].x += forces[i].x;
        dst_parts[i].y += forces[i].y;
        Coord dst_coord(dst_parts[i].x, dst_parts[i].y);
        if ((dst_coord >= Coord(0, 0)) && (dst_coord < Coord(engine.get_width(), engine.get_height()))) {
          over = false;
        }
      }
    }
  }

  void draw (const Engine& engine) {
    if (!over) {
      for (unsigned i = 0; i < src_parts.size(); i++) {
        engine.draw_image(image, dst_parts[i], 0, color, src_parts[i]);
      }
    }
  }
};

class Board {
  Coord pos = Coord(50, 50);
  int width = 10;
  int height = 24;
  std::vector<std::vector<int>> matrix;
  std::vector<std::vector<SDL_Color>> color_matrix;

  int block_size = 20;
  int hidden = 4;

  SDL_Color blank = {255, 255, 255, 255};

  std::vector<BlockAnimation> animations;

public:
  Board() {

    for (int i = 0; i < width; i++) {
      matrix.push_back(std::vector<int>(height, 0));
      color_matrix.push_back(std::vector<SDL_Color>(height, blank));
    }
  }

  bool check_block(const Block& block) {

    for (const auto& p : block.get_parts()) {
      if (int(p.y) >= height) { return false; }
      else if (int(p.x) < 0) { return false; }
      else if (int(p.x) >= width) { return false; }
      else if (matrix[int(p.x)][int(p.y)] == 1) { return false; }
    }
    return true;
  }

  int check_rows(const Engine& engine) {
    int ret = 0;

    for (int j = 0; j < height; j++) {
      bool remove = true;
      for (int i = 0; (i < width) && remove; i++) {
        if (matrix[i][j] == 0) { remove = false; }
      }
      if (remove) {
        ret += 1;
        for (int i = 0; i < width; i++) {
          SDL_Color c = color_matrix[i][j];
          animations.push_back(BlockAnimation(engine, "img/no_part.png", Coord(pos.x + (i * block_size), pos.y + ((j - hidden) * block_size)), block_size, c));

          matrix[i].erase(matrix[i].begin() + j);
          color_matrix[i].erase(color_matrix[i].begin() + j);
          matrix[i].insert(matrix[i].begin(), 0);
          color_matrix[i].insert(color_matrix[i].begin(), blank);
        }
      }
    }
    return ret;
  }

  bool is_over() {
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

  void draw(const Engine& engine) { //const lehetne :/
    unsigned temp_size = animations.size();
    unsigned i = 0;
    while (i < temp_size) {
      if (animations[i].over) {
        animations.erase(animations.begin() + i);
        temp_size -= 1;
      }
      else { i++; }
    }


    for (unsigned i = 0; i < matrix.size(); i++) {
      for (unsigned j = 0; j < matrix[0].size() - hidden; j++) {
        Rect draw_rect(pos.x + (i * block_size), pos.y + (j * block_size), block_size, block_size);
        engine.draw_image("img/no_part.png", draw_rect, 0, color_matrix[i][j + hidden]);
      }
    }

    for (auto& a : animations) {
      a.update(engine);
      a.draw(engine);
    }
    std::cerr << animations.size() << "-------\r";
  }

  Coord get_pos() const { return pos; }
  int get_block_size() const { return block_size; }
  int get_hidden() const { return hidden; }
};

#endif
