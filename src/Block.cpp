#include "Block.hpp"

void insert_shadow(Rect shadow, std::vector<Rect>& shadows) {
  float ls = shadow.x;
  float rs = shadow.x + shadow.w;
  for (const auto& s : shadows) {
    float s_ls = s.x;
    float s_rs = s.x + s.w;

    if ((s_ls <= ls) && (ls <= s_rs)) {
      ls = s_rs;
    }
    if ((s_ls <= rs) && (rs <= s_rs)) {
      rs = s_ls;
    }
  }
  ls = round(ls);
  rs = round(rs);
  shadow.x = ls;
  shadow.w = rs - ls;
  shadows.push_back(shadow);
}

Block::Block(const char& type) {
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

Block::Block() {
  char types[7] = {'o', 'i', 'j', 'l', 't', 's', 'z'};
  *this = Block(types[rand() % 7]);
}

void Block::rotate() {
  if (rotations > 0) {
    rotation += 1;
    if (rotation >= rotations) {
      rotation = 0;
    }
  }
}

void Block::move(const Coord& dir) { pos += dir; }

void Block::update(const Engine& engine, const float& duration) {
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

void Block::draw(const Engine& engine, const Coord& rel_pos, const std::string& style, const int& block_size, const bool& shadow, const bool& animations) const {
  float deg_rad = 0;
  if (animations) { deg_rad = rotation_pos * (M_PI / 2); }
  else { deg_rad = rotation * (M_PI / 2); }

  float deg = (deg_rad / M_PI) * 180;

  std::vector<Rect> shadows;
  std::vector<Rect> cubes;
  for (const auto& p : parts) {
    Coord rotated;
    rotated.x = cos(deg_rad) * p.x - sin(deg_rad) * p.y;
    rotated.y = sin(deg_rad) * p.x + cos(deg_rad) * p.y;
    if ((int(deg) % 90) != 0) { rotated *= 0.96; }
    
    Coord draw_pos;
    if (animations) { draw_pos = rel_pos + (animation_pos + rotated) * float(block_size); }
    else { draw_pos = rel_pos + (pos + rotated) * float(block_size); }


    Rect draw_rect(draw_pos.x, draw_pos.y, block_size, block_size);
    cubes.push_back(draw_rect);

    if (shadow) {
      Rect shadow_rect(draw_pos.x, draw_pos.y + (block_size / 2), block_size, engine.get_height() - draw_pos.y);
      insert_shadow(shadow_rect, shadows);
    }
  }

  for (const auto& s : shadows) {
    engine.draw_image("img/shadow.png", s, 0, {255, 255, 255, 100});
  }
  for (const auto& c : cubes) {
    engine.draw_image(style, c, deg, color);
  }
}

Coord Block::get_pos() const { return pos; }

std::vector<Coord> Block::get_parts() const {
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

SDL_Color Block::get_color() const { return color; }

void Block::set_pos(const Coord& new_pos) {
  pos = new_pos;
  animation_pos = new_pos;
}
