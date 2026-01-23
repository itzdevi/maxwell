#include "mw_device.h"

#include <string.h>

const char* deviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

int mw_renderer_check_device_extension_support(VkPhysicalDevice device) {
  unsigned int extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

  VkExtensionProperties availableExtensions[extensionCount];
  vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount,
                                       availableExtensions);

  int requiredExtensionsCount =
      sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);
  int usedFlags[requiredExtensionsCount];
  memset(usedFlags, 0, requiredExtensionsCount * sizeof(int));

  for (size_t i = 0; i < extensionCount; i++) {
    char* extensionName = availableExtensions[i].extensionName;
    int extensionNameLen = strlen(extensionName);
    for (size_t j = 0; j < requiredExtensionsCount; j++) {
      if (!strcmp(extensionName, deviceExtensions[j])) {
        usedFlags[j] = 1;
      }
    }
  }

  for (size_t i = 0; i < requiredExtensionsCount; i++) {
    if (!usedFlags[i]) return 0;
  }

  return 1;
}