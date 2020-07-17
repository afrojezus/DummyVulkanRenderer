// Shimakaze.h : Include file for standard system include files,
// or project specific include files.

// Much of the code is based off the Vulkan tutorial https://vulkan-tutorial.com/
// This project was mostly made for learning the SDK.

// TODO: REFACTOR THE LIVING SHIT OUT OF THIS HEADER.
// Please save me from this hell.

#pragma once

#include "helpers.h"
#include "uniform_buffer_object.h"
#include "vertex.h"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// c++ std libs
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// constants
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;
const int MAX_FRAMES_IN_FLIGHT = 2;

const char* GAME_TITLE = "Shimakaze";
const char* ENGINE_TITLE = "ShimakazeEngine";
const int MAJOR_VERSION_NUMBER = 1;
const int MINOR_VERSION_NUMBER = 0;
const int PATCH_VERSION_NUMBER = 0;

const std::string MODEL_PATH = "models/cube.obj";
const std::string TEXTURE_PATH = "textures/guy.gif";

// validation layers for Vulkan
const std::vector<const char*> validationLayers{ "VK_LAYER_KHRONOS_validation" };

// device extensions for Vulkan
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// vkDebugUtilsMessengerEXT
VkResult CreateDebugUtilsMessengerEXT(
	VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
	const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
		instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger,
	const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
		instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
};

// structs
struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// arrays
VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT,
								  VK_DYNAMIC_STATE_LINE_WIDTH };

// helper functions
static std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file!");
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

// main entry class of the application
// all of the vulkan shit should be sent to its own damn class oh my god
class Shimakaze {
	public:
	void run();

	private:
	// Define application for Vulkan
	VkApplicationInfo appInfo{};
	// Vulkan instance
	VkInstance instance;
	// Debug messenger for Vulkan
	VkDebugUtilsMessengerEXT debugMessenger;
	// Physical device (graphics card)
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	// Render window output
	GLFWwindow* window;
	// Logical device for Vulkan
	VkDevice device;
	// Graphics queue
	VkQueue graphicsQueue;
	// Surface for the window
	VkSurfaceKHR surface;
	// Present queue
	VkQueue presentQueue;
	// Swapchain
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	// Graphics pipeline
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	// Framebuffers
	std::vector<VkFramebuffer> swapChainFramebuffers;
	// Command pools & buffers
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	// Semaphores, sync objects
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame{ 0 };
	// Vertex
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VkBuffer vertexBuffer;
	VkMemoryRequirements memRequirements;
	VkDeviceMemory vertexBufferMemory;
	// Index
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	bool framebufferResized = false;

	// Textures
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	// Depth
	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	// Measurement specific
	double previousTime{ glfwGetTime() };
	int frameCount{ 0 };

	// VULKAN-SPECIFIC FUNCTIONS

	bool checkValidationLayerSupport();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	std::vector<const char*> getRequiredExtensions();
	static VKAPI_ATTR VkBool32 VKAPI_CALL
		debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	void populateDebugMessengerCreateInfo(
		VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	// PHYSICAL DEVICES
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	int rateDeviceSuitability(VkPhysicalDevice device);
	// QUEUE FAMILIES
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	// LOGICAL DEVICES
	void createLogicalDevice();
	// SURFACE
	void createSurface();
	// SWAP CHAIN & PRESENTATION
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void createSwapChain();
	void createImageViews();
	// GRAPHICS PIPELINE
	void createGraphicsPipeline();
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createRenderPass();
	// FRAMEBUFFER
	void createFramebuffers();
	// COMMAND POOLS & BUFFERS
	void createCommandPool();
	void createCommandBuffers();
	// SEMAPHORES
	void createSyncObjects();
	// VERTEX-BUFFERS
	void createVertexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter,
		VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
		VkMemoryPropertyFlags properties, VkBuffer& buffer,
		VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	// INDEX-BUFFERS
	void createIndexBuffer();
	// UNIFORM-BUFFERS
	void createDescriptorSetLayout();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	void createDescriptorPool();
	void createDescriptorSets();
	// TEXTURES
	void createTextureImage();
	void createImage(uint32_t width, uint32_t height, VkFormat format,
		VkImageTiling tiling, VkImageUsageFlags usage,
		VkMemoryPropertyFlags properties, VkImage& image,
		VkDeviceMemory& imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format,
		VkImageAspectFlags aspectFlags);
	void transitionImageLayout(VkImage image, VkFormat format,
		VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
		uint32_t height);
	void createTextureImageView();
	void createTextureSampler();
	// DEPTH
	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	// RENDERING-SPECIFIC FUNCTIONS
	void drawFrame();
	void recreateSwapChain();
	void cleanupSwapChain();
	static void framebufferResizeCallback(GLFWwindow* window, int width,
		int height);
	void loadModel();

	// MAIN FUNCTIONS
	void initWindow();
	void createInstance();
	void initVulkan();
	void setupDebugMessenger();
	void mainLoop();
	void cleanup();

	// MEASUREMENT-SPECIFIC FUNCTIONS
	void setWindowFPS();
};