#include "../libs/entity.h"
#include "../libs/tiles.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <raylib.h>

using anim_vec = std::vector<anim*>;

tile grass {
  Texture2D{},
  Shape::SQUARE,
  v2{16.0f, .0f},
  v2{16.0f, 16.0f},
  (u16)Tiles::GRASS
};

int main() {
  InitWindow(WIDTH , HEIGHT, "raylib");
  SetTargetFPS(60);
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
  entity player;
  grass.init();
  player.anims = anims;
  player.curr = (u16) Anims::RUN;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);
    player.update(GetFrameTime());
    player.draw();
    grass.draw(v2{100.0f, 100.0f});
    EndDrawing();
  }
  return EXIT_SUCCESS;
}
