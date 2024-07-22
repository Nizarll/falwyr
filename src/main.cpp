#include "../libs/entity.h"
#include "../libs/levels.h"
#include "../libs/tiles.h"

#include <cmath>
#include <cstdlib>
#define PHYSAC_STATIC
#define PHYSAC_IMPLEMENTATION
#include <raylib.h>

#include <algorithm>
#include <fstream>
#include <functional>
#include <string.h>
#include <execution>
#include <iostream>
#include <iterator>
#include <raymath.h>
#include <unistd.h>
#include <utility>
#include <raylib.h>

static Color sky{59, 163, 255};

using anim_vec = std::vector<anim*>;
constexpr auto max_row = 3;
constexpr auto tile_size = 16;
constexpr auto icon_size = 60;

static Texture2D img;
static std::array<Rectangle, level_size> recs{};

void draw_lv1() {
  for (size_t x = 0; x < level_size; x++) {
    for (size_t y = 0; y < level_size; y++) {
      if (lv_one[x][y].kind != (u16)Tiles::EMPTY) {
        size_t kind = lv_one[x][y].kind;
        DrawTexturePro(img, recs[kind], Rectangle{x * icon_size, y * icon_size - 14600.0f, icon_size, icon_size}, v2{0, 0}, .0f, WHITE);
      }
    }
  }
}

void init() {
  InitPhysics();
  img = LoadTexture("resources/tilesheet.png");
  int source_x{}, source_y{};
  std::for_each(std::execution::par, recs.begin(), recs.end(), [&](auto& p){
    p.x = source_x;
    p.y = source_y;
    p.width = tile_size;
    p.height = tile_size;
    source_x += tile_size;
    if (source_x >= max_row * tile_size) {
      source_x = 0;
      source_y += tile_size;
    }
  });
}

void update_physics(){
  int bodiesCount = GetPhysicsBodiesCount();
  for (int i = 0; i < bodiesCount; i++)
  {
    PhysicsBody body = GetPhysicsBody(i);
  }
}

int main() {
  InitWindow(WIDTH , HEIGHT, "raylib");
  SetTargetFPS(60);
  init();
  anim walk{LoadTexture("resources/player_sheet.png"),
    v2{16.0f * 3.0f, 16.0f},
    v2{80.0f, 80.0f},
    v2{16.0f, 16.0f},
    0,
    3,
    .0f,
    10
  };
  anim_vec anims{&walk};
  entity player(anims, v2{100, 100}, v2{WIDTH/2.0f, HEIGHT/2.0f});
  player.curr = (u16)Anims::RUN;
  player.anims = anims;
  while (!WindowShouldClose()) {
    update_physics();
    BeginDrawing();
    ClearBackground(sky);
    if (IsKeyDown(KEY_D))
      player.body->velocity.x += 1.0f;
    if (IsKeyDown(KEY_A))
      player.body->velocity.x -= 1.0f;
    draw_lv1();
    player.draw();
    player.update(GetFrameTime());
    EndDrawing();
  }
  CloseWindow();
  ClosePhysics();
  return EXIT_SUCCESS;
}
