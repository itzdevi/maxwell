#include "mw_swapchain.h"

#include <cglm/cglm.h>
#include <limits.h>
#include <mw_core.h>
#include <stdio.h>
#include <stdlib.h>

#include "core/mw_renderer_def.h"
#include "mw_framebuffer.h"
#include "resources/mw_buffer.h"

VkSurfaceFormatKHR mw_choose_swap_surface_format(
    VkSurfaceFormatKHR* availableFormats, unsigned int availableFormatsCount) {
  for (size_t i = 0; i < availableFormatsCount; i++)
    if (availableFormats[i].format == VK_FORMAT_B8G8R8_SRGB &&
        availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return availableFormats[i];

  return availableFormats[0];
}

VkExtent2D mw_choose_swap_extent(VkSurfaceCapabilitiesKHR capabilities,
                                 vec2 framebufferSize) {
  if (capabilities.currentExtent.width != UINT32_MAX)
    return capabilities.currentExtent;
  else {
    VkExtent2D actualExtent = {(unsigned int)framebufferSize[0],
                               (unsigned int)framebufferSize[1]};
    actualExtent.width =
        glm_clamp(actualExtent.width, capabilities.minImageExtent.width,
                  capabilities.maxImageExtent.width);
    actualExtent.height =
        glm_clamp(actualExtent.height, capabilities.minImageExtent.height,
                  capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

VkPresentModeKHR mw_choose_swap_present_mode(
    VkPresentModeKHR* availablePresentModes, unsigned int presentModesCount) {
  for (size_t i = 0; i < presentModesCount; i++)
    if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentModes[i];
    }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkImageView* mw_create_image_views(VkDevice device, VkFormat imageFormat,
                                   int imageCount, VkImage* images) {
  VkImageView* imageViews = malloc(imageCount * sizeof(VkImageView));

  for (size_t i = 0; i < imageCount; i++) {
    VkImageViewCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = images[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = imageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    MW_ASSERT(vkCreateImageView(device, &createInfo, NULL, &imageViews[i]) ==
              VK_SUCCESS);
  }

  return imageViews;
}

void mw_renderer_create_swapchain(mw_renderer* renderer) {
  mw_swapchain_support_details details =
      mw_gpu_create_swapchain_support_details(renderer->gpu.physicalDevice,
                                              renderer->surface);

  VkSurfaceFormatKHR surfaceFormat =
      mw_choose_swap_surface_format(details.formats, details.formatsCount);
  VkPresentModeKHR presentMode = mw_choose_swap_present_mode(
      details.presentModes, details.presentModesCount);
  VkExtent2D extent =
      mw_choose_swap_extent(details.capabilities, renderer->framebuffer_size);

  unsigned int imageCount = details.capabilities.minImageCount + 1;
  if (details.capabilities.maxImageCount > 0 &&
      imageCount > details.capabilities.maxImageCount) {
    imageCount = details.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo = {0};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = renderer->surface;

  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageExtent = extent;

  struct mw_queue_family_indices indices = mw_gpu_find_queue_families(
      renderer->gpu.physicalDevice, renderer->surface);
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

  if (renderer->swapchain.swapchain)
    createInfo.oldSwapchain = renderer->swapchain.swapchain;
  else
    createInfo.oldSwapchain = VK_NULL_HANDLE;

  VkSwapchainKHR swapchain;
  MW_ASSERT(vkCreateSwapchainKHR(renderer->gpu.device.device, &createInfo, NULL,
                                 &swapchain) == VK_SUCCESS);

  if (renderer->swapchain.swapchain) mw_renderer_destroy_swapchain(renderer);

  vkGetSwapchainImagesKHR(renderer->gpu.device.device, swapchain, &imageCount,
                          NULL);
  VkImage* images = malloc(imageCount * sizeof(VkImage));
  vkGetSwapchainImagesKHR(renderer->gpu.device.device, swapchain, &imageCount,
                          images);

  VkFormat imageFormat = surfaceFormat.format;
  VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;

  mw_swapchain sc;
  sc.swapchain = swapchain;
  sc.image_count = imageCount;
  sc.images = images;
  sc.image_format = imageFormat;
  sc.extent = extent;
  sc.image_views = mw_create_image_views(renderer->gpu.device.device,
                                         imageFormat, imageCount, images);

  VkImageCreateInfo depthBufferCreateInfo = {0};
  depthBufferCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  depthBufferCreateInfo.imageType = VK_IMAGE_TYPE_2D;
  depthBufferCreateInfo.format = depthFormat;
  depthBufferCreateInfo.mipLevels = 1;
  depthBufferCreateInfo.arrayLayers = 1;
  depthBufferCreateInfo.extent.width = extent.width;
  depthBufferCreateInfo.extent.height = extent.height;
  depthBufferCreateInfo.extent.depth = 1;
  depthBufferCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  depthBufferCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthBufferCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  depthBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  depthBufferCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

  vkCreateImage(renderer->gpu.device.device, &depthBufferCreateInfo, NULL,
                &sc.depth_image);

  VkMemoryRequirements memReq;
  vkGetImageMemoryRequirements(renderer->gpu.device.device, sc.depth_image,
                               &memReq);

  VkMemoryAllocateInfo allocInfo = {0};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memReq.size;
  allocInfo.memoryTypeIndex = mw_renderer_find_memory_type(
      renderer->gpu.physicalDevice, memReq.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  vkAllocateMemory(renderer->gpu.device.device, &allocInfo, NULL,
                   &sc.depth_image_memory);
  vkBindImageMemory(renderer->gpu.device.device, sc.depth_image,
                    sc.depth_image_memory, 0);

  VkImageViewCreateInfo depthViewInfo = {0};
  depthViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  depthViewInfo.image = sc.depth_image;
  depthViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  depthViewInfo.format = depthFormat;
  depthViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  depthViewInfo.subresourceRange.baseMipLevel = 0;
  depthViewInfo.subresourceRange.levelCount = 1;
  depthViewInfo.subresourceRange.baseArrayLayer = 0;
  depthViewInfo.subresourceRange.layerCount = 1;

  vkCreateImageView(renderer->gpu.device.device, &depthViewInfo, NULL,
                    &sc.depth_image_view);

  sc.depth_format = depthFormat;

  mw_gpu_free_swapchain_support_details(details);
  renderer->swapchain = sc;
}

void mw_renderer_destroy_swapchain(mw_renderer* renderer) {
  vkDestroySwapchainKHR(renderer->gpu.device.device,
                        renderer->swapchain.swapchain, NULL);

  free(renderer->swapchain.images);

  for (size_t i = 0; i < renderer->swapchain.image_count; i++)
    vkDestroyImageView(renderer->gpu.device.device,
                       renderer->swapchain.image_views[i], NULL);

  vkDestroyImage(renderer->gpu.device.device, renderer->swapchain.depth_image,
                 NULL);
  vkDestroyImageView(renderer->gpu.device.device,
                     renderer->swapchain.depth_image_view, NULL);
  vkFreeMemory(renderer->gpu.device.device,
               renderer->swapchain.depth_image_memory, NULL);
  free(renderer->swapchain.image_views);
}
