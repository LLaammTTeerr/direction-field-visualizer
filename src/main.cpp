#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <raymath.h>
#include <string>
#include "utils.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1000;
const int MENU_HEIGHT = 100;
const int PADDING = 20;
constexpr int VIEW_WIDTH = SCREEN_WIDTH - 2 * PADDING;
constexpr int VIEW_HEIGHT = SCREEN_HEIGHT - MENU_HEIGHT - 2 * PADDING;
const int TARGET_FPS = 60;
const int MIN_X = -5;
const int MAX_X = +5;
const int MIN_Y = -5;
const int MAX_Y = +5;
const float STEP = 0.5f;
const float SCALE = 20.0f;

char CURRENT_FUNCTION_TEXT[128] = "x + y";
char NEW_FUNCTION_TEXT[128] = "x + y";

ExpressionParser parser;

float f(float x, float y) {
  return parser.Evaluate(CURRENT_FUNCTION_TEXT, x, y);
}

void DrawArrow(Vector2 start, Vector2 end, float thickness, Color arrowColor) {
  DrawLineEx(start, end, thickness, arrowColor);

  float angle = atan2f(end.y - start.y, end.x - start.x);

  float headLength = 3.0f * thickness;
  float headWidth = 3.0f * thickness;

  Vector2 arrowLeft = {
    end.x - headLength * cosf(angle) + headWidth * sinf(angle),
    end.y - headLength * sinf(angle) - headWidth * cosf(angle)
  };

  Vector2 arrowRight = {
    end.x - headLength * cosf(angle) - headWidth * sinf(angle),
    end.y - headLength * sinf(angle) + headWidth * cosf(angle)
  };

  DrawTriangle(end, arrowLeft, arrowRight, arrowColor);
}

Vector2 DecartesToView(float x, float y) {
  float scale = fminf(VIEW_WIDTH / (MAX_X - MIN_X), VIEW_HEIGHT / (MAX_Y - MIN_Y));
  float offsetX = PADDING + (VIEW_WIDTH - scale * (MAX_X - MIN_X)) / 2;
  float offsetY = PADDING + (VIEW_HEIGHT - scale * (MAX_Y - MIN_Y)) / 2;
  return {offsetX + scale * (x - MIN_X),
      offsetY + scale * (y - MIN_Y)};
}

void DrawMenu(void) {
  DrawRectangle(0, SCREEN_HEIGHT - MENU_HEIGHT, SCREEN_WIDTH, MENU_HEIGHT, GRAY);
  if (GuiTextBox({PADDING, SCREEN_HEIGHT - MENU_HEIGHT + PADDING, 500, 75}, NEW_FUNCTION_TEXT, 128, true)) {
    
  }
  if (GuiButton({PADDING + 500 + PADDING, SCREEN_HEIGHT - MENU_HEIGHT + PADDING, 100, 75}, "DRAW")) {
    strcpy(CURRENT_FUNCTION_TEXT, NEW_FUNCTION_TEXT);
  }
}

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Direction Field Visualizer by LamTer");
  SetTargetFPS(TARGET_FPS);

  GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawMenu();
    for (float x = MIN_X; x <= MAX_X; x += STEP) {
      for (float y = MIN_Y; y <= MAX_Y; y += STEP) {
        Vector2 pos = DecartesToView(x, y);
        Vector2 dir = Vector2Scale(Vector2Normalize({1.0f, f(x, y)}), SCALE);
        DrawArrow(pos, Vector2Add(pos, dir), 2.0f, BLUE);
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
