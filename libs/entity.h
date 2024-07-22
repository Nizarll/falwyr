#pragma once

#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "types.h"
#include <iostream>
#include <vector>

#define PHYSAC_STATIC
#define PHYSAC_IMPLEMENTATION
#include "physac.h"


#define WIDTH  800
#define HEIGHT 800

typedef PhysicsBody phbody;

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
  phbody body;
  v2 size;
  u16 curr;
  //state s;
  entity(std::vector<anim*>& anims, v2 size, v2 pos);
  void update(f32 dt);
  void draw();
};
