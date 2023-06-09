cmake_minimum_required(VERSION 3.25)
project(voxelgame_opengl_qt)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS_ASAN "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS_ASAN "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL)
qt_standard_project_setup()

find_package(LibNoise REQUIRED)

add_executable(voxelgame_opengl_qt resources.qrc src/main.cpp src/window.cpp src/window.h src/vertex.h src/geometry.h src/player.cpp src/player.h src/blocks/block.cpp src/blocks/block.h src/blocks/air_block.cpp src/blocks/air_block.h src/blocks/grass_block.cpp src/blocks/grass_block.h src/chunk.cpp src/chunk.h src/chunk_mesh.cpp src/chunk_mesh.h src/blocks/cobblestone_block.cpp src/blocks/cobblestone_block.h src/physics_loop.cpp src/physics_loop.h src/geometry.cpp src/input.cpp src/input.h src/selection_renderer.cpp src/selection_renderer.h src/world_renderer.cpp src/world_renderer.h src/world.cpp src/world.h src/arithmetic.h src/synchronized_queue.cpp src/synchronized_queue.h src/blocking_queue.cpp src/blocking_queue.h src/chunk_mesh_vertex_builder.cpp src/chunk_mesh_vertex_builder.h src/blocks/stone_block.cpp src/blocks/stone_block.h src/raycast.cpp src/raycast.h src/world_builder.cpp src/world_builder.h src/chunk_surrounder.cpp src/chunk_surrounder.h src/blocks/sand_block.cpp src/blocks/sand_block.h src/blocks/water_block.cpp src/blocks/water_block.h src/multimap3d.h)
target_link_libraries(voxelgame_opengl_qt PRIVATE Qt6::Core Qt6::Gui Qt6::OpenGL ${LIBNOISE_LIBRARIES})
target_include_directories(voxelgame_opengl_qt PRIVATE ${LIBNOISE_INCLUDE_DIR})

find_package(GTest)
enable_testing()
add_executable(all_tests test/all_tests.cpp src/window.cpp src/window.h src/vertex.h src/geometry.h src/player.cpp src/player.h src/blocks/block.cpp src/blocks/block.h src/blocks/air_block.cpp src/blocks/air_block.h src/blocks/grass_block.cpp src/blocks/grass_block.h src/chunk.cpp src/chunk.h src/chunk_mesh.cpp src/chunk_mesh.h src/blocks/cobblestone_block.cpp src/blocks/cobblestone_block.h src/physics_loop.cpp src/physics_loop.h src/geometry.cpp src/input.cpp src/input.h src/selection_renderer.cpp src/selection_renderer.h src/world_renderer.cpp src/world_renderer.h src/world.cpp src/world.h src/arithmetic.h src/synchronized_queue.cpp src/synchronized_queue.h src/blocking_queue.cpp src/blocking_queue.h src/chunk_mesh_vertex_builder.cpp src/chunk_mesh_vertex_builder.h src/blocks/stone_block.cpp src/blocks/stone_block.h src/raycast.cpp src/raycast.h src/world_builder.cpp src/world_builder.h src/chunk_surrounder.cpp src/chunk_surrounder.h src/blocks/sand_block.cpp src/blocks/sand_block.h src/blocks/water_block.cpp src/blocks/water_block.h src/multimap3d.h)
target_link_libraries(all_tests PRIVATE GTest::gtest_main Qt6::Core Qt6::Gui Qt6::OpenGL ${LIBNOISE_LIBRARIES})
target_include_directories(all_tests PRIVATE "${CMAKE_SOURCE_DIR}/src" "${LIBNOISE_INCLUDE_DIR}")
gtest_discover_tests(all_tests)
