#ifndef GAME_H
#define GAME_H

#include "Block.hpp"
#include "Board.hpp"
#include "Animation.hpp"

/*
  800 ms initial
  85 ms decrease every level
  9 max level?

  for n level
  1 row         2row            3row            4row
  40 * n        100 * n         300 * n         1200 * n
  levelup after 10 lines
 */

Coord get_next_pos(const Block& block, const Rect& rect, const float& block_size) {
  Coord min(0, 0);
  Coord max(0, 0);
  bool first = true;
  for (const auto& p : block.get_parts()) {
    if (first) {
      first = false;
      max.x = min.x = p.x;
      max.y = min.y = p.y;
    }
    else {
      if (p.x > max.x) { max.x = p.x; }
      if (p.y > max.y) { max.y = p.y; }
      if (p.x < min.x) { min.x = p.x; }
      if (p.y < min.y) { min.y = p.y; }
    }
  }
  Coord ret = (max - min) + 1;
  ret = (Coord(rect.x, rect.y) + (Coord(rect.w, rect.h) / 2)) - ((ret * block_size) / 2);
  ret -= (min * block_size);
  return ret;
}

class Game {
  Board board = Board(Coord(0, 0));
  Block active_block = Block();
  Block next_block = Block();
  Coord next_pos = Coord(0, 0);
  Rect next_rect = Rect(0, 0, 0, 0);

  float block_size = 20;
  std::string style = "img/no_part.png";

  float step = 800; //ms
  float step_current = step;
  float step_timer = 0;
  float side_timer = 0;

  Coord x_movement = Coord(0, 0);
  Coord y_movement = Coord(0, 0);
  bool rotation = false;

  bool left_lock = true;
  bool right_lock = true;
  bool up_lock = true;
  bool down_lock = true;

  int score = 0;
  int level = 1;
  int clears = 0;

  int required_clears = 10;
  float step_decrease = 85;

  bool over = false;
  std::vector<BreakAnimation> animations;

  //hud elements
  Frame top;
  Frame bottom;
  Frame middle;
  Frame next;
  std::string info_text = "Left/Right - Left and Right arrows\nBoost - Down arrow\nRotate - Up arrow";
  TextBox info;

public:

  Game(const Engine& engine) {
    board = Board(Coord(0, engine.get_height() / 10));
    active_block.set_pos(Coord(4, 1));
    block_size = (engine.get_width() / 2) / 10;

    //initializing hud
    top = Frame("img/basic_style.png", Rect(0, 0, engine.get_width(), engine.get_height() / 10));
    bottom = Frame("img/basic_style.png", Rect(0, engine.get_height() - (engine.get_height() / 10), engine.get_width(), engine.get_height() / 10));
    middle = Frame("img/basic_style.png", Rect(engine.get_width() / 2, engine.get_height() / 10, engine.get_width() / 2, engine.get_height() - (engine.get_height() / 5)));
    info = TextBox("img/basic_style.png", Rect(210, 250, 180, 100), info_text, 16, {0, 0, 0, 0});
    info.update(engine);

    next_rect.x = ((engine.get_width() / 4) * 3) - ((5 * block_size) / 2);
    next_rect.y = ((engine.get_height() / 6) * 2) - ((5 * block_size) / 2);
    next_rect.w = block_size * 5;
    next_rect.h = block_size * 5;
    next = Frame("img/basic_style.png", next_rect);

    next_pos = get_next_pos(next_block, next_rect, block_size);
    //Coord((engine.get_width() / 4) * 3, (engine.get_height() / 4))
  }

  void update(const Engine& engine) {
    if (!over) {
      x_movement = Coord(0, 0);
      y_movement = Coord(0, 0);
      rotation = false;

      step_timer += engine.get_ft();
      side_timer += engine.get_ft();

      if (engine.keyboard_state[SDL_SCANCODE_LEFT] && !left_lock) {
        left_lock = true;
        x_movement += Coord(-1, 0);
        side_timer = 0;
      }
      else if (!engine.keyboard_state[SDL_SCANCODE_LEFT]) { left_lock = false; }

      if (engine.keyboard_state[SDL_SCANCODE_RIGHT] && !right_lock) {
        right_lock = true;
        x_movement += Coord(1, 0);
        side_timer = 0;
      }
      else if (!engine.keyboard_state[SDL_SCANCODE_RIGHT]) { right_lock = false; }

      if (engine.keyboard_state[SDL_SCANCODE_UP] && !up_lock) {
        up_lock = true;
        rotation = true;
      }
      else if (!engine.keyboard_state[SDL_SCANCODE_UP]) { up_lock = false; }

      if (engine.keyboard_state[SDL_SCANCODE_DOWN] && !down_lock) {
        down_lock = true;
        step_current = step / 4;
        step_timer = step_current;
      }
      else if (!engine.keyboard_state[SDL_SCANCODE_DOWN]) {
        down_lock = false;
        step_current = step;
      }

      if (side_timer >= step_current) {
        side_timer = step_current;
        left_lock = false;
        right_lock = false;
      }

      if (step_timer >= step_current) {
        step_timer -= step_current;
        y_movement += Coord(0, 1);
      }

      if (rotation) {
        active_block.rotate();
        if (!board.check_block(active_block)) {
          for (int i = 0; i < 3; i++) { // this is not ok
            active_block.rotate();
          }
        }
      }

      active_block.move(x_movement);
      if (!board.check_block(active_block)) {
        active_block.move(x_movement * -1);
        x_movement = Coord(0, 0);
      }

      active_block.move(y_movement);
      if (!board.check_block(active_block)) {
        active_block.move(y_movement * -1);
        y_movement = Coord(0, 0);
        board.set_block(active_block);
        active_block = next_block;
        next_block = Block();
        active_block.set_pos(Coord(4, 1));
        next_pos = get_next_pos(next_block, next_rect, block_size);

        if (board.is_over()) {
          over = true;
        }
        else {
          int clear = board.update_rows(engine, animations, style, block_size);
          switch(clear) {
            case 1: score += 40 * level; break;
            case 2: score += 100 * level; break;
            case 3: score += 300 * level; break;
            case 4: score += 1200 * level; break;
          }
          clears += clear;
          if ((clears >= required_clears) && ((step - step_decrease) > 0)) {
            clears -= required_clears;
            level += 1;
            step -= step_decrease;
          }
        }
      }

      active_block.update(engine, step_current / 3);

      unsigned temp_size = animations.size();
      unsigned i = 0;
      while (i < temp_size) {
        if (animations[i].is_over()) {
          animations.erase(animations.begin() + i);
          temp_size -= 1;
        }
        else {
          animations[i].update(engine);
          i++;
        }
      }
    }
  }

  void draw(const Engine& engine) const {
    engine.draw_image("img/blank.png", Rect(0, 0, engine.get_width(), engine.get_height()));
    middle.draw(engine);
    next.draw(engine);

    board.draw(engine, style, block_size);
    active_block.draw(engine, board.get_pos() - Coord(0, board.get_hidden()) * block_size, style, block_size);
    next_block.draw(engine, next_pos, style, block_size, false);
    bottom.draw(engine);

    for (auto& a : animations) { a.draw(engine); }

    top.draw(engine);
    info.draw(engine);
    engine.draw_text("Score: " + std::to_string(get_score()), Coord(210, 60), {0, 0, 0, 0}, 16);
    engine.draw_text("Level: " + std::to_string(get_level()), Coord(210, 80), {0, 0, 0, 0}, 16);
  }

  bool is_over() const { return over; }

  int get_score() const { return score; }
  int get_level() const { return level; }
};

#endif
