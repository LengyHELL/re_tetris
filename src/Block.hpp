#ifndef BLOCK_H
#define BLOCK_H

class Block {
  Coord pos = Coord(0, 3);
  int rotation = 0;
  int** matrix = nullptr; //not very good, kinda bad
  int rotations = 0;
  int size = 0;
  SDL_Color color = {128, 0, 0, 255};

  void rotate_matrix() {
    for (int i = 0; i < size / 2; i++) {
      for (int j = i; j < size - i - 1; j++) {
        int k = size - i - 1;
        int l = size - j - 1;
        int temp = matrix[i][j];
        matrix[i][j] = matrix[l][i];
        matrix[l][i] = matrix[k][l];
        matrix[k][l] = matrix[j][k];
        matrix[j][k] = temp;
      }
    }
  }

public:
  Block(const Block& other) { *this = other; }
  Block(const char& type) {
    matrix = new int*[4];
    for (int i = 0; i < 4; i++) {
      matrix[i] = new int[4];
    }

    switch(type) { //o i j l t s z
      case 'o':
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][1] = 1;
        matrix[2][2] = 1;
        rotations = 0;
        size = 4;
      break;
      case 'i':
        matrix[2][0] = 1;
        matrix[2][1] = 1;
        matrix[2][2] = 1;
        matrix[2][3] = 1;
        rotations = 2;
        size = 4;
      break;
      case 'j':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][2] = 1;
        rotations = 4;
        size = 3;
      break;
      case 'l':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][0] = 1;
        rotations = 4;
        size = 3;
      break;
      case 't':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][1] = 1;
        rotations = 4;
        size = 3;
      break;
      case 's':
        matrix[1][1] = 1;
        matrix[1][2] = 1;
        matrix[2][0] = 1;
        matrix[2][1] = 1;
        rotations = 2;
        size = 3;
      break;
      case 'z':
        matrix[1][0] = 1;
        matrix[1][1] = 1;
        matrix[2][1] = 1;
        matrix[2][2] = 1;
        rotations = 2;
        size = 3;
      break;
    }
  }
  ~Block() {
    for (int i = 0; i < 4; i++) {
      delete[] matrix[i];
    }
    delete[] matrix;
  }

  void rotate() {
    if (rotations == 4) {
      rotation += 1;
      if (rotation == 4) { rotation = 0; }
      rotate_matrix();
    }
    else if (rotations == 2) {
      if (rotation == 0) {
        rotation = 1;
        for (int i = 0; i < 3; i++) {
          rotate_matrix();
        }
      }
      else {
        rotation = 0;
        rotate_matrix();
      }
    }
  }

  void move(const Coord& dir) {
    if ((dir.x == -1) || (dir.x == 1) || (dir.y == -1) || (dir.y == 1)) {
      pos += dir;
    }
  }

  void operator=(const Block& other) {
    pos = other.pos;
    rotation = other.rotation;
    rotations = other.rotations;
    size = other.size;
    color = other.color;

    matrix = new int*[4];
    for (int i = 0; i < 4; i++) {
      matrix[i] = new int[4];
      for (int j = 0; j < 4; j++) {
        matrix[i][j] = other.matrix[i][j];
      }
    }
  }

  Coord get_pos() const { return pos; }
  int get_rotation() const { return rotation; }
  int** get_matrix() const { return matrix; }
  int get_size() const { return size; }
  SDL_Color get_color() const { return color; }
};

#endif
