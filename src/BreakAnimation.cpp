#include "Animation.hpp"

BreakAnimation::BreakAnimation(const Engine& engine, const std::string& image, const Coord& pos, const float& block_size, const SDL_Color& color)
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

void BreakAnimation::update(const Engine& engine) {
  if (!over) {
    for (auto& f : forces) {
      f += Coord(0, 0.001) * scale * engine.get_ft();
    }

    over = true;
    for (unsigned i = 0; i < dst_parts.size(); i++) {
      dst_parts[i].x += forces[i].x * engine.get_ft();
      dst_parts[i].y += forces[i].y * engine.get_ft();
      Coord dst_coord(dst_parts[i].x, dst_parts[i].y);
      if ((dst_coord >= Coord(0, 0)) && (dst_coord < Coord(engine.get_width(), engine.get_height()))) {
        over = false;
      }
    }
  }
}

void BreakAnimation::draw (const Engine& engine) const {
  if (!over) {
    for (unsigned i = 0; i < src_parts.size(); i++) {
      engine.draw_image(image, dst_parts[i], 0, color, src_parts[i]);
    }
  }
}
