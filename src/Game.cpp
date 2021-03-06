#include "Game.hpp"

Coord get_next_pos(const Block& block, const Rect& rect, const Coord& block_size) {
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

Game::Game(const Engine& engine) {
    board = Board(Coord(0, engine.get_height() / 10));
    active_block.set_pos(Coord(4, 1));
    block_size = Coord((engine.get_width() / 2) / 10, (engine.get_height() - (engine.get_height() / 5)) / 20);
    std::cerr << block_size << std::endl;

    //initializing hud
    top = Frame("img/basic_style.png", Rect(0, 0, engine.get_width(), engine.get_height() / 10));
    bottom = Frame("img/basic_style.png", Rect(0, engine.get_height() - (engine.get_height() / 10), engine.get_width(), engine.get_height() / 10));
    middle = Frame("img/basic_style.png", Rect(engine.get_width() / 2, engine.get_height() / 10, engine.get_width() / 2, engine.get_height() - (engine.get_height() / 5)));
    info = TextBox("img/basic_style.png", Rect((engine.get_width() / 2) + (block_size.x / 2), engine.get_height() / 2, (engine.get_width() / 2) - block_size.x, engine.get_height() / 5), info_text, 16, {0, 0, 0, 0});
    info.update(engine);

    next_rect.x = ((engine.get_width() / 4) * 3) - ((5 * block_size.x) / 2);
    next_rect.y = ((engine.get_height() / 6) * 2) - ((5 * block_size.y) / 2);
    next_rect.w = block_size.x * 5;
    next_rect.h = block_size.y * 5;
    next = Frame("img/basic_style.png", next_rect);

    next_pos = get_next_pos(next_block, next_rect, block_size);
    //Coord((engine.get_width() / 4) * 3, (engine.get_height() / 4))
  }

void Game::update(const Engine& engine, const Config& cfg) {
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
          int clear = board.update_rows(engine, animations, style, block_size, cfg.animations);
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

void Game::draw(const Engine& engine, const Config& cfg) const {
    engine.draw_image("img/blank.png", Rect(0, 0, engine.get_width(), engine.get_height()));
    middle.draw(engine);
    next.draw(engine);

    board.draw(engine, style, block_size);
    active_block.draw(engine, board.get_pos() - Coord(0, board.get_hidden()) * block_size, style, block_size, cfg.shadow, cfg.animations);
    next_block.draw(engine, next_pos, style, block_size, false, false);
    bottom.draw(engine);

    for (auto& a : animations) { a.draw(engine); }

    top.draw(engine);
    info.draw(engine);
    engine.draw_text("Score: " + std::to_string(get_score()), Coord(engine.get_width() / 2 + (block_size.x / 2), (engine.get_height() / 10) + (block_size.y / 2)), {0, 0, 0, 0}, 16);
    engine.draw_text("Level: " + std::to_string(get_level()), Coord(engine.get_width() / 2 + (block_size.x / 2), (engine.get_height() / 10) + (block_size.y / 2) + 20), {0, 0, 0, 0}, 16);
  }
