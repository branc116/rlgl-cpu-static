#include "external/raylib/raylib.h"
#include "external/raylib/raymath.h"
#include "external/raylib/rlgl.h"
#include "common.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TESTS(X) X(0) \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) X(10)

void rlSaveFrame(const char* name);

void case0(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  EndDrawing();
}

void case1(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  DrawLine(100, 100, 100, 200, RED);
  EndDrawing();
}

void case2(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  DrawLine(100, 100, 100, 250, YELLOW);
  DrawLine(100, 100, 400, 100, RED);
  EndDrawing();
}

void case3(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  Vector2 f = { 100, 100 };
  float length = 100; 
  int i = 0;
  for (float angle = 0.f; i < 64; ++i, angle += PI / 32) {
    float s = sinf(angle), c = cosf(angle);
    Vector2 to = { c, -s }; 
    DrawLineV(f, Vector2Add(f, Vector2Scale(to, length)), YELLOW);
  }
  EndDrawing();
}

void case4(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  DrawTriangle((Vector2) {20, 20}, (Vector2) {20, 100}, (Vector2) {100, 20}, RED);
  DrawTriangle((Vector2) {20, 20}, (Vector2) {20, 100}, (Vector2) {100, 20}, RED);
  EndDrawing();
}

void case5(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  DrawCircle(100, 100, 50, RED);
  EndDrawing();
}

void case6(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  int lines[][4] = {
    { 10, 1, 10, 100 },
    { 10, 1, 15, 100 },
  };
  DrawRectangle(1, 1, RW/2, RH/2, YELLOW);
  DrawRectangle(RW/2, RH/2, RW/2, RH/2, YELLOW);
  for (int i = 0; i < sizeof(lines)/sizeof(lines[0]); ++i)
    DrawLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3], RED);
  rlDrawRenderBatchActive();

  rlViewport(0, 0, RW/2, RH/2);
  for (int i = 0; i < sizeof(lines)/sizeof(lines[0]); ++i)
    DrawLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3], RED);
  DrawCircle(50, 50, 50, BLACK);
  rlDrawRenderBatchActive();

  rlViewport(RW/2, RH/2, RW/2, RH/2);
  for (int i = 0; i < sizeof(lines)/sizeof(lines[0]); ++i)
    DrawLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3], RED);
  DrawCircle(50, 50, 50, RED);
  EndDrawing();
}

void case7(void) {
  BeginDrawing();
  ClearBackground(BLUE);

  int rows = 3;
  int cols = 4;
  int count = rows * cols;
  Vector2 angle_up = { 1, 0 };
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      float w = (float)RW / cols, h = (float)RH / rows;
      rlViewport(c * (RW / cols), r * (RH / rows), w, h);
      float rot = (c + 1) * (1 + r) * 0.1f;

      Vector2 a = (Vector2) { cosf(rot), -sinf(rot) };
      Vector2 b = Vector2Rotate(a, -2*PI/3);
      Vector2 c = Vector2Rotate(b, -2*PI/3);
      Vector2 arr[] = { a, b, c };
      for (int i = 0; i < 3; ++i) {
        arr[i] = Vector2Add(arr[i], (Vector2){ 1, 1 });
        arr[i] = Vector2Multiply(arr[i], (Vector2) { w, h });
        arr[i] = Vector2Scale(arr[i], 1.0f);
      }
      DrawTriangle(arr[0], arr[1], arr[2], YELLOW);
      rlDrawRenderBatchActive();
    }
  }
  EndDrawing();
}

void case8(void) {
  BeginDrawing();
  ClearBackground(BLUE);
  DrawText("Hello From the case8", 10, 100, 23, BLACK);
  EndDrawing();
}

void case9(void) {
  Camera camera = { 
    .position = { 2.0f, 4.0f, 6.0f }, // Camera position
    .target = { 0.0f, 0.5f, 0.0f },   // Camera looking at point
    .up = { 0.0f, 1.0f, 0.0f },       // Camera up vector (rotation towards target)
    .fovy = 55.0f,                    // Camera field-of-view Y
    .projection = CAMERA_PERSPECTIVE, // Camera projection type
  };
  BeginDrawing();
    ClearBackground(BLUE);
    BeginMode3D(camera);
      DrawCube(Vector3Zero(), 2.0, 4.0, 2.0, YELLOW);
    EndMode3D();
  EndDrawing();
}

void case10(void) {

}

void run_all_cases(void) {
#define X(c) do { case ## c (); rlViewport(0,0,RW,RH); rlSaveFrame("output/case" #c ".png"); } while (0);
  TESTS(X)
#undef X
}

typedef void (*case_t)(void);

case_t all_cases[] = {
#define X(c) case ## c,
  TESTS(X)
#undef X
};

extern int br_frames_count;
int main(int argc, char** argv) {
  InitWindow(RW, RH, "hi");
  br_frames_count = 1000000;

  if (argc == 1) {
    run_all_cases();
  } else if (argc == 2) {
    int test_num = atoi(argv[1]);
    all_cases[test_num]();
    char buff[32];
    sprintf(buff, "output/case%d.png", test_num);
    rlSaveFrame(buff); 
  } else if (argc == 3) {
    int test_num = atoi(argv[1]);
    SetTargetFPS(30);
    while(WindowShouldClose() == false) {
      rlViewport(0, 0, RW, RH);
      if (IsKeyPressed(KEY_J)) {
        if (test_num == 0) {
          test_num = (sizeof(all_cases) / sizeof(all_cases[0])) - 1;
        } else test_num = (test_num - 1);
      } else if (IsKeyPressed(KEY_K)) {
        test_num = test_num + 1 < (sizeof(all_cases) / sizeof(all_cases[0])) ? test_num + 1 : 0;
      }
      all_cases[test_num]();
    }
  }
  CloseWindow();
  return 0;
}

__attribute__((weak)) void rlSaveFrame(const char* name) {}


// ../../raysan5/raylib/examples
