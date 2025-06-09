#ifndef WINDOW_SYSTEM
#define WINDOW_SYSTEM
#include "window_data.h"

void Init(WindowData data);
void Update(float dt);
void Shutdown();
void SaveScreenShot();
#endif