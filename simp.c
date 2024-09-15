#include "external/stb_image_write.h"
#include "external/raylib/raylib.h"

#include "common.h"
#include "simp_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gcc -ggdb -O3 -c -o rc.o -DPLATFORM_DESKTOP src/rcore.c & gcc -ggdb -O3 -c -o u.o  -DPLATFORM_DESKTOP src/utils.c & gcc -ggdb -O3 -c -o t.o  -DPLATFORM_DESKTOP src/rtext.c & gcc -ggdb -O3 -c -o tx.o -DPLATFORM_DESKTOP src/rtextures.c & gcc -ggdb -O3 -c -o s.o  -DPLATFORM_DESKTOP src/rshapes.c 
// gcc -ggdb -fsanitize=address -DPLATFORM_DESKTOP simp.c rc.o u.o t.o tx.o s.o -lm
// gcc -ggdb -fsanitize=address -DPLATFORM_DESKTOP simp.c rc.o u.o t.o tx.o s.o -lm && time ./a.out && imv -u nearest_neighbour clear.png
// gcc -DNO_SW -o simp_rl simp.c ./bin/lib/libraylib.a -lm && ./simp_rl

#define BR_ASSERT(EXPR) if (!(EXPR)) *(volatile int*)0;

int g_texture_index;
int g_texture_active;
struct tex_s {
  void* texture;
  int width;
  int height;
  int format;
  int mipmaps;
} g_textures[16];
#define FRAMEBUFFER g_textures[0]

void rlSaveFrame(const char* name) {
  Color* pxs = FRAMEBUFFER.texture;
  stbi_write_png(name, FRAMEBUFFER.width, FRAMEBUFFER.height, 4, pxs, FRAMEBUFFER.width * 4);
}

