#include "mw_instance.h"

#include "mw_application.h"
#include "mw_core.h"

VkInstance mw_renderer_create_instance(const char* title) {
  VkApplicationInfo appInfo = {0};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = title;
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Maxwell";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo = {0};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  unsigned int extensionCount = 0;
  const char** extensions = mw_app_get_extensions(&extensionCount);

  createInfo.enabledExtensionCount = extensionCount;
  createInfo.ppEnabledExtensionNames = extensions;

  createInfo.enabledLayerCount = 0;

  VkInstance instance;
  MW_ASSERT(vkCreateInstance(&createInfo, NULL, &instance) == VK_SUCCESS);

  return instance;
}

void mw_renderer_destroy_instance(VkInstance instance) {
  vkDestroyInstance(instance, NULL);
}