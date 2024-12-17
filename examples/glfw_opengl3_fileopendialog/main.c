#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>

//----------------------------
// Include for FileOpenDialog
//----------------------------
#include "ImGuiFileDialog.h"
#include <stdlib.h>

//
#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

GLFWwindow *window;

int main(int argc, char *argv[])
{

  if (!glfwInit())
    return -1;

  // Decide GL+GLSL versions
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#if __APPLE__
  // GL 3.2 Core + GLSL 150
  const char *glsl_version = "#version 150";
#else
  // GL 3.2 + GLSL 130
  const char *glsl_version = "#version 130";
#endif

  // just an extra window hint for resize
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window = glfwCreateWindow(1024, 768, "CImGuiFileDialog example", NULL, NULL);
  if (!window)
  {
    printf("Failed to create window! Terminating!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // enable vsync
  glfwSwapInterval(1);

  // check opengl version sdl uses
  printf("opengl version: %s\n", (char *)glGetString(GL_VERSION));

  // setup imgui
  igCreateContext(NULL);

  // set docking
  ImGuiIO *ioptr = igGetIO();
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
  //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
  ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
  ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
#endif

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  igStyleColorsDark(NULL);
  // ImFontAtlas_AddFontDefault(io.Fonts, NULL);

  bool showDemoWindow = false;
  ImVec4 clearColor;
  clearColor.x = 0.45f;
  clearColor.y = 0.55f;
  clearColor.z = 0.60f;
  clearColor.w = 1.00f;

  //------------------------------
  // Create FileOpenDialog object
  //------------------------------
  ImGuiFileDialog *cfiledialog = IGFD_Create();

  // main event loop
  bool quit = false;
  while (!glfwWindowShouldClose(window))
  {

    glfwPollEvents();

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    if (showDemoWindow)
      igShowDemoWindow(&showDemoWindow);

    // show a simple window that we created ourselves.
    {
      static float f = 0.0f;
      static int counter = 0;

      igBegin("FileOpenDialog example window at 2024/12", NULL, 0);
      igText("ImGuiFileDialog: %s", IGFD_VERSION);
      igText("ImGui / CImGui: %s", igGetVersion());

      igCheckbox("Demo window", &showDemoWindow);

      igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0);
      igColorEdit3("clear color", (float *)&clearColor, 0);

      ImVec2 buttonSize;
      buttonSize.x = 100;
      buttonSize.y = 100;

      if (igButton("FileOpen", buttonSize)){
        //------------------------------
        // Trigger FileOpenDialog
        //------------------------------
        struct IGFD_FileDialog_Config config = IGFD_FileDialog_Config_Get();
        config.path = ".";
        config.flags = ImGuiFileDialogFlags_ConfirmOverwrite; // ImGuiFileDialogFlags
        IGFD_OpenDialog(cfiledialog,
            "filedlg",                                        // dialog key (make it possible to have different treatment reagrding the dialog key
            "Open a File",                                    // dialog title
            "c files(*.c *.h){.c,.h}",                        // dialog filter syntax : simple => .h,.c,.pp, etc and collections : text1{filter0,filter1,filter2}, text2{filter0,filter1,filter2}, etc..
            config); 								                          // the file dialog config
        counter++;
      }
      //------------------------------
      // Start display FileOpenDialog
      //------------------------------
      ImGuiIO* ioptr = igGetIO();
      ImVec2 maxSize;
      maxSize.x = ioptr->DisplaySize.x * 0.8f;
      maxSize.y = ioptr->DisplaySize.y * 0.8f;
      ImVec2 minSize;
      minSize.x = maxSize.x * 0.25f;
      minSize.y = maxSize.y * 0.25f;

      if (IGFD_DisplayDialog(cfiledialog, "filedlg", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
        if (IGFD_IsOk(cfiledialog)){ // result ok
          char* cfilePathName = IGFD_GetFilePathName(cfiledialog, IGFD_ResultMode_AddIfNoFileExt);
          printf("GetFilePathName : %s\n", cfilePathName);
          char* cfilePath = IGFD_GetCurrentPath(cfiledialog);
          printf("GetCurrentPath : %s\n", cfilePath);
          char* cfilter = IGFD_GetCurrentFilter(cfiledialog);
          printf("GetCurrentFilter : %s\n", cfilter);
          // here convert from string because a string was passed as a userDatas, but it can be what you want
          void* cdatas = IGFD_GetUserDatas(cfiledialog);
          if (cdatas) printf("GetUserDatas : %s\n", (const char*)cdatas);
          struct IGFD_Selection csel = IGFD_GetSelection(cfiledialog, IGFD_ResultMode_KeepInputFile); // multi selection
          printf("Selection :\n");
          for (int i = 0; i < (int)csel.count; i++) {
            printf("(%i) FileName %s => path %s\n", i, csel.table[i].fileName, csel.table[i].filePathName);
          }
          // action

          // destroy
          if (cfilePathName) free(cfilePathName);
          if (cfilePath) free(cfilePath);
          if (cfilter) free(cfilter);
          IGFD_Selection_DestroyContent(&csel);
        }
        IGFD_CloseDialog(cfiledialog);
      }
      // End FileOpenDialog



      igSameLine(0.0f, -1.0f);
      igText("counter = %d", counter);

      igText("Application average %.3f ms/frame (%.1f FPS)",
             1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
      igEnd();
    }

    // render
    igRender();
    glfwMakeContextCurrent(window);
    glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
    if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      GLFWwindow *backup_current_window = glfwGetCurrentContext();
      igUpdatePlatformWindows();
      igRenderPlatformWindowsDefault(NULL, NULL);
      glfwMakeContextCurrent(backup_current_window);
    }
#endif
    glfwSwapBuffers(window);
  }
      // destroy ImGuiFileDialog
      IGFD_Destroy(cfiledialog);

  // clean up
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
