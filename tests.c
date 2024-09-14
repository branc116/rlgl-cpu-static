#include "external/raylib/raylib.h"
#include "external/raylib/raymath.h"
#include "common.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TESTS(X) X(0) \
  X(1) X(2) X(3) X(4) X(5)


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

#define CASE case3
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
  }
  CloseWindow();
  return 0;
}

