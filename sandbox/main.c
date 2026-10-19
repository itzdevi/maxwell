#include <maxwell.h>

vec3 rotation;
const float FLOOR_Y = -0.4;
int pressedJump = 0;

void camera_controller(mw_ctx* ctx, uint64_t entity, mw_comps* comps) {
  mw_position* pos = comps->ptrs[0];
  mw_rotation* rot = comps->ptrs[1];
  mw_velocity* vel = comps->ptrs[2];
  mw_camera* camera = comps->ptrs[3];

  mw_time time = mw_get_time(ctx);

  vec2 mouseDelta;
  mw_mouse_delta(ctx, mouseDelta);

  rotation[1] -= mouseDelta[0] * 0.0003;
  rotation[0] += mouseDelta[1] * 0.0003;
  rotation[0] = glm_clamp(rotation[0], glm_rad(-90), glm_rad(90));
  glm_euler_yxz_quat(rotation, *rot);

  vec3 forward = {0, 0, -1};
  glm_quat_rotatev(*rot, forward, forward);
  forward[1] = 0;
  glm_vec3_normalize(forward);

  vec3 right = {1, 0, 0};
  glm_quat_rotatev(*rot, right, right);

  vec2 inputVec;
  glm_vec2_zero(inputVec);
  if (mw_key_pressed(ctx, MW_KEY_W)) {
    inputVec[1] = 1;
  }
  if (mw_key_pressed(ctx, MW_KEY_S)) {
    inputVec[1] = -1;
  }
  if (mw_key_pressed(ctx, MW_KEY_A)) {
    inputVec[0] = -1;
  }
  if (mw_key_pressed(ctx, MW_KEY_D)) {
    inputVec[0] = 1;
  }
  glm_vec2_normalize(inputVec);

  glm_vec3_scale(forward, inputVec[1], forward);
  glm_vec3_scale(right, inputVec[0], right);

  vec3 moveVector;
  glm_vec3_add(forward, right, moveVector);
  glm_vec3_normalize(moveVector);
  (*vel)[0] = moveVector[0];
  (*vel)[2] = moveVector[2];

  if (mw_key_pressed(ctx, MW_KEY_SPACE)) {
    if (!pressedJump && (*pos)[1] == FLOOR_Y) {
      (*vel)[1] = -3;
      pressedJump = 1;
    }
  } else {
    pressedJump = 0;
  }

  if ((*pos)[1] <= FLOOR_Y) {
    (*vel)[1] += time.delta_time * 9.81;
  } else {
    (*vel)[1] = 0;
    (*pos)[1] = FLOOR_Y;
  }
}

int main() {
  mw_engine_info info = {.title = "Sandbox", .size = {1920 / 1, 1080 / 1}};
  mw_ctx* engine = mw_engine_create(info);

  uint64_t camComps[] = {MW_COMPONENT_POSITION, MW_COMPONENT_ROTATION,
                         MW_COMPONENT_VELOCITY, MW_COMPONENT_CAMERA};
  mw_engine_add_system(engine, "camera_controller", camera_controller, camComps,
                       4);

  uint64_t cam = mw_make_camera_entity(engine, 0.01, 1000, 60, (void*)0);
  mw_velocity* camVelocity =
      mw_engine_entity_add_component(engine, cam, MW_COMPONENT_VELOCITY);
  mw_engine_set_main_camera_entity(engine, cam);

  mw_position* camPos =
      mw_engine_get_component(engine, cam, MW_COMPONENT_POSITION);
  glm_vec3_copy((vec3){0, -0.4, 1}, *camPos);

  glm_vec3_zero(rotation);

  mw_load_glb(engine, "../assets/models/ABeautifulGame.glb");

  mw_cursor_mode(engine, MW_CURSOR_LOCKED);

  mw_engine_run(engine);
  mw_engine_destroy(engine);
}