#define RL_LINES 0x0001     // GL_LINES
#define RL_TRIANGLES 0x0004 // GL_TRIANGLES
#define RL_QUADS 0x0007     // GL_QUADS
int g_begin_mode;
void rlBegin(int mode) {
  g_begin_mode = mode;
}
static Color g_clear_color;
void rlClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  g_clear_color = (Color){ r, g, b, a };
}
void rlClearScreenBuffers() {
  Color* color = FRAMEBUFFER.texture;
  for (int i = 0; i < FRAMEBUFFER.height; ++i) {
    for (int j = 0; j < FRAMEBUFFER.width; ++j) {
      color[i * FRAMEBUFFER.width + j] = g_clear_color;
    }
  }
}
static Color g_color;
void rlColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  g_color = (Color) { r, g, b, a };
}
static bool g_depth_test = true;
void rlDisableDepthTest(void) { g_depth_test = false; }
void rlDisableFramebuffer(void) {
  printf("Disable Framebuffer...\n");
}
static bool g_scissor_enabled = true;
void rlDisableScissorTest(void) {
  g_scissor_enabled = false;
}
void rlDisableStereoRender()       { *(volatile int*)0; }
void rlDrawRenderBatchActive()     { }
void rlEnableDepthTest(void) { g_depth_test = true; }
void rlEnableFramebuffer(int framebuffer) {
  printf("Enabling framebuffer: %d\n", framebuffer);
}
void rlEnableScissorTest(void) {
  g_scissor_enabled = true;
}
void rlEnableShader()              { *(volatile int*)0; }
void rlEnableStereoRender()        { *(volatile int*)0; }
typedef struct {
  Vector3 position;
  Vector2 tex_coord;
  Vector3 normal_coord;
  Color color;
} vertex_t;
vertex_t g_vertexies[4];
int g_vertexies_index;
void rlEnd(void) {
  BR_ASSERT(g_vertexies_index == 0);
}
void rlFramebufferAttach()         { *(volatile int*)0; }
void rlFramebufferComplete()       { *(volatile int*)0; }
#define RL_MODELVIEW  0x1700      // GL_MODELVIEW
#define RL_PROJECTION 0x1701      // GL_PROJECTION
#define RL_TEXTURE    0x1702      // GL_TEXTURE
Matrix g_matrix_stacks[3][16];
int g_matrix_stacks_index[3];
int g_matrix_active;
#define ACTIVE_MATRIX g_matrix_stacks[g_matrix_active][g_matrix_stacks_index[g_matrix_active]]
void rlFrustum(double left, double right, double bottom, double top, double znear, double zfar) {
  ACTIVE_MATRIX = rlMatrixMultiply(rlGetFrustum(left, right, bottom, top, znear, zfar), ACTIVE_MATRIX);
}
void rlGenTextureMipmaps()         { *(volatile int*)0; }
int rlGetActiveFramebuffer(void) { return 1; }
static double rlCullDistanceNear = 0.01;
static double rlCullDistanceFar = 1000.0f;
double rlGetCullDistanceFar(void) { return rlCullDistanceFar; }
double rlGetCullDistanceNear(void) { return rlCullDistanceNear; }
void rlGetLocationAttrib()         { *(volatile int*)0; }
void rlGetLocationUniform()        { *(volatile int*)0; }
const char* rlGetPixelFormatName(int format) {
    switch (format)
    {
        case PIXELFORMAT_UNCOMPRESSED_GRAYSCALE: return "GRAYSCALE"; break;         // 8 bit per pixel (no alpha)
        case PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA: return "GRAY_ALPHA"; break;       // 8*2 bpp (2 channels)
        case PIXELFORMAT_UNCOMPRESSED_R5G6B5: return "R5G6B5"; break;               // 16 bpp
        case PIXELFORMAT_UNCOMPRESSED_R8G8B8: return "R8G8B8"; break;               // 24 bpp
        case PIXELFORMAT_UNCOMPRESSED_R5G5B5A1: return "R5G5B5A1"; break;           // 16 bpp (1 bit alpha)
        case PIXELFORMAT_UNCOMPRESSED_R4G4B4A4: return "R4G4B4A4"; break;           // 16 bpp (4 bit alpha)
        case PIXELFORMAT_UNCOMPRESSED_R8G8B8A8: return "R8G8B8A8"; break;           // 32 bpp
        case PIXELFORMAT_UNCOMPRESSED_R32: return "R32"; break;                     // 32 bpp (1 channel - float)
        case PIXELFORMAT_UNCOMPRESSED_R32G32B32: return "R32G32B32"; break;         // 32*3 bpp (3 channels - float)
        case PIXELFORMAT_UNCOMPRESSED_R32G32B32A32: return "R32G32B32A32"; break;   // 32*4 bpp (4 channels - float)
        case PIXELFORMAT_UNCOMPRESSED_R16: return "R16"; break;                     // 16 bpp (1 channel - half float)
        case PIXELFORMAT_UNCOMPRESSED_R16G16B16: return "R16G16B16"; break;         // 16*3 bpp (3 channels - half float)
        case PIXELFORMAT_UNCOMPRESSED_R16G16B16A16: return "R16G16B16A16"; break;   // 16*4 bpp (4 channels - half float)
        case PIXELFORMAT_COMPRESSED_DXT1_RGB: return "DXT1_RGB"; break;             // 4 bpp (no alpha)
        case PIXELFORMAT_COMPRESSED_DXT1_RGBA: return "DXT1_RGBA"; break;           // 4 bpp (1 bit alpha)
        case PIXELFORMAT_COMPRESSED_DXT3_RGBA: return "DXT3_RGBA"; break;           // 8 bpp
        case PIXELFORMAT_COMPRESSED_DXT5_RGBA: return "DXT5_RGBA"; break;           // 8 bpp
        case PIXELFORMAT_COMPRESSED_ETC1_RGB: return "ETC1_RGB"; break;             // 4 bpp
        case PIXELFORMAT_COMPRESSED_ETC2_RGB: return "ETC2_RGB"; break;             // 4 bpp
        case PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA: return "ETC2_RGBA"; break;       // 8 bpp
        case PIXELFORMAT_COMPRESSED_PVRT_RGB: return "PVRT_RGB"; break;             // 4 bpp
        case PIXELFORMAT_COMPRESSED_PVRT_RGBA: return "PVRT_RGBA"; break;           // 4 bpp
        case PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA: return "ASTC_4x4_RGBA"; break;   // 8 bpp
        case PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA: return "ASTC_8x8_RGBA"; break;   // 2 bpp
        default: return "UNKNOWN"; break;
    }
}
void rlGetShaderIdDefault()        { *(volatile int*)0; }
void rlGetShaderLocsDefault()      { *(volatile int*)0; }
int rlGetVersion()                { return 3; }
typedef void (*func_t)(void);
func_t extensions_func_g;
void rlLoadExtensions(func_t func) {
  extensions_func_g = func;
}

int rlLoadFramebuffer() {
  return 0;
}
void rlLoadIdentity() { 
  ACTIVE_MATRIX = rlMatrixIdentity();
}
void rlLoadShaderCode()            { *(volatile int*)0; }

static void* copy_texture(const void* data, size_t num_elements, int kind) {
  size_t el_size = 0;
  switch (kind) {
    case PIXELFORMAT_UNCOMPRESSED_GRAYSCALE: el_size = 1; break;
    case PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA: el_size = 2; break;
    case PIXELFORMAT_UNCOMPRESSED_R8G8B8A8: el_size = 4; break;
    default: printf("Unknown texture format: %d\n", kind); *(volatile int*)0;
  }
  void* ret = malloc(num_elements * el_size);
  memcpy(ret, data, num_elements * el_size);
  return ret;
}

