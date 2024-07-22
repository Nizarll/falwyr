#include "../libs/tiles.h"
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <string.h>
#include <execution>
#include <iostream>
#include <iterator>
#include <raylib.h>
#include <raymath.h>
#include <unistd.h>
#include <utility>

static Texture2D img;
static Texture2D cursors;
static Camera2D camera{};
static Vector2 draw_pos{};

static bool show_menu = true;

constexpr auto red = Color{17, 23, 33};
constexpr auto icon_size = 60;
constexpr auto tile_size = 16;
constexpr auto left_margin= 40;
constexpr auto top_margin= 120;
constexpr auto max_row = 3;
constexpr auto step = icon_size + 8;
constexpr auto map_size = 256;
constexpr auto num_tiles = std::to_underlying(Tiles::LEN) - 1;

typedef Vector2 v2;
using pair = std::pair<Rectangle, Rectangle>;
using tiles = std::array<tile, map_size>;

std::array<pair, num_tiles> recs{};
std::array<tiles, map_size> tls{};

static v2 orgn{icon_size / 2.0f, icon_size / 2.0f};
static Rectangle Selector{.0f, .0f, 220.0f, 200.0f};

static size_t curr{};
static size_t curr_tool{};

enum class Tools : u8 {
  MOVE,
  PAINT,
  SELECT,
  SELECT_2,
  ERASE,
  LEN
};

struct tool {
  Rectangle rec;
  Rectangle pos;
};

std::array<tool, std::to_underlying(Tools::LEN)> tools{};

bool bounding_box(const v2 pos, const Rectangle& p){
  return pos.x <= p.x + p.width / 2.0f && pos.x >= p.x - p.width / 2.0f &&
  pos.y <= p.y + p.height / 2.0f && pos.y >= p.y - p.height / 2.0f;
}

bool bounding_box_vzero(const v2 pos, const Rectangle& p){
  return pos.x <= p.x + p.width && pos.x >= p.x &&
  pos.y <= p.y + p.height && pos.y >= p.y;
}


void save() {
  std::ofstream file_stream("map.bin", std::ios::trunc | std::ios::out);
  if (!file_stream.is_open()) {
    std::cerr << "Failed to open file for writing\n";
    return;
  }
  file_stream.write("{", 1);
  for (int i = 0; i < map_size; i++) {
    file_stream.write("{\n", strlen("{\n"));
    for (int j = 0; j < map_size; j++) {
      auto& tl = tls[i][j];
      std::string str_kind = std::to_string(tl.kind);
      file_stream.write("  tile{", strlen("  tile{"));
      file_stream.write(str_kind.c_str(), str_kind.size());
      if (j == (map_size - 1)) {
        file_stream.write("}", strlen("}"));
      } else {
        file_stream.write("},", strlen("},"));
      }
    }
    if (i == map_size - 1) {
      file_stream.write("\n}\n", strlen("\n}\n"));
    } else {
      file_stream.write("\n},\n", strlen("\n},\n"));
    }
  }
  file_stream.write("}", 1);
  file_stream.close();
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
  HideCursor();
  camera.zoom = 1.0f;
  camera.target = v2{2 * icon_size, (float)(map_size - 6) * icon_size};
  cursors = LoadTexture("resources/cursors.png");
  img = LoadTexture("resources/tilesheet.png");
  int x{left_margin}, source_x{};
  int y{top_margin}, source_y{};
  int tool_x{400}, tool_src{};
  for (auto& arr : tls)
  arr.fill(tile{(u16)Tiles::EMPTY, (u16)Shape::SQUARE});
  std::for_each(std::execution::par, tools.begin(), tools.end(), [&](auto& tool){
    tool.rec = Rectangle{tool_src, .0f, tile_size,tile_size};
    tool.pos = Rectangle{tool_x, 60.0f,icon_size,icon_size};
    tool_src += tile_size;
    tool_x += 60.0f + 40.0f;
  });
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
  const auto i = get_hovered_tile();
  std::for_each(std::execution::par, recs.begin(), recs.end(), [&](const auto& rec){
    DrawTexturePro(img, rec.second, rec.first, v2{icon_size / 2.0f, icon_size / 2.0f}, .0f, WHITE);
    if (i >= 0 &&  recs[i] == rec) {
      DrawRectangle((int)rec.first.x - icon_size / 2.0f, (int)rec.first.y - icon_size / 2.0f,(int)icon_size, (int)icon_size, ColorAlpha(WHITE, .3f));
    }
  });
  std::for_each(std::execution::par, tools.begin(), tools.end(), [&](const auto& tool){
    DrawTexturePro(cursors, tool.rec, tool.pos, v2{icon_size / 2.0f, icon_size / 2.0f}, .0f, WHITE);
  });
}

