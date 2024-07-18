#include "../libs/entity.h"

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

void entity::draw() {
  let& anim= *anims[curr];
  DrawTexturePro(anim.sht.t2d,
                 Rectangle{anim._curr * anim.spr_size.x, .0f, anim.spr_size.x, anim.spr_size.y},
                 Rectangle{200.0f, 200.0f, anim.size.x,anim.size.y},
                 v2{anim.size.x / 2.0f, anim.size.y / 2.0f}, 
                 .0f,
                 WHITE);
}
