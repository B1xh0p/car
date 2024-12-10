#pragma once
#include "ggl.h"

void Init();
void SetViewPortSize(float width, float height);
void Draw();

void KeyFunc(GLubyte byte, int x, int y);
void MouseFunc(int button, int state, int x, int y);
void MotionFunc(int x, int y);