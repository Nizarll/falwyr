#include "raylib.h"
#include <algorithm>
#include <execution>
#include <iostream>
#include <iterator>
#include <vector>

static Texture2D img;

constexpr auto tile_size = 16;
constexpr auto num_tiles = 9;
constexpr auto left_margin= 30;
constexpr auto top_margin= 120;
constexpr auto max_row = 3;
constexpr auto icon_size = 60;
constexpr auto step = icon_size + 8;

constexpr auto map_size = 1024;

typedef Vector2 v2;
using pair = std::pair<Rectangle, Rectangle>;
using tiles = std::array<int, map_size>;

std::array<pair, num_tiles> recs{};
std::array<tiles, map_size> tls{};

static v2 orgn{icon_size / 2.0f, icon_size / 2.0f};
static size_t curr{};

enum class TILE : unsigned int {
  GRASS_LT = 0,
  GRASS_T,
  GRASS_LR,
  PLAT_L,
  PLAT,
  PLAT_R,
  PLAT_LB,
  PLAT_B,
  PLAT_RB
};

bool bounding_box(v2 pos, const pair& p){
  return pos.x <= p.first.x + icon_size / 2.0f && pos.y >= p.first.x - icon_size / 2.0f &&
  pos.y <= p.first.y + icon_size / 2.0f && pos.y >= p.first.y - icon_size / 2.0f;
}

int get_hovered_tile() {
  auto it = std::find_if(std::execution::par_unseq, recs.begin(), recs.end(), [&](const auto& p){
    v2 pos = GetMousePosition();
    return bounding_box(pos, p);
  });
  return it == recs.end() ? -1 : std::distance(recs.begin(), it);
}

bool operator==(const Rectangle& lhs, const Rectangle& rhs)
{
  return lhs.x== rhs.x && lhs.y == rhs.y &&
  lhs.width == rhs.width &&
  lhs.width == rhs.width;
}

void init() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(400, 400, "tile editor");
  SetTargetFPS(60);
  img = LoadTexture("resources/tilesheet.png");
  int x{left_margin};
  int y{top_margin};
  int source_x{};
  int source_y{};
  std::for_each(std::execution::par, recs.begin(), recs.end(), [&](auto& p){
    p.second.x = source_x;
    p.second.y = source_y;
    p.second.width = tile_size;
    p.second.height = tile_size;
    p.first.x = x;
    p.first.y = y;
    p.first.width = icon_size;
    p.first.height = icon_size;
    x += step;
    source_x += tile_size;
    if (source_x >= max_row * tile_size) {
      source_x = 0;
      source_y += tile_size;
    }
    if (x >= max_row * icon_size) {
      x = left_margin;
      y+= step;
    }
  });
}
void draw_panel() {
  std::for_each(std::execution::par, recs.begin(), recs.end(), [&](const auto& rec){
    DrawTexturePro(img, rec.second, rec.first, orgn, .0f, WHITE);
    const auto i = get_hovered_tile();
    if (i >= 0 &&  recs[i] == rec) {
      DrawRectangle((int)rec.first.x - icon_size / 2.0f, (int)rec.first.y - icon_size / 2.0f,(int)icon_size, (int)icon_size, ColorAlpha(WHITE, .3f));
      curr = i;
    }
  });
}

void draw_ui() {
  draw_panel();
  for (size_t x = 0; x < map_size; x++) {
    for (size_t y = 0; y < map_size; y++) {
      if (tls[x][y])
        DrawTexturePro(img, recs[curr].second, Rectangle{x * icon_size, y * icon_size, 60, 60}, v2{30, 30}, .0f, WHITE);
    }
  }
}

void handle_input() {
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    int x = (int)(GetMouseX()) / icon_size;
    int y = (int)(GetMouseY()) / icon_size;
    tls[x][y] = 1;
  }
}

int main() {
  init();
  while(!WindowShouldClose()){
    handle_input();
    BeginDrawing();
    ClearBackground(WHITE);
    draw_ui();
    EndDrawing();
  }
  return 0;
}
