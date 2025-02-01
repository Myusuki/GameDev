#include <stdio.h>
#include <raylib.h>
#include <tmx.h>
#include "include/mapRender.h"

int main(void)
{
  const int windowWidth = 800;
  const int windowHeight = 600;

  InitWindow(windowWidth, windowHeight, "Collision Interaction");
  SetTargetFPS(60);

  return 0;
}
