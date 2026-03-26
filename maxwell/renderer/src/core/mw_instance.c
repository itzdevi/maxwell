#include "mw_instance.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mw_application.h"
#include "mw_assert.h"

const char* validationLayers[] = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
int enableValidationLayers = 0;
#else
int enableValidationLayers = 1;
#endif

static VkDebugUtilsMessengerEXT debugMessenger;

static int mw_renderer_check_validation_layer_support() {
  unsigned int layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, NULL);

  VkLayerProperties* availableLayers =
      malloc(layerCount * sizeof(VkLayerProperties));
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  for (int i = 0; i < sizeof(validationLayers) / sizeof(validationLayers[0]);
       i++) {
    int layerFound = 0;
    for (int j = 0; j < layerCount; j++) {
      if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
        layerFound = 1;
        break;
      }
    }

    if (!layerFound) {
      free(availableLayers);
      return 0;
    }
  }

  free(availableLayers);
  return 1;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData) {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    fprintf(stderr, "validation layer: %s\n", pCallbackData->pMessage);
  }

  return VK_FALSE;
}

static void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT* createInfo) {
  createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo->messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo->pfnUserCallback = debugCallback;
  createInfo->pUserData = NULL;
}

VkInstance mw_renderer_create_instance(const char* title) {
  if (enableValidationLayers && !mw_renderer_check_validation_layer_support()) {
    printf(
        "Warning: Validation layers requested, but not available! "
        "Disabling validation layers.\n");
    enableValidationLayers = 0;
  }

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

  unsigned int glfwExtensionCount = 0;
  const char** glfwExtensions = mw_app_get_extensions(&glfwExtensionCount);

  unsigned int extensionCount = glfwExtensionCount;
  if (enableValidationLayers) {
    extensionCount += 1;
  }

  const char** extensions = malloc(extensionCount * sizeof(const char*));
  for (int i = 0; i < glfwExtensionCount; ++i) {
    extensions[i] = glfwExtensions[i];
  }

  if (enableValidationLayers) {
    extensions[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
  }

  createInfo.enabledExtensionCount = extensionCount;
  createInfo.ppEnabledExtensionNames = extensions;

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        sizeof(validationLayers) / sizeof(validationLayers[0]);
    createInfo.ppEnabledLayerNames = validationLayers;
    populateDebugMessengerCreateInfo(&debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = NULL;
  }

  VkInstance instance;
  MW_ASSERT(vkCreateInstance(&createInfo, NULL, &instance) == VK_SUCCESS);

  free(extensions);

  if (enableValidationLayers) {
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {0};
    populateDebugMessengerCreateInfo(&messengerCreateInfo);

    PFN_vkCreateDebugUtilsMessengerEXT func =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT");
    MW_ASSERT(func != NULL);
    MW_ASSERT(func(instance, &messengerCreateInfo, NULL, &debugMessenger) ==
              VK_SUCCESS);
  }

  return instance;
}

void mw_renderer_destroy_instance(VkInstance instance) {
  if (enableValidationLayers) {
    PFN_vkDestroyDebugUtilsMessengerEXT func =
        (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
      func(instance, debugMessenger, NULL);
    }
  }
  vkDestroyInstance(instance, NULL);
}