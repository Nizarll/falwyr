#include "../libs/tiles.h"
#include <vector>

//struct tile {
//  shape s;
//  v2 data;
//  u16 kind;
//  tile_var var;
//  void draw();
//  void switchbox();
//};

void tile::init() {
  atlas = LoadTexture("resources/tilesheet.png");
};

void tile::draw(v2 pos) {
  DrawTexturePro(atlas, 
                Rectangle{data.x, data.y, size.x, size.y},
                Rectangle{150.0f, 280.0f, 80.0f, 80.0f},
                v2{8.0f, 8.0f}, 
                .0f,WHITE);

}
