#include "mw_device.h"

#include <string.h>

#include "mw_core.h"
#include "mw_physical_device.h"
#include "mw_render_queue.h"

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

VkDevice mw_renderer_create_device(VkPhysicalDevice physicalDevice,
                                   VkSurfaceKHR surface, VkQueue* presentQueue,
                                   VkQueue* graphicsQueue) {
  struct QueueFamilyIndices indices =
      mw_renderer_find_queue_families(physicalDevice, surface);

  float queuePriority = 1.0f;

  uint32_t uniqueQueueFamilies[2];
  uint32_t queueCount = 0;

  uniqueQueueFamilies[queueCount++] = indices.graphicsFamily;
  if (indices.presentFamily != indices.graphicsFamily)
    uniqueQueueFamilies[queueCount++] = indices.presentFamily;

  VkDeviceQueueCreateInfo queueCreateInfos[2] = {0};
  for (uint32_t i = 0; i < queueCount; i++) {
    queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfos[i].queueFamilyIndex = uniqueQueueFamilies[i];
    queueCreateInfos[i].queueCount = 1;
    queueCreateInfos[i].pQueuePriorities = &queuePriority;
  }

  VkDeviceCreateInfo createInfo = {0};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = queueCount;
  createInfo.pQueueCreateInfos = queueCreateInfos;

  VkPhysicalDeviceFeatures deviceFeatures = {0};
  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);
  createInfo.ppEnabledExtensionNames = deviceExtensions;

  VkDevice device;
  MW_ASSERT(vkCreateDevice(physicalDevice, &createInfo, NULL, &device) ==
            VK_SUCCESS);

  vkGetDeviceQueue(device, indices.presentFamily, 0, presentQueue);
  vkGetDeviceQueue(device, indices.graphicsFamily, 0, graphicsQueue);
  return device;
}

void mw_renderer_destroy_device(VkDevice device) {
  vkDestroyDevice(device, NULL);
}