void rlLoadTexture(void* data, int width, int height, int format, int mipmaps ) {
  g_textures[g_texture_index++] = (struct tex_s) { copy_texture(data, width * height, format), width, height, format, mipmaps };
}
void rlLoadTextureCubemap()        { *(volatile int*)0; }
void rlLoadTextureDepth()          { *(volatile int*)0; }
void rlMatrixMode(int matrix) { g_matrix_active = matrix - RL_MODELVIEW; }
void rlMultMatrixf(const float *mat) {
  ACTIVE_MATRIX = rlMatrixMultiply(floats_to_mat(mat), ACTIVE_MATRIX);
}

Vector3 g_normal;
void rlNormal3f(float x, float y, float z) {
  g_normal = (Vector3) { x, y, z };
}
void rlOrtho(double left, double right, double bottom, double top, double znear, double zfar) {
  ACTIVE_MATRIX = rlMatrixMultiply(rlGetOrtho(left, right, bottom, top, znear, zfar), ACTIVE_MATRIX);
}
void rlPopMatrix() { --g_matrix_stacks_index[g_matrix_active]; }
void rlPushMatrix() { ++g_matrix_stacks_index[g_matrix_active]; ACTIVE_MATRIX = g_matrix_stacks[g_matrix_active][g_matrix_stacks_index[g_matrix_active] - 1]; }
void rlReadScreenPixels()          { *(volatile int*)0; }
void rlReadTexturePixels()         { *(volatile int*)0; }
void rlRotatef(float angle, float x, float y, float z) { 
  Matrix matRotation = rlMatrixIdentity();
  // Axis vector (x, y, z) normalization
  float lengthSquared = x*x + y*y + z*z;
  if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
  {
      float inverseLength = 1.0f/sqrtf(lengthSquared);
      x *= inverseLength;
      y *= inverseLength;
      z *= inverseLength;
  }

  // Rotation matrix generation
  float sinres = sinf(DEG2RAD*angle);
  float cosres = cosf(DEG2RAD*angle);
  float t = 1.0f - cosres;

  matRotation.m0 = x*x*t + cosres;
  matRotation.m1 = y*x*t + z*sinres;
  matRotation.m2 = z*x*t - y*sinres;
  matRotation.m3 = 0.0f;

  matRotation.m4 = x*y*t - z*sinres;
  matRotation.m5 = y*y*t + cosres;
  matRotation.m6 = z*y*t + x*sinres;
  matRotation.m7 = 0.0f;

  matRotation.m8 = x*z*t + y*sinres;
  matRotation.m9 = y*z*t - x*sinres;
  matRotation.m10 = z*z*t + cosres;
  matRotation.m11 = 0.0f;

  matRotation.m12 = 0.0f;
  matRotation.m13 = 0.0f;
  matRotation.m14 = 0.0f;
  matRotation.m15 = 1.0f;
  ACTIVE_MATRIX = rlMatrixMultiply(matRotation, ACTIVE_MATRIX);
}

// Rectangle, 4 components
typedef struct {
    int x;      // Rectangle top-left corner position x
    int y;      // Rectangle top-left corner position y
    int width;  // Rectangle width
    int height; // Rectangle height
} br_irectangle;
br_irectangle g_scissor;

