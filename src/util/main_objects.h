#pragma once

#define MAIN_SCENE MAIN_APP.scene
#define MAIN_RENDERER MAIN_APP.renderer
#define MAIN_CAMERA MAIN_SCENE.camera

#define ECS MAIN_SCENE.ecs

#define SHADERS MAIN_RENDERER.shaders

#define TEXTURE MAIN_RENDERER.textures

#define VBO_1 MAIN_RENDERER.allocators.vbo1
#define VBO_2 MAIN_RENDERER.allocators.vbo2
#define VBO_3 MAIN_RENDERER.allocators.vbo3
#define VBO_4 MAIN_RENDERER.allocators.vbo4
#define VBO_HOLDER MAIN_RENDERER.allocators.vboHeap
#define EBO_HOLDER MAIN_RENDERER.allocators.eboHeap