#include <maxwell.h>

void camera_controller(mw_iter* it) {
  const mw_position* pos = mw_engine_get_field(it, sizeof(mw_position), 0);
  const mw_camera* camera = mw_engine_get_field(it, sizeof(mw_camera), 1);

  for (int i = 0; i < mw_engine_get_iter_count(it); i++) {
  }
}

int main() {
  mw_engine_info info = {.title = "Sandbox", .size = {800, 600}};
  mw_ctx* engine = mw_engine_create(info);

  uint64_t camComps[] = {MW_POSITION, MW_CAMERA};
  mw_engine_add_system(engine, "CameraController", camera_controller, camComps,
                       2);
  uint64_t meshComps[] = {MW_POSITION, MW_MESH_RENDERER};

  uint64_t cam = mw_engine_create_entity(engine);
  mw_position* camPos =
      (mw_position*)mw_engine_entity_add_component(engine, cam, MW_POSITION);
  glm_vec3_copy((vec3){0, 0, 10}, camPos->data);

  mw_camera* camComp =
      (mw_camera*)mw_engine_entity_add_component(engine, cam, MW_CAMERA);
  camComp->near_plane = 0.1;
  camComp->far_plane = 1000;
  camComp->fov = 60;

  mw_engine_set_main_camera_entity(engine, cam);

  mw_load_glb(engine, "../assets/models/ABeautifulGame.glb");

  uint64_t piece = mw_engine_create_entity(engine);
  mw_position* piecePos =
      (mw_position*)mw_engine_entity_add_component(engine, piece, MW_POSITION);
  glm_vec3_copy((vec3){0, 0, 0}, piecePos->data);

  mw_rotation* pieceRot =
      (mw_rotation*)mw_engine_entity_add_component(engine, piece, MW_ROTATION);
  glm_vec3_copy((vec3){0, 0, 0}, pieceRot->data);

  mw_scale* pieceScale =
      (mw_scale*)mw_engine_entity_add_component(engine, piece, MW_SCALE);
  glm_vec3_copy((vec3){30, 30, 30}, pieceScale->data);

  mw_mesh_renderer* pieceMR = (mw_mesh_renderer*)mw_engine_entity_add_component(
      engine, piece, MW_MESH_RENDERER);
  pieceMR->mesh_id = 5;

  mw_engine_run(engine);
  mw_engine_destroy(engine);
}