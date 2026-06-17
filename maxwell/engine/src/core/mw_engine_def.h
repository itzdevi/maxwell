#ifndef MW_ENGINE_DEF_H
#define MW_ENGINE_DEF_H

#include <GLFW/glfw3.h>
#include <flecs.h>
#include <mw_renderer.h>

#include "ecs/mw_camera.h"

typedef struct mw_ctx {
  GLFWwindow* window;
  mw_renderer* renderer;
  ecs_world_t* ecs;
  uint64_t main_camera_entity;
} mw_ctx;

#endif