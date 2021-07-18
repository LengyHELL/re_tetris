#ifndef BLOCK_H
#define BLOCK_H

#include "engine/Engine.hpp"
#include "engine/Rect.hpp"
#include "engine/Coord.hpp"

#include <vector>
#include <math.h>

class Block {
  Coord pos = Coord(3, 3);
  int rotation = 0;
  int rotations = 0;
  std::vector<Coord> parts;

  Coord animation_pos = pos;
  float rotation_pos = rotation;

  SDL_Color color = {128, 0, 0, 255};

public:
  Block(const char& type) {
    parts.clear();

    switch(type) {
      case 'o':
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(0, 1));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(1, 1));
        rotations = 0;
      break;
      case 'i':
        parts.push_back(Coord(-2, 0));
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        rotations = 2;
      break;
      case 'j':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(1, 1));
        rotations = 4;
      break;
      case 'l':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(-1, 1));
        rotations = 4;
      break;
      case 't':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(0, 1));
        rotations = 4;
      break;
      case 's':
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(-1, 1));
        parts.push_back(Coord(0, 1));
        rotations = 2;
      break;
      case 'z':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(0, 1));
        parts.push_back(Coord(1, 1));
        rotations = 2;
      break;
    }
  }

  void rotate() {
    rotation += 1;
    if (rotation >= rotations) {
      rotation -= rotations;
    }
  }

  void move(const Coord& dir) { pos += dir; }

  void draw(const Engine& engine, const Coord& rel_pos, const int& block_size, const float& duration) {
    animation_pos += ((pos - animation_pos) / duration) * engine.get_ft();
    rotation_pos += ((rotation - rotation_pos) / duration) * engine.get_ft();
    float deg = 90 * rotation_pos;
    float deg_rad = (deg / 180) * M_PI;

    for (const auto& p : parts) {
      Coord rotated;
      rotated.x = cos(deg_rad) * p.x - sin(deg_rad) * p.y;
      rotated.y = sin(deg_rad) * p.x + cos(deg_rad) * p.y;
      Coord draw_pos = rel_pos + (animation_pos + rotated) * block_size;

      Rect draw_rect(draw_pos.x, draw_pos.y, block_size, block_size);
      engine.draw_image("img/no_part.png", draw_rect, deg, color);
    }
  }

  Coord get_pos() const { return pos; }

  std::vector<Coord> get_parts() const {
    float deg = rotation * 90;
    float deg_rad = (deg / 180) * M_PI;
    std::vector<Coord> temp;

    for (const auto& p : parts) {
      Coord rotated;
      rotated.x = lround(cos(deg_rad) * p.x - sin(deg_rad) * p.y);
      rotated.y = lround(sin(deg_rad) * p.x + cos(deg_rad) * p.y);
      temp.push_back(rotated + pos);
    }
    return temp;
  }

  SDL_Color get_color() const { return color; }
};

#endif
