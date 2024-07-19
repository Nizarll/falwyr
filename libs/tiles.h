#pragma once
#include "types.h"
#include "raylib.h"
#include <vector>

struct tile;
struct layer;

enum class Shape : uint {
  SQUARE = 0,
  TRI,
};

enum class Tiles : unsigned int {
  GRASS_LT = 0,
  GRASS_T,
  GRASS_LR,
  PLAT_L,
  PLAT,
  PLAT_R,
  PLAT_LB,
  PLAT_B,
  PLAT_RB,
  EMPTY,
};


struct tile {
  u16 kind;
  u16 shape;
  v2 data;
  v2 size;
};