void rlScissor(int x, int y, int width, int height) {
  g_scissor = (br_irectangle) { x, y, width, height };
}
void rlSetBlendMode()              { *(volatile int*)0; }
void rlSetFramebufferHeight(int height) {
  FRAMEBUFFER.height = height;
}
void rlSetFramebufferWidth(int width) {
  FRAMEBUFFER.width = width;
}
void rlSetMatrixProjectionStereo() { *(volatile int*)0; }
void rlSetMatrixViewOffsetStereo() { *(volatile int*)0; }
void rlSetShader()                 { *(volatile int*)0; }
void rlSetTexture(int id) {
  g_texture_active = id;
}
void rlSetUniform()                { *(volatile int*)0; }
void rlSetUniformMatrix()          { *(volatile int*)0; }
void rlSetUniformSampler()         { *(volatile int*)0; }
Vector2 g_tex_coord;
void rlTexCoord2f(float x, float y) {
  g_tex_coord = (Vector2) { x, y };
}
void rlTextureParameters()         { *(volatile int*)0; }
void rlTranslatef(float x, float y, float z) { 
  Matrix matTranslation = {
    1.0f, 0.0f, 0.0f, x,
    0.0f, 1.0f, 0.0f, y,
    0.0f, 0.0f, 1.0f, z,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  ACTIVE_MATRIX = rlMatrixMultiply(matTranslation, ACTIVE_MATRIX);
}
void rlUnloadFramebuffer()         { *(volatile int*)0; }
void rlUnloadShaderProgram()       { *(volatile int*)0; }
void rlUnloadTexture(int id) {
  //free(g_textures[id].texture);
}
void rlUpdateTexture()             { *(volatile int*)0; }

void rlActiveTextureSlot() { *(volatile int*)0; }
void rlColor3f(float r, float g, float b) {
  g_color = (Color) { r * 255.f, g * 255.f, b * 255.f, g_color.a };
}
void rlDisableBackfaceCulling() { *(volatile int*)0; }
void rlDisableShader() { *(volatile int*)0; }
void rlDisableTexture() { *(volatile int*)0; }
void rlDisableTextureCubemap() { *(volatile int*)0; }
void rlDisableVertexArray() { *(volatile int*)0; }
void rlDisableVertexAttribute() { *(volatile int*)0; }
void rlDisableVertexBuffer() { *(volatile int*)0; }
void rlDisableVertexBufferElement() { *(volatile int*)0; }
void rlDisableWireMode() { *(volatile int*)0; }
void rlDrawVertexArray() { *(volatile int*)0; }
void rlDrawVertexArrayElements() { *(volatile int*)0; }
void rlDrawVertexArrayElementsInstanced() { *(volatile int*)0; }
void rlDrawVertexArrayInstanced() { *(volatile int*)0; }
void rlEnableBackfaceCulling() { *(volatile int*)0; }
void rlEnablePointMode() { *(volatile int*)0; }
void rlEnableTexture() { *(volatile int*)0; }
void rlEnableTextureCubemap() { *(volatile int*)0; }
void rlEnableVertexArray() { *(volatile int*)0; }
void rlEnableVertexAttribute() { *(volatile int*)0; }
void rlEnableVertexBuffer() { *(volatile int*)0; }
void rlEnableVertexBufferElement() { *(volatile int*)0; }
void rlEnableWireMode() { *(volatile int*)0; }
void rlGetFramebufferHeight() { *(volatile int*)0; }
void rlGetFramebufferWidth() { *(volatile int*)0; }
void rlGetMatrixModelview() { *(volatile int*)0; }
void rlGetMatrixProjection() { *(volatile int*)0; }
void rlGetMatrixProjectionStereo() { *(volatile int*)0; }
void rlGetMatrixTransform() { *(volatile int*)0; }
void rlGetMatrixViewOffsetStereo() { *(volatile int*)0; }
void rlGetTextureIdDefault() { *(volatile int*)0; }
void rlIsStereoRenderEnabled() { *(volatile int*)0; }
void rlLoadVertexArray() { *(volatile int*)0; }
void rlLoadVertexBuffer() { *(volatile int*)0; }
void rlLoadVertexBufferElement() { *(volatile int*)0; }
void rlScalef(float x, float y, float z) {
    Matrix matScale = {
        x, 0.0f, 0.0f, 0.0f,
        0.0f, y, 0.0f, 0.0f,
        0.0f, 0.0f, z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    ACTIVE_MATRIX = rlMatrixMultiply(matScale, ACTIVE_MATRIX);
}
void rlSetMatrixModelview() { *(volatile int*)0; }
void rlSetMatrixProjection() { *(volatile int*)0; }
void rlSetVertexAttribute() { *(volatile int*)0; }
void rlSetVertexAttributeDefault() { *(volatile int*)0; }
void rlSetVertexAttributeDivisor() { *(volatile int*)0; }
void rlUnloadVertexArray() { *(volatile int*)0; }
void rlUnloadVertexBuffer() { *(volatile int*)0; }
void rlUpdateVertexBuffer() { *(volatile int*)0; }

static void br_draw_line(struct tex_s tex, const vertex_t* from, const vertex_t* to);
static void br_draw_triangle(struct tex_s tex, const vertex_t* a, const vertex_t* b, const vertex_t* c);
static void br_draw_quad(struct tex_s tex, const vertex_t* a, const vertex_t* b, const vertex_t* c, const vertex_t* d);
static void br_try_pop_vertexies(void) {
  if (g_vertexies_index == 1) return;
  else if (g_begin_mode == RL_LINES) {
    br_draw_line(FRAMEBUFFER, &g_vertexies[0], &g_vertexies[1]);
    g_vertexies_index = 0;
  }
  else if (g_vertexies_index == 2) return;
  else if (g_begin_mode == RL_TRIANGLES) {
    br_draw_triangle(FRAMEBUFFER, &g_vertexies[0], &g_vertexies[1], &g_vertexies[2]);
    g_vertexies_index = 0;
  }
  else if (g_vertexies_index == 3) return;
  else if (g_begin_mode == RL_QUADS) {
    br_draw_quad(FRAMEBUFFER, &g_vertexies[0], &g_vertexies[1], &g_vertexies[2], &g_vertexies[3]);
    g_vertexies_index = 0;
  }
  else *(volatile int*)0;
}

void rlVertex3f(float x, float y, float z) {
  g_vertexies[g_vertexies_index++] = (vertex_t) {
    .position = { x, y, z },
    .tex_coord = g_tex_coord,
    .normal_coord = g_normal,
    .color = g_color
  };
  br_try_pop_vertexies();
}

void rlVertex2f(float x, float y) {
  g_vertexies[g_vertexies_index++] = (vertex_t) {
    .position = { x, y },
    .tex_coord = g_tex_coord,
    .normal_coord = g_normal,
    .color = g_color
  };
  br_try_pop_vertexies();
}

br_irectangle g_viewport;
void rlViewport(int x, int y, int width, int height) {
  g_viewport = (br_irectangle){ x, y, width, height };
  printf("Setting viewport to: %d %d %d %d\n", x, y, width, height);
}
void rlglClose(void) {
  rlSaveFrame("done.png");
}

int g_rlgl_width, g_rlgl_height;
void rlglInit(unsigned int width, unsigned int height) {
  FRAMEBUFFER.width = width;
  FRAMEBUFFER.height = height;
  FRAMEBUFFER.texture = malloc(width * height * 4);
  g_texture_index = 1;
  g_rlgl_width = width, g_rlgl_height = height;
  printf("Init rlgl to %d, %d\n", width, height);
}

void glfwCreateStandardCursor() {*(volatile int*)0;}
void* g_window = (void*)1;
const char* g_title;
int g_width;
int g_height;
void* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share) {
  g_width = width;
  g_height = height;
  g_title = title;
  printf("Create window %d %d %s", width, height, title);
  return g_window;
}
void glfwDefaultWindowHints(void) {}
void glfwDestroyWindow(void* monitor_handle) {}
void glfwFocusWindow() {*(volatile int*)0;}
void glfwGetClipboardString() {*(volatile int*)0;}

#define GLFW_NO_ERROR 0
int glfwGetError(const char** out_error) {
  (void)(out_error ? *out_error = NULL : NULL);
  return GLFW_NO_ERROR;
}
void glfwGetFramebufferSize() {*(volatile int*)0;}
void glfwGetGamepadState() {*(volatile int*)0;}
void glfwGetJoystickName() {*(volatile int*)0;}
void glfwGetKeyName() {*(volatile int*)0;}
void glfwGetKeyScancode() {*(volatile int*)0;}
void glfwGetMonitorName() {*(volatile int*)0;}
void glfwGetMonitorPhysicalSize() {*(volatile int*)0;}
void glfwGetMonitorPos(void* monitor, int* xpos, int* ypos) { *xpos = *ypos = 0; }
void glfwGetMonitorWorkarea(void* monitor_handle, int* out_x, int* out_y, int* out_width, int* out_height) {
  *out_x = *out_y = 0;
  *out_width = g_width;
  *out_height = g_height;
}
void* monitors[] = { (void*)1 };
void** glfwGetMonitors(int* count) { *count = 1; return monitors;  }
#define GLFW_PLATFORM_NULL 3932210
int glfwGetPlatform(void) {
  return GLFW_PLATFORM_NULL;
}
void glfwGetPrimaryMonitor() {*(volatile int*)0;}

void glfw_get_func() { *(volatile int*)0; }

func_t glfwGetProcAddress(const char* procname) {
  return &glfw_get_func;
}
double g_time = 0;
double glfwGetTime(void) { return g_time += 1.0; }
struct { int w, h, r, g, b, f; } g_mode = { RW, RH, 8, 8, 8, 60 };
void* glfwGetVideoMode() { return &g_mode; }
void glfwGetVideoModes() {*(volatile int*)0;}
void glfwGetWindowAttrib() {*(volatile int*)0;}
void glfwGetWindowContentScale() {*(volatile int*)0;}
void glfwGetWindowMonitor() {*(volatile int*)0;}
void glfwGetWindowPos(void* window, int* xpos, int* ypos) { *xpos = *ypos = 0; }
void glfwHideWindow() {*(volatile int*)0;}
void glfwIconifyWindow() {*(volatile int*)0;}
bool glfwInit() { return true; }
bool glfwJoystickPresent(int i) { return false; }
void glfwMakeContextCurrent(void* window_handle) {
  printf("Making Current context: %p\n", window_handle);
}
void glfwMaximizeWindow() {*(volatile int*)0;}
void glfwPollEvents(void) {}
int glfwRawMouseMotionSupported() { return false; }
void glfwRestoreWindow() {*(volatile int*)0;}
void glfwSetClipboardString() {*(volatile int*)0;}
void glfwSetCursor() {*(volatile int*)0;}
void glfwSetCursorPos(void* monitor_handle, int x, int y) {

}
void glfwSetInputMode(void* monitor_handle, int modes, bool value) {
  printf("Setting input modes %d to: %d\n", modes, value);
}
void glfwSetWindowAttrib() {*(volatile int*)0;}
void glfwSetWindowIcon() {*(volatile int*)0;}
void glfwSetWindowMonitor() {*(volatile int*)0;}
void glfwSetWindowOpacity() {*(volatile int*)0;}
int g_window_pos_x, g_window_pos_y;
void glfwSetWindowPos(void* monitor_handle, int x, int y) {
  g_window_pos_x = x, g_window_pos_y = y;
  printf("Move window to %d %d\n", x, y);
}
bool g_should_close;
void glfwSetWindowShouldClose(bool should) { g_should_close = should; }
void glfwSetWindowSize() {*(volatile int*)0;}
void glfwSetWindowSizeLimits(int width, int height) {
  printf("Min size: %d, %d\n", width, height);
}
void glfwSetWindowTitle() {*(volatile int*)0;}
void glfwShowWindow() {*(volatile int*)0;}
void glfwSwapBuffers(void* monitor_handle) {}
int g_swap_interval;
void glfwSwapInterval(int interval) {
  g_swap_interval = interval;
}
void glfwTerminate(void) {}
void glfwUpdateGamepadMappings() {*(volatile int*)0;}
void glfwWaitEvents() {*(volatile int*)0;}
void glfwWindowHint(int hint, bool value) {printf("Setting window hint %d to %d\n", hint, value);}
bool glfwWindowShouldClose(void* monitor_handle) { return false; }

void glfwSetMouseButtonCallback(void* monitor_h, func_t func) {}
void glfwSetScrollCallback(void* monitor_h, func_t func) {}
void glfwSetKeyCallback(void* monitor_h, func_t func) {}
void glfwSetCursorPosCallback(void* monitor_h, func_t func) {}

void glfwSetCharCallback(void* monitor_h, func_t func) {}
void glfwSetCursorEnterCallback(void* monitor_h, func_t func) {}
void glfwSetDropCallback(void* monitor_h, func_t func) {}
void* g_error_callback;
void glfwSetErrorCallback(void (*ec)(int error, const char *description)) { g_error_callback = ec; }
void* g_joystic_callback;
void glfwSetJoystickCallback(void* cb) { g_joystic_callback = cb; }
void glfwSetWindowContentScaleCallback() {*(volatile int*)0;}
void glfwSetWindowFocusCallback(void* monitor_h, func_t func) {}
void glfwSetWindowIconifyCallback(void* monitor_h, func_t func) {}
void glfwSetWindowMaximizeCallback(void* monitor_h, func_t func) {}
void glfwSetWindowSizeCallback(void* monitor_h, func_t func) {}
void glfwSetWindowPosCallback(void* monitor_h, func_t func) {}

#define RL_MODELVIEW_I  0
#define RL_PROJECTION_I 1
typedef struct {
  int x, y;
} br_ivec2_t;

void br_get_raster_point(const vertex_t* x, br_ivec2_t* out) {
  float pix_height = 2.f / g_viewport.height;
  float pix_width = 2.f / g_viewport.width;
  Matrix combined = MatrixMultiply(g_matrix_stacks[RL_MODELVIEW_I][g_matrix_stacks_index[RL_MODELVIEW_I]], g_matrix_stacks[RL_PROJECTION_I][g_matrix_stacks_index[RL_PROJECTION_I]]);
  Vector4 v = { x->position.x, x->position.y, x->position.z, 1 };
  Vector4 y = Vector4Transform(v, combined);
  y.x /= y.w;
  y.y /= y.w;

  out->x = (int)((y.x + 1.f) / 2.f * g_viewport.width);
  out->y = (int)((1.f - ((y.y + 1.f) / 2.f)) * g_viewport.height);
}

static float br_clamp(float x, float m, float M) {
  if (x < m) return m;
  if (x > M) return M;
  return x;
}

static Vector2 br_clamp_v2(Vector2 x, float m, float M) {
  if      (x.x < m) x.x = m;
  else if (x.x > M) x.x = M;
  if      (x.y < m) x.y = m;
  else if (x.y > M) x.y = M;
  return x;
}

static Vector3 br_clamp_v3(Vector3 x, float m, float M) {
  if      (x.x < m) x.x = m;
  else if (x.x > M) x.x = M;
  if      (x.y < m) x.y = m;
  else if (x.y > M) x.y = M;
  if      (x.z < m) x.z = m;
  else if (x.z > M) x.z = M;
  return x;
}

#define SET_PIX(TEX, X, Y) do { \
  int final_x = (X) + g_viewport.x; \
  int final_y = (Y) + (TEX.height - g_viewport.height) - g_viewport.y; \
  if (final_x < (TEX).width) \
  if (final_y < (TEX).height) \
  if (final_x >= 0) \
  if (final_y >= 0) { \
    if (g_texture_active == 0) { \
      ((Color*)(TEX).texture)[final_x + (final_y * (TEX).width)] = g_color; \
    } else { \
      *(volatile int*)0; \
    } \
  } \
} while (0)

