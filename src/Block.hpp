#ifndef BLOCK_H
#define BLOCK_H

#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include <vector>
#include <math.h>

class Block {
  Coord pos = Coord(0, 0);
  int rotation = 0;
  int rotations = 0;
  std::vector<Coord> parts;

  Coord animation_pos = pos;
  float rotation_pos = rotation;
  float animation_threshold = 0.02;

  SDL_Color color = {0, 0, 0, 255};

public:
  Block(const char& type);
  Block();

  void rotate();
  void move(const Coord& dir);
  void update(const Engine& engine, const float& duration);
  void draw(const Engine& engine, const Coord& rel_pos, const std::string& style, const int& block_size, const bool& shadow = true, const bool& animations = true) const;

  Coord get_pos() const;
  std::vector<Coord> get_parts() const;
  SDL_Color get_color() const;

  void set_pos(const Coord& new_pos);
};

#endif
