#ifndef GAME_H
#define GAME_H

#include "engine/Frame.hpp"
#include "engine/TextBox.hpp"

#include "Block.hpp"
#include "Board.hpp"
#include "Animation.hpp"
#include "Config.hpp"

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
  Board board = Board(Coord(0, 0));
  Block active_block = Block();
  Block next_block = Block();
  Coord next_pos = Coord(0, 0);
  Rect next_rect = Rect(0, 0, 0, 0);

  Coord block_size = Coord(20, 20);
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

  Game(const Engine& engine);

  void update(const Engine& engine, const Config& cfg);
  void draw(const Engine& engine, const Config& cfg) const;

  bool is_over() const { return over; }
  int get_score() const { return score; }
  int get_level() const { return level; }
};

#endif
