cmake_minimum_required(VERSION 3.21)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Ofast")

set(SDL2_INCLUDE_DIR F:/Learnin/RayTracing/include)
set(SDL2_LIB_DIR F:/Learnin/RayTracing/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(RayTracing main.cpp CApp.h CApp.cpp RayTrace/Image.cpp RayTrace/Image.h RayTrace/Scene.cpp RayTrace/Scene.h RayTrace/ray.cpp RayTrace/ray.h RayTrace/camera.cpp RayTrace/camera.h RayTrace/Primitives/objectbase.cpp RayTrace/Primitives/objectbase.h RayTrace/Primitives/objsphere.cpp RayTrace/Primitives/objsphere.h RayTrace/Lights/lightbase.cpp RayTrace/Lights/lightbase.h RayTrace/Lights/pointlight.cpp RayTrace/Lights/pointlight.h RayTrace/gtfm.cpp RayTrace/gtfm.h RayTrace/Primitives/objplane.cpp RayTrace/Primitives/objplane.h RayTrace/Materials/materialbase.cpp RayTrace/Materials/materialbase.h RayTrace/Materials/simplematerial.cpp RayTrace/Materials/simplematerial.h RayTrace/Primitives/cylinder.cpp RayTrace/Primitives/cylinder.h RayTrace/Primitives/cone.cpp RayTrace/Primitives/cone.h RayTrace/Textures/checker.cpp RayTrace/Textures/checker.h RayTrace/Textures/flat.cpp RayTrace/Textures/flat.h RayTrace/Textures/texturebase.cpp RayTrace/Textures/texturebase.h RayTrace/Textures/textureimage.cpp RayTrace/Textures/textureimage.h RayTrace/Materials/simplerefractive.cpp RayTrace/Materials/simplerefractive.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)