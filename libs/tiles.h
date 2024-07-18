#pragma once
#include "types.h"
#include "raylib.h"
#include <variant>
#include <vector>

struct tile;
struct layer;
struct square;
struct tri;

using tiles = std::vector<std::vector<tile>>;
using layers = std::vector<std::vector<layer>>;

enum class Shape : uint {
  SQUARE = 0,
  TRI,
};

enum class Tiles : uint {
  GRASS_L = 0,
  GRASS_R,
  GRASS,
};

struct tile {
  Texture2D atlas;
  Shape s;
  v2 data;
  v2 size;
  u16 kind;
  void init();
  void draw(v2 pos);
  void switchbox();
};

struct layer {
  u8 opacity;
  tiles tls;
};

struct chunk {
  v2 size;
  layers lrs;
  void add_layer(layer l);
};

void hitbox(square s);
void hitbox(tri s);
