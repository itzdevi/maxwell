#ifndef MW_ENGINE_DEF_H
#define MW_ENGINE_DEF_H

#include <GLFW/glfw3.h>
#include <flecs.h>
#include <mw_renderer.h>

typedef struct mw_ctx {
  GLFWwindow* window;
  mw_renderer* renderer;
  ecs_world_t* ecs;
} mw_ctx;

#endif