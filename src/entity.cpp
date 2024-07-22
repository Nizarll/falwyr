#include "../libs/entity.h"
#include <exception>
#include <iostream>
#include <stdexcept>

void entity::update(f32 dt) {
  let& anim = *anims[curr];
  anim.last += dt;
  if (anim.last < 1.0f / (std::fabs(anim.speed) + 1)) {
    return;
  }
  anim.last = .0f;
  if (anim._curr >= (anim.len - 1)) {
    anim._curr = 0;
    return;
  }
  anim._curr += 1;
  return;
}

entity::entity(std::vector<anim*>& anims, v2 size, v2 pos) {
  phbody body = CreatePhysicsBodyRectangle(pos, size.x, size.y, 10);
  body->enabled = true;
  anims = anims;
  body = body;
  curr = 0;
}

void entity::draw() {
  if (anims[curr] == nullptr){
    std::cout << "bruh nullptr";
    return;
  }
  let& anim= *anims[curr];
  DrawTexturePro(anim.sht.t2d,
                 Rectangle{anim._curr * anim.spr_size.x, .0f, anim.spr_size.x, anim.spr_size.y},
                 Rectangle{body->position.x, body->position.y, anim.size.x,anim.size.y},
                 v2{anim.size.x / 2.0f, anim.size.y / 2.0f}, 
                 .0f,
                 WHITE);
}
