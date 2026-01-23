#include "mw_swapchain.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "mw_application.h"
#include "mw_core.h"
#include "mw_math.h"
#include "mw_render_queue.h"
VkSurfaceFormatKHR mw_choose_swap_surface_format(
    VkSurfaceFormatKHR* availableFormats, unsigned int availableFormatsCount) {
  for (size_t i = 0; i < availableFormatsCount; i++)
    if (availableFormats[i].format == VK_FORMAT_B8G8R8_SRGB &&
        availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return availableFormats[i];

  return availableFormats[0];
}

VkExtent2D mw_choose_swap_extent(VkSurfaceCapabilitiesKHR capabilities) {
  if (capabilities.currentExtent.width != UINT32_MAX)
    return capabilities.currentExtent;
  else {
    int width, height;
    mw_app_get_framebuffer_size(&width, &height);

    VkExtent2D actualExtent = {(unsigned int)width, (unsigned int)height};
    actualExtent.width =
        mw_clamp(actualExtent.width, capabilities.minImageExtent.width,
                 capabilities.maxImageExtent.width);
    actualExtent.height =
        mw_clamp(actualExtent.height, capabilities.minImageExtent.height,
                 capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

VkPresentModeKHR mw_choose_swap_present_mode(
    VkPresentModeKHR* availablePresentModes, unsigned int presentModesCount) {
  for (size_t i = 0; i < presentModesCount; i++)
    if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
      return availablePresentModes[i];

  return VK_PRESENT_MODE_FIFO_KHR;
}

mw_renderer_swapchain_support_details
mw_renderer_create_swapchain_support_details(VkPhysicalDevice device,
                                             VkSurfaceKHR surface) {
  mw_renderer_swapchain_support_details details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  unsigned int formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);
  details.formatsCount = formatCount;

  if (formatCount) {
    details.formats = malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats);
  }

  unsigned int presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            NULL);
  details.presentModesCount = presentModeCount;

  if (presentModeCount) {
    details.presentModes = malloc(presentModeCount * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes);
  }

  return details;
}

void mw_renderer_free_swapchain_support_details(
    mw_renderer_swapchain_support_details details) {
  free(details.formats);
  free(details.presentModes);
}

VkSwapchainKHR mw_renderer_create_swapchain(
    VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device,
    unsigned int* imageCount, VkImage** images, VkFormat* imageFormat,
    VkExtent2D* extent) {
  mw_renderer_swapchain_support_details details =
      mw_renderer_create_swapchain_support_details(physicalDevice, surface);

  VkSurfaceFormatKHR surfaceFormat =
      mw_choose_swap_surface_format(details.formats, details.formatsCount);
  VkPresentModeKHR presentMode = mw_choose_swap_present_mode(
      details.presentModes, details.presentModesCount);
  VkExtent2D ext = mw_choose_swap_extent(details.capabilities);

  *imageCount = details.capabilities.minImageCount + 1;
  if (details.capabilities.maxImageCount > 0 &&
      *imageCount > details.capabilities.maxImageCount) {
    *imageCount = details.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {0};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;

  createInfo.minImageCount = *imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageExtent = ext;

  struct mw_renderer_queue_family_indices indices =
      mw_renderer_find_queue_families(physicalDevice, surface);
  unsigned int queueFamilyIndices[] = {indices.graphicsFamily,
                                       indices.presentFamily};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
  }

  createInfo.preTransform = details.capabilities.currentTransform;
  if (details.capabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  } else {
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
  }

  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;

  createInfo.oldSwapchain = VK_NULL_HANDLE;

  VkSwapchainKHR swapchain;
  MW_ASSERT(vkCreateSwapchainKHR(device, &createInfo, NULL, &swapchain) ==
            VK_SUCCESS);

  vkGetSwapchainImagesKHR(device, swapchain, imageCount, NULL);
  *images = malloc(*imageCount * sizeof(VkImage));
  vkGetSwapchainImagesKHR(device, swapchain, imageCount, *images);

  *imageFormat = surfaceFormat.format;
  *extent = ext;

  mw_renderer_free_swapchain_support_details(details);

  return swapchain;
}

void mw_renderer_destroy_swapchain(VkDevice device, VkSwapchainKHR swapchain,
                                   VkImage* images) {
  vkDestroySwapchainKHR(device, swapchain, NULL);

  free(images);
}