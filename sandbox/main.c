#include <maxwell.h>

vec3 rotation;
float zoomProgress = 0;

void camera_controller(mw_iter* it) {
  mw_position* pos = mw_engine_get_field(it, sizeof(mw_position), 0);
  mw_rotation* rot = mw_engine_get_field(it, sizeof(mw_rotation), 1);
  mw_camera* camera = mw_engine_get_field(it, sizeof(mw_camera), 2);

  mw_time time = mw_get_time(it);

  vec2 mouseDelta;
  mw_mouse_delta(it, mouseDelta);

  for (int i = 0; i < mw_engine_get_iter_count(it); i++) {
    rotation[1] -= mouseDelta[0] * 0.0003;
    rotation[0] += mouseDelta[1] * 0.0003;
    rotation[0] = glm_clamp(rotation[0], glm_rad(-90), glm_rad(90));
    glm_euler_yxz_quat(rotation, rot[i]);

    vec2 velocityHorizontal;
    glm_vec2_zero(velocityHorizontal);

    if (mw_key_pressed(it, MW_KEY_W)) {
      velocityHorizontal[1] = -1;
    }
    if (mw_key_pressed(it, MW_KEY_S)) {
      velocityHorizontal[1] = 1;
    }
    if (mw_key_pressed(it, MW_KEY_A)) {
      velocityHorizontal[0] = -1;
    }
    if (mw_key_pressed(it, MW_KEY_D)) {
      velocityHorizontal[0] = 1;
    }

    if (mw_key_pressed(it, MW_KEY_C)) {
      zoomProgress += time.delta_time * 7;
    } else {
      zoomProgress -= time.delta_time * 7;
    }
    zoomProgress = glm_clamp_zo(zoomProgress);

    camera[i].fov = glm_lerp(60, 10, glm_ease_cubic_inout(zoomProgress));

    vec3 forward = {0, 0, 1};
    glm_quat_rotatev(rot[i], forward, forward);

    vec3 forwardHorizontal;
    glm_vec3_copy(forward, forwardHorizontal);
    forwardHorizontal[1] = 0;
    glm_vec3_normalize(forwardHorizontal);

    vec3 right = {1, 0, 0};
    glm_quat_rotatev(rot[i], right, right);

    glm_vec2_normalize(velocityHorizontal);
    glm_vec2_scale(velocityHorizontal, time.delta_time, velocityHorizontal);

    vec3 velz;
    glm_vec3_scale(forwardHorizontal, velocityHorizontal[1], velz);
    glm_vec3_add(pos[i], velz, pos[i]);

    vec3 velx;
    glm_vec3_scale(right, velocityHorizontal[0], velx);
    glm_vec3_add(pos[i], velx, pos[i]);
  }
}

int main() {
  mw_engine_info info = {.title = "Sandbox", .size = {1920 / 1.5, 1080 / 1.5}};
  mw_ctx* engine = mw_engine_create(info);

  uint64_t camComps[] = {MW_POSITION, MW_ROTATION, MW_CAMERA};
  mw_engine_add_system(engine, "CameraController", camera_controller, camComps,
                       3);

  uint64_t cam = mw_engine_create_entity(engine);
  mw_build_transform_no_scale(engine, cam);

  mw_camera* camComp = mw_build_camera(engine, cam, 0.01, 1000, 60);

  mw_engine_set_main_camera_entity(engine, cam);

  mw_position* camPos = mw_engine_get_component(engine, cam, MW_POSITION);
  glm_vec3_copy((vec3){0, -0.4, 1}, *camPos);

  glm_vec3_zero(rotation);

  mw_load_glb(engine, "../assets/models/ABeautifulGame.glb");

  mw_cursor_mode(engine, MW_CURSOR_LOCKED);

  mw_engine_run(engine);
  mw_engine_destroy(engine);
}