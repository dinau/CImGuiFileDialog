#pragma once
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igCheckbox igCheckbox_Str
#define igButton igButton_Str
#endif

#include "setupFonts.h"
#include <stdlib.h>

typedef struct stWindow {
  GLFWwindow* handle;
  ImVec4 clearColor;
  int showDelayWindow;
} Window;

Window* createImGui(int32_t width, int32_t height);
void destroyImGui(Window* window);
void render(Window* window);
void newFrame(void);
void setTheme(int32_t num);