Color br_sample_texture(struct tex_s* tex, float x, float y) {
  int xi = ((int)(tex->width * x + 0.5f)) % tex->width;
  int yi = ((int)(tex->height * y + 0.5f)) % tex->height;
  int index = xi + yi * tex->width;

  switch (tex->format) {
    case PIXELFORMAT_UNCOMPRESSED_GRAYSCALE: {
      unsigned char* t = tex->texture;
      return (Color) { g_color.r * t[index], g_color.g * t[index], g_color.b * t[index], 255 };
    } break;
    case PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA: {
      struct { unsigned char c, a; } *t = tex->texture;
      return (Color) { g_color.r * t[index].c, g_color.g * t[index].c, g_color.b * t[index].c, t[index].a };
    } break;
    default: {
      printf("Unknown texture format: %d\n", tex->format);
      *(volatile int*)0;
    } break;
  }
  return BLACK;
}

void br_blend(Color* c, Color in) {
  if (in.a >= c->a) *c = in;
}

#define SET_PIX_TEX(TEX, X, Y, TX, TY) do { \
  int final_x = (X) + g_viewport.x; \
  int final_y = (Y) + (TEX.height - g_viewport.height) - g_viewport.y; \
  if (final_x < (TEX).width) \
  if (final_y < (TEX).height) \
  if (final_x >= 0) \
  if (final_y >= 0) { \
    if (g_texture_active != 0) { \
      Color c = br_sample_texture(&g_textures[g_texture_active], TX, TY); \
      br_blend(&((Color*)(TEX).texture)[final_x + (final_y * (TEX).width)], c); \
    } else { \
      *(volatile int*)0; \
    } \
  } \
} while (0)

