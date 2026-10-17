#ifndef MW_ENGINE_DEF_H
#define MW_ENGINE_DEF_H

#include <GLFW/glfw3.h>
#include <flecs.h>
#include <mw_renderer.h>

#include "ecs/mw_camera.h"
#include "ecs/mw_ecs.h"

typedef struct mw_ctx {
  GLFWwindow* window;
  mw_renderer* renderer;
  ecs_world_t* ecs;
  uint64_t main_camera_entity;
  double cursor_x;
  double cursor_y;
  double last_cursor_x;
  double last_cursor_y;
} mw_ctx;

#endif