#include <maxwell.h>

void camera_controller(mw_iter* it) {
  mw_position* pos = mw_engine_get_field(it, sizeof(mw_position), 0);
  mw_rotation* rot = mw_engine_get_field(it, sizeof(mw_rotation), 1);
  mw_camera* camera = mw_engine_get_field(it, sizeof(mw_camera), 2);

  mw_time time = mw_get_time(it);

  for (int i = 0; i < mw_engine_get_iter_count(it); i++) {
    glm_vec3_copy((vec3){0, sin(time.time) * 0.2, -1.2}, pos[i]);
    glm_euler_xyz_quat(
        (vec3){glm_rad(0.0), glm_rad(sin(time.time / 2) * 20), glm_rad(0.0)},
        rot[i]);
  }
}

int main() {
  mw_engine_info info = {.title = "Sandbox", .size = {1800, 1200}};
  mw_ctx* engine = mw_engine_create(info);

  uint64_t camComps[] = {MW_POSITION, MW_ROTATION, MW_CAMERA};
  mw_engine_add_system(engine, "CameraController", camera_controller, camComps,
                       3);

  uint64_t cam = mw_engine_create_entity(engine);
  mw_build_transform_no_scale(engine, cam);

  mw_camera* camComp = mw_build_camera(engine, cam, 0.1, 1000, 60);

  mw_engine_set_main_camera_entity(engine, cam);

  mw_load_glb(engine, "../assets/models/ABeautifulGame.glb");

  mw_engine_run(engine);
  mw_engine_destroy(engine);
}