static void br_draw_point(struct tex_s tex, const vertex_t* x) {
  br_ivec2_t out;
  br_get_raster_point(x, &out);
  SET_PIX(tex, out.x, out.y);
}

#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
int g_draw_mode = 0x1B02;

static void br_draw_line(struct tex_s tex, const vertex_t* a, const vertex_t* b) {
  if (g_draw_mode == GL_POINT) {
    br_draw_point(tex, a);
    br_draw_point(tex, b);
    return;
  }
  float pix_height = 2.f / g_viewport.height;
  float pix_width = 2.f / g_viewport.width;
  Color* t = tex.texture;
  int t_cap = tex.width * tex.height;
  Matrix combined = MatrixMultiply(g_matrix_stacks[RL_MODELVIEW_I][g_matrix_stacks_index[RL_MODELVIEW_I]], g_matrix_stacks[RL_PROJECTION_I][g_matrix_stacks_index[RL_PROJECTION_I]]);
  Vector3 from = br_clamp_v3(Vector3Transform(a->position, combined), -1.f, 1.f);
  Vector3 to = br_clamp_v3(Vector3Transform(b->position, combined), -1.f, 1.f);

  if (from.x > to.x) {
    Vector3 tmp = from;
    from = to;
    to = tmp;
  }

  float dx = to.x - from.x;
  float dy = to.y - from.y;
  if (dx == 0) {
    int x = (1.f + to.x) / 2 * g_viewport.width;
    float min_y = fminf(to.y, from.y);
    float max_y = fmaxf(to.y, from.y);
    int pix_fill_count = (int)ceilf((max_y - min_y) / pix_height);
    int y = (1.f - ((1.f + (max_y)) / 2.f)) * g_viewport.height;
    for (int i = 0; i < pix_fill_count; ++i) {
      SET_PIX(tex, x, i + y);
    }
  } else if (dy == 0) {
    int y = (1.f - ((1.f + (to.y)) / 2.f)) * g_viewport.height;
    float min_x = fminf(to.x, from.x);
    float max_x = fmaxf(to.x, from.x);
    int pix_fill_count = (int)ceilf((max_x - min_x) / pix_width);
    int x = (1.f + min_x) / 2 * g_viewport.width;
    for (int i = 0; i < pix_fill_count; ++i) {
      SET_PIX(tex, x + i, y);
    }
  } else if (dx > 0) {
    float x_from = from.x;
    float x_to = to.x;

    int sign_dy = from.y > to.y ? 1 : -1;
    int pix_fill_count_x = (int)ceilf((dx) / pix_width);
    int pix_fill_count_y = (int)ceilf(fabsf(dy) / pix_height);
    int xi = (1.f + x_from) / 2 * g_viewport.width;
    int yi = (1.f - ((1.f + (from.y)) / 2.f)) * g_viewport.height;
    if (pix_fill_count_x > pix_fill_count_y) {
      for (int i = 0; i < pix_fill_count_x; ++i) {
        int yn = yi + sign_dy * ((pix_fill_count_y * i) / pix_fill_count_x);
        SET_PIX(tex, xi + i, yn);
      }
    } else {
      for (int i = 0; i < pix_fill_count_y; ++i) {
        int xn = xi + ((pix_fill_count_x * i) / pix_fill_count_y);
        SET_PIX(tex, xn, yi + sign_dy * i);
      }
    }
  } else {
    printf("Failed to draw line: %f,%f %f,%f\n", from.x, from.y, to.x, to.y);
  }
}

