#include "../libs/tiles.h"
#include <cmath>
#include <algorithm>
#include <execution>
#include <iostream>
#include <iterator>

static Texture2D img;
static auto icon_size = 60;

constexpr auto tile_size = 16;
constexpr auto num_tiles = 9;
constexpr auto left_margin= 30;
constexpr auto top_margin= 120;
constexpr auto max_row = 3;
constexpr auto step = 60 + 8;
constexpr auto map_size = 1024;

typedef Vector2 v2;
using pair = std::pair<Rectangle, Rectangle>;
using tiles = std::array<tile, map_size>;

std::array<pair, num_tiles> recs{};
std::array<tiles, map_size> tls{};

static v2 orgn{icon_size / 2.0f, icon_size / 2.0f};
static size_t curr{};

bool bounding_box(const v2 pos, const Rectangle& p){
  return pos.x <= p.x + p.width / 2.0f && pos.y >= p.x - p.width / 2.0f &&
  pos.y <= p.y + p.height / 2.0f && pos.y >= p.y - p.height / 2.0f;
}

int get_hovered_tile() {
  auto it = std::find_if(std::execution::par_unseq, recs.begin(), recs.end(), [&](const auto& p){
    v2 pos = GetMousePosition();
    return bounding_box(pos, p.first);
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
  int x{left_margin}, source_x{};
  int y{top_margin}, source_y{};
  for (auto& arr : tls)
    arr.fill(tile{(u16)Tiles::EMPTY, (u16)Shape::SQUARE, v2{}, v2{}});
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
    const auto i = get_hovered_tile();
    if (i >= 0 &&  recs[i] == rec) {
      DrawRectangle((int)rec.first.x - icon_size / 2.0f, (int)rec.first.y - icon_size / 2.0f,(int)icon_size, (int)icon_size, ColorAlpha(WHITE, .3f));
      curr = i;
    }
  });
}

void draw_ui() {
  draw_panel();
  icon_size = 60.0f * GetScreenWidth() / 1920;
  for (size_t x = 0; x < map_size; x++) {
    for (size_t y = 0; y < map_size; y++) {
      if (tls[x][y].kind != (u16)Tiles::EMPTY) {
        size_t kind = tls[x][y].kind;
        DrawTexturePro(img, recs[kind].second, Rectangle{x * 60, y * 60, 60, 60}, v2{0, 0}, .0f, WHITE);
      }
    }
  }
}

void handle_input() {
  f32 mouse_x = GetMouseX();
  f32 mouse_y = GetMouseY();
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    int x = std::floor(mouse_x / icon_size);
    int y = std::floor(mouse_y / icon_size);
    std::cout << "position : " << x * 60 << ", " << y * 60 << "\n";
    std::cout << "mouse position : " << GetMouseX() << ", " << GetMouseY() << "\n";
    tls[x][y].shape = (u16) Shape::SQUARE;
    tls[x][y].kind  = (u16) curr;
  }
}

int main() {
  init();
  while(!WindowShouldClose()){
    handle_input();
    BeginDrawing();
    ClearBackground(WHITE);
    DrawRectangleRec(Rectangle{.0f, .0f, GetScreenWidth() / 4, GetScreenHeight()}, ColorAlpha(RED, .1f));
    draw_ui();
    EndDrawing();
  }
  return 0;
}