void draw_ui() {
  BeginMode2D(camera);
  for (size_t x = 0; x < map_size; x++) {
    for (size_t y = 0; y < map_size; y++) {
      if (tls[x][y].kind != (u16)Tiles::EMPTY) {
        size_t kind = tls[x][y].kind;
        DrawTexturePro(img, recs[kind].second, Rectangle{x * icon_size, y * icon_size, icon_size, icon_size}, v2{0, 0}, .0f, WHITE);
      }
    }
  }
  EndMode2D();
  if(!show_menu) return;
  DrawRectangleRec(Selector, ColorAlpha(BLACK, .45f));
  draw_panel();
}

void draw_cursor() {
  DrawTexturePro(cursors, tools[curr_tool].rec, Rectangle{GetMouseX(), GetMouseY(), icon_size, icon_size}, v2{icon_size/ 2.0f, icon_size/2.0f}, .0f, WHITE);
}

void handle_input() {
  f32 mouse_x = GetMouseX();
  f32 mouse_y = GetMouseY();
  draw_pos = GetScreenToWorld2D(GetMousePosition(), camera);
  if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) return;
  for (size_t i = 0; i < tools.size(); i++) {
    if (bounding_box(v2{mouse_x, mouse_y}, Rectangle{
      tools[i].pos.x, tools[i].pos.y, tools[i].pos.width + left_margin, tools[i].pos.height + top_margin}
                     )) {
      curr_tool = i;
      return;
    }
  }
  if (bounding_box_vzero(v2{mouse_x, mouse_y}, Selector)) {
    const auto i = get_hovered_tile();
    if(i < 0) return;
    curr = i;
    curr_tool = (u16) Tools::PAINT;
    return;
  }
  if (curr_tool == (u16)Tools::MOVE) {
    v2 delta = GetMouseDelta();
    //delta = Vector2Scale(delta, -1.0f/camera.scale);
    camera.target = Vector2Subtract(camera.target, delta);
    return;
  }
  int x = std::floor( draw_pos.x / icon_size);
  int y = std::floor( draw_pos.y / icon_size);
  if (x < 0 || x>= map_size || y >= map_size || y < 0) return;
  if (curr_tool == (u16)Tools::PAINT) {
    v2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);
    tls[x][y].shape = (u16) Shape::SQUARE;
    tls[x][y].kind  = (u16) curr;
    return;
  };
  if (curr_tool == (u16)Tools::ERASE) {
    tls[x][y].shape = (u16) Shape::SQUARE;
    tls[x][y].kind  = (u16) Tiles::EMPTY;
    return;
  };
}

void handle_shortcuts() {
  if (IsKeyPressed(KEY_D)) {
    curr_tool = (u16) Tools::PAINT;
    return;
  }
  if (IsKeyPressed(KEY_H)) {
    show_menu = !show_menu;
    return;
  }
  if (IsKeyPressed(KEY_V)) {
    curr_tool = (u16) Tools::MOVE;
    return;
  }
  if (IsKeyPressed(KEY_E)){
    curr_tool = (u16) Tools::ERASE;
    return;
  }
  if (IsKeyPressed(KEY_S)) {
    save();
    return;
  }
}

int main() {
  init();
  while(!WindowShouldClose()){
    Selector.height = GetScreenHeight();
    handle_shortcuts();
    handle_input();
    BeginDrawing();
    ClearBackground(red);
    draw_ui();
    draw_cursor();
    EndDrawing();
  }
  return 0;
}
