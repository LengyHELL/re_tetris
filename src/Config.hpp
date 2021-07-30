#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>

struct Config {
  int width = 400;
  int height = 500;
  bool animations = true;
  bool shadow = true;
  int fps_cap = 120;

  void load(std::string filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
      std::string line;

      int line_count = 0;
      while(getline(file, line)) {
        line_count += 1;
        int sign = line.find("=");

        if ((sign > 0) && (sign < int(line.size()) - 1)) {
          std::string key = line.substr(0, sign);
          int value = std::stoi(line.substr(sign + 1, line.size() - sign));

          if (key == "width") { width = value; }
          else if (key == "height") { height = value; }
          else if (key == "animations") { animations = bool(value); }
          else if (key == "shadow") { shadow = bool(value); }
          else if (key == "fps_cap") { fps_cap = value; }
          else { std::cerr << "No paramter to set named '" << key << "'!"; }
        }
        else { std::cerr << "Error reading config file at line " << line_count << "!" << std::endl; }
      }
    }
    else { std::cerr << "No config file '" << filename << "' found!" << std::endl; }
  }
  void save(std::string filename) {}
};

#endif
