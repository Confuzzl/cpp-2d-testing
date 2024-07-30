#pragma once

#define MAIN_SCENE MAIN_APP.scene
#define MAIN_RENDERER MAIN_APP.renderer
#define MAIN_CAMERA MAIN_SCENE.camera

#define SHADERS MAIN_RENDERER.shaders

#define TEXTURE MAIN_RENDERER.textures

#define VBO_1 MAIN_RENDERER.allocators.VBO_1
#define VBO_HOLDER MAIN_RENDERER.allocators.vboHeap
#define EBO_HOLDER MAIN_RENDERER.allocators.eboHeap