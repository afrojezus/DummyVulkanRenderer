# CMakeList.txt : CMake project for Shimakaze, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("Shimakaze")

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (Shimakaze "Shimakaze.cpp" "Shimakaze.h"  "vertex.h" "uniform_buffer_object.h" "helpers.h" "helpers.cpp"  )

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)


find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} glm)


find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)

add_library(stb_image SHARED ${CMAKE_SOURCE_DIR}/include/stb_image.h)
set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(stb_image PRIVATE STB_IMAGE_IMPLEMENTATION)

find_package(tinyobjloader CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} tinyobjloader::tinyobjloader)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} fmt::fmt fmt::fmt-header-only)

#set(MAKE_CMD "${CMAKE_CURRENT_SOURCE_DIR}/compile.bat")
#execute_process(COMMAND ${MAKE_CMD}
#   RESULT_VARIABLE CMD_ERROR
#   OUTPUT_FILE CMD_OUTPUT)
#MESSAGE( STATUS "CMD_ERROR:" ${CMD_ERROR})
#MESSAGE( STATUS "CMD_OUTPUT:" ${CMD_OUTPUT})

add_custom_command(
         TARGET Shimakaze POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:Shimakaze>/shaders
         COMMENT "Copying shaders" VERBATIM
         )

add_custom_command(
         TARGET Shimakaze POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:Shimakaze>/textures
         COMMENT "Copying textures" VERBATIM
         )

add_custom_command(
         TARGET Shimakaze POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/models $<TARGET_FILE_DIR:Shimakaze>/models
         COMMENT "Copying textures" VERBATIM
         )