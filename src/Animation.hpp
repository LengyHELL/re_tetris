#ifndef ANIMATION_H
#define ANIMATION_H

#include "engine/Engine.hpp"
#include <vector>

class Animation {
public:
  virtual bool is_over() const = 0;
  virtual void update(const Engine& engine) = 0;
  virtual void draw(const Engine& engine) const = 0;
};

class BreakAnimation : public Animation {
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

public:
  BreakAnimation(const Engine& engine, const std::string& image, const Coord& pos, const float& block_size, const SDL_Color& color);

  void update(const Engine& engine);
  void draw (const Engine& engine) const;

  bool is_over() const { return over; }
};

#endif
