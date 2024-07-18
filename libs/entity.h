#pragma once
#include "types.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <raylib.h>

#define WIDTH  800
#define HEIGHT 800

enum class Anims : uint {
  RUN = 0,
  IDLE,
  WALK,
  DASH,
  ATTACK_1,
  ATTACK_2,
  ANIMS_LEN
};

struct sheet {
  Texture2D t2d;
  v2 size;
};

struct anim {
  sheet sht;
  v2 size;
  v2 spr_size;
  u16 _curr;
  u16 len;
  f32 last;
  f32 speed;
  void update(f32 dt);
};

struct entity {
  std::vector<anim*> anims;
  v2 pos;
  v2 size;
  u16 curr;
  // state s;
  void update(f32 dt);
  void draw();
};