static bool is_ccw(br_ivec2_t a, br_ivec2_t b, br_ivec2_t c) {
  int sum = (a.x - b.x)*(a.y + b.y);
  sum +=    (b.x - c.x)*(b.y + c.y);
  sum +=    (c.x - a.x)*(c.y + a.y);
  if (sum >= 0) return true;
  return false;
}

static bool is_cw(br_ivec2_t a, br_ivec2_t b, br_ivec2_t c) {
  int sum = (a.x - b.x)*(a.y + b.y);
  sum +=    (b.x - c.x)*(b.y + c.y);
  sum +=    (c.x - a.x)*(c.y + a.y);
  if (sum <= 0) return true;
  return false;
}

static float br_ivec2_dist(br_ivec2_t a, br_ivec2_t b) {
  return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

static void br_draw_triangle(struct tex_s tex, const vertex_t* a, const vertex_t* b, const vertex_t* c) {
  switch (g_draw_mode) {
    case GL_LINE:
      br_draw_line(tex, a, b);
      br_draw_line(tex, b, c);
      br_draw_line(tex, a, c);
      return;
    case GL_POINT:
      br_draw_point(tex, a);
      br_draw_point(tex, b);
      br_draw_point(tex, c);
      return;
  }
  float pix_height = 2.f / g_viewport.height;
  float pix_width = 2.f / g_viewport.width;

  br_ivec2_t ai, bi, ci;
  br_get_raster_point(a, &ai);
  br_get_raster_point(b, &bi);
  br_get_raster_point(c, &ci);

  for (int y = 0; y < g_viewport.height; ++y)
  for (int x = 0; x < g_viewport.width; ++x) {
    br_ivec2_t p = { x, y };
    bool apb = is_ccw(ai, p, bi);
    bool bpc = is_ccw(bi, p, ci);
    bool cpa = is_ccw(ci, p, ai);
    if (apb == bpc && bpc == cpa) {
      if (g_texture_active == 0) {
        SET_PIX(tex, x, y);
      } else {
        float w1 = (bi.y-ci.y)*(p.x-ci.x)+(ci.x-bi.x)*(p.y-ci.y);
        float w2 = (ci.y-ai.y)*(p.x-ci.x)+(ai.x-ci.x)*(p.y-ci.y);
        float n = (bi.y-ci.y)*(ai.x-ci.x)+(ci.x-bi.x)*(ai.y-ci.y); 
        w1 /= n;
        w2 /= n;
        Vector3 dists = { w1, w2, 1 - w1 - w2 };
        Vector2 texture = Vector2Add(
          Vector2Add(
            Vector2Scale(a->tex_coord, dists.x),
            Vector2Scale(b->tex_coord, dists.y)),
          Vector2Scale(c->tex_coord, dists.z));
        SET_PIX_TEX(tex, x, y, texture.x, texture.y);
      }
    }
  }
}

static void br_draw_quad(struct tex_s tex, const vertex_t* a, const vertex_t* b, const vertex_t* c, const vertex_t* d) {
  br_draw_triangle(tex, a, b, c);
  br_draw_triangle(tex, c, d, a);
}

