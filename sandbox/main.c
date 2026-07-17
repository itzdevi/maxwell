#include <maxwell.h>

void camera_controller(mw_iter* it) {
  const mw_position* pos = mw_engine_get_field(it, sizeof(mw_position), 0);
  const mw_camera* camera = mw_engine_get_field(it, sizeof(mw_camera), 1);

  for (int i = 0; i < mw_engine_get_iter_count(it); i++) {
  }
}

int main() {
  mw_engine_info info = {.title = "Sandbox", .size = {1800, 1200}};
  mw_ctx* engine = mw_engine_create(info);

  uint64_t camComps[] = {MW_POSITION, MW_CAMERA};
  mw_engine_add_system(engine, "CameraController", camera_controller, camComps,
                       2);
  uint64_t meshComps[] = {MW_POSITION, MW_MESH_RENDERER};

  uint64_t cam = mw_engine_create_entity(engine);
  mw_position* camPos =
      mw_engine_entity_add_component(engine, cam, MW_POSITION);
  glm_vec3_copy((vec3){0, -0.6, 0.8}, *camPos);

  mw_camera* camComp =
      (mw_camera*)mw_engine_entity_add_component(engine, cam, MW_CAMERA);
  camComp->near_plane = 0.1;
  camComp->far_plane = 1000;
  camComp->fov = 60;

  mw_engine_set_main_camera_entity(engine, cam);

  mw_load_glb(engine, "../assets/models/ABeautifulGame.glb");

  mw_engine_run(engine);
  mw_engine_destroy(engine);
}