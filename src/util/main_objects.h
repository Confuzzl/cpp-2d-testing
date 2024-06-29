#pragma once

#define MAIN_SCENE MAIN_APP.scene
#define MAIN_RENDERER MAIN_APP.renderer
#define MAIN_CAMERA MAIN_SCENE.camera

#define VBO_1 VBOHolder::cachedGet<vertex_layout::pos, 1>()
#define VBO_2 VBOHolder::cachedGet<vertex_layout::pos, 2>()
#define VBO_3 VBOHolder::cachedGet<vertex_layout::pos, 3>()
#define VBO_4 VBOHolder::cachedGet<vertex_layout::pos, 4>()
