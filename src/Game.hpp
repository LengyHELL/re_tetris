#ifndef GAME_H
#define GAME_H

#include "Block.hpp"
#include "Board.hpp"

/*
  800 ms initial
  85 ms decrease every level
  9 max level?

  for n level
  1 row         2row            3row            4row
  40 * n        100 * n         300 * n         1200 * n
  levelup after 10 lines
 */

class Game {
  Board board;
  Block active_block = Block('t');

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

  char types[7] = {'o', 'i', 'j', 'l', 't', 's', 'z'};

  int score = 0;
  int level = 1;
  int clears = 0;

  int required_clears = 10;
  float step_decrease = 85;

  bool over = false;

public:

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
        active_block = Block(types[rand() % 7]);
        if (board.is_over()) {
          over = true;
        }
        else {
          int clear = board.check_rows();
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
    }
  }

  void draw(const Engine& engine) {
    board.draw(engine);
    active_block.draw(engine, board.get_pos() - Coord(0, board.get_hidden()) * board.get_block_size(), board.get_block_size(), step_current / 3);
    engine.draw_text("Score: " + std::to_string(score), Coord(400, 5), {255, 255, 255, 0}, 16);
    engine.draw_text("Level: " + std::to_string(level), Coord(400, 25), {255, 255, 255, 0}, 16);
  }

  bool is_over() const { return over; }
};

#endif
