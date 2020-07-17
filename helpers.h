#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
void endSingleTimeCommands(VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkDevice device, VkQueue graphicsQueue);
bool hasStencilComponent(VkFormat format);