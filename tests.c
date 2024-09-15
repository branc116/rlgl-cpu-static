#include "external/raylib/raylib.h"
#include "external/raylib/raymath.h"
#include "external/raylib/rlgl.h"
#include "common.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TESTS(X) X(0) \
  X(1) X(2) X(3) X(4) X(5) X(6) X(7)

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
    printf("angle: %f\n", angle);
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

  rlViewport(0, 0, RW/2, RH/2);
  for (int i = 0; i < sizeof(lines)/sizeof(lines[0]); ++i)
    DrawLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3], RED);
  DrawCircle(50, 50, 50, BLACK);
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

      Vector2 a = (Vector2) { cos(rot), -sin(rot) };
      Vector2 b = Vector2Rotate(a, 2*PI/3);
      Vector2 c = Vector2Rotate(b, 2*PI/3);
      Vector2 arr[] = { a, b, c };
      for (int i = 0; i < 3; ++i) {
        arr[i] = Vector2Multiply(arr[i], (Vector2) { w, h });
        arr[i] = Vector2Add(arr[i], (Vector2){ 3.f*w, 3.f*h });
        arr[i] = Vector2Scale(arr[i], 0.5);
      }

      DrawTriangle(arr[0], arr[1], arr[2], YELLOW);
    }
  }
  EndDrawing();
}

void run_all_cases(void) {
#define X(c) do { case ## c (); rlSaveFrame("output/case" #c ".png"); } while (0);
  TESTS(X)
#undef X
}

typedef void (*case_t)(void);

case_t all_cases[] = {
#define X(c) case ## c,
  TESTS(X)
#undef X
};

int main(int argc, char** argv) {
  InitWindow(RW, RH, "hi");

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
    while(WindowShouldClose() == false) {
      all_cases[test_num]();
    }
  }
  CloseWindow();
  return 0;
}

