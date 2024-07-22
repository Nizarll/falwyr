#pragma once
#include "types.h"
#include "raylib.h"
#include <vector>

constexpr auto level_size = 256;

struct tile;

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
  GRASS_UPL,
  GRASS_UPR,
  GRASS_UPBR,
  GRASS_UPBL,
  EMPTY,
  LEN,
};

struct tile {
  u16 kind;
  u16 shape;
};
