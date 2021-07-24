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
  Block(const char& type) {
    parts.clear();

    switch(type) {
      case 'o':
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(0, 1));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(1, 1));
        rotations = 0;
        color.r = 192;
        color.g = 192;
        color.b = 0;
      break;
      case 'i':
        parts.push_back(Coord(-2, 0));
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        rotations = 2;
        color.r = 0;
        color.g = 192;
        color.b = 192;
      break;
      case 'j':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(1, 1));
        rotations = 4;
        color.r = 0;
        color.g = 0;
        color.b = 192;
      break;
      case 'l':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(-1, 1));
        rotations = 4;
        color.r = 192;
        color.g = 96;
        color.b = 0;
      break;
      case 't':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(0, 1));
        rotations = 4;
        color.r = 192;
        color.g = 0;
        color.b = 192;
      break;
      case 's':
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(1, 0));
        parts.push_back(Coord(-1, 1));
        parts.push_back(Coord(0, 1));
        rotations = 2;
        color.r = 0;
        color.g = 192;
        color.b = 0;
      break;
      case 'z':
        parts.push_back(Coord(-1, 0));
        parts.push_back(Coord(0, 0));
        parts.push_back(Coord(0, 1));
        parts.push_back(Coord(1, 1));
        rotations = 2;
        color.r = 192;
        color.g = 0;
        color.b = 0;
      break;
    }
  }
  Block() {
    char types[7] = {'o', 'i', 'j', 'l', 't', 's', 'z'};
    *this = Block(types[rand() % 7]);
  }

  void rotate() {
    if (rotations > 0) {
      rotation += 1;
      if (rotation >= rotations) {
        rotation = 0;
      }
    }
  }

  void move(const Coord& dir) { pos += dir; }

  void update(const Engine& engine, const float& duration) {
    if (Coord(pos - animation_pos).len() < animation_threshold) {
      animation_pos = pos;
    }
    else {
      animation_pos += ((pos - animation_pos) / duration) * engine.get_ft();
    }


    float rotation_value = float(rotation) - rotation_pos;
    while ((rotation_value < 0) && (rotations == 4)) { rotation_value += float(rotations); } //sketchy

    if (abs(rotation_value) < animation_threshold) {
      rotation_pos = rotation;
    }
    else {
      rotation_pos += (rotation_value / duration) * engine.get_ft();
    }
  }

  void draw(const Engine& engine, const Coord& rel_pos, const std::string& style, const int& block_size) const {
    float deg_rad = rotation_pos * (M_PI / 2);
    float deg = (deg_rad / M_PI) * 180;

    for (const auto& p : parts) {
      Coord rotated;
      rotated.x = cos(deg_rad) * p.x - sin(deg_rad) * p.y;
      rotated.y = sin(deg_rad) * p.x + cos(deg_rad) * p.y;
      if ((int(deg) % 90) != 0) { rotated *= 0.96; }
      Coord draw_pos = rel_pos + (animation_pos + rotated) * float(block_size);

      Rect draw_rect(draw_pos.x, draw_pos.y, block_size, block_size);
      engine.draw_image(style, draw_rect, deg, color);
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
