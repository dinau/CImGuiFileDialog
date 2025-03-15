#include <stdio.h>
#include <string.h>
#include "appImGui.h"

// Include for FileDialog
#include "ImGuiFileDialog.h"

#define MAX_SIZE  2048
char sFilePathName[MAX_SIZE];
char sFileDirPath[MAX_SIZE];
char sFilter[MAX_SIZE];
char sDatas[MAX_SIZE];

//-------------
//--- gui_main
//-------------
void gui_main(Window *window) {
  bool showDemoWindow = false;

  //------------------------------
  // Create FileDialog object
  //------------------------------
  ImGuiFileDialog *cfd = IGFD_Create();

  //-----------------
  // main event loop
  //-----------------
  bool quit = false;
  while (!glfwWindowShouldClose(window->handle)) {
    glfwPollEvents();
    newFrame();

    if (showDemoWindow) {
      igShowDemoWindow(&showDemoWindow);
    }

    // show a simple window that we created ourselves.
    {
      igBegin("FileDialog example window at 2024/12", NULL, 0);
      igText("ImGuiFileDialog: %s", IGFD_VERSION);
      igText("ImGui / CImGui: %s", igGetVersion());

      igCheckbox("ImGui Demo", &showDemoWindow);
      igColorEdit3("Background color", (float *)&window->clearColor, 0);

      ImVec2 buttonSize = {.x = 100, .y = 50};

      if (igButton("FileOpen", buttonSize)) {
        //------------------------------
        // Trigger FileOpenDialog
        //------------------------------
        struct IGFD_FileDialog_Config config = IGFD_FileDialog_Config_Get();
        config.path                          = ".";
        //config.flags                       =   ImGuiFileDialogFlags_ConfirmOverwrite
        //                                     | ImGuiFileDialogFlags_Modal;
        config.flags                         = ImGuiFileDialogFlags_Modal;     // ImGuiFileDialogFlags
        IGFD_OpenDialog(cfd,
                        "filedlg",                             // dialog key (make it possible to have different treatment reagrding the dialog key
                        "Open a File",                         // dialog title
                        "all (*){.*},c files(*.c *.h){.c,.h}", // dialog filter syntax : simple => .h,.c,.pp, etc and collections : text1{filter0,filter1,filter2}, text2{filter0,filter1,filter2}, etc..
                                                               // NULL,                                 // dialog filter syntax : if it wants to select directory then set NULL
                        config);                               // the file dialog config
      }

      //------------------------------
      // Start display FileOpenDialog
      //------------------------------
      ImGuiIO *ioptr = igGetIO();
      ImVec2 maxSize = {.x = ioptr->DisplaySize.x * 0.8f, .y = ioptr->DisplaySize.y * 0.8f};
      ImVec2 minSize = {.x = maxSize.x * 0.25f, .y = maxSize.y * 0.25f};

      if (IGFD_DisplayDialog(cfd, "filedlg", ImGuiWindowFlags_NoCollapse, minSize, maxSize)) {
        if (IGFD_IsOk(cfd)) { // result ok
          char *cfilePathName = IGFD_GetFilePathName(cfd, IGFD_ResultMode_AddIfNoFileExt);
          printf("GetFilePathName : %s\n", cfilePathName);
          char *cfilePath = IGFD_GetCurrentPath(cfd);
          printf("GetCurrentPath : %s\n", cfilePath);
          char *cfilter = IGFD_GetCurrentFilter(cfd);
          printf("GetCurrentFilter : %s\n", cfilter);
          // here convert from string because a string was passed as a userDatas, but it can be what you want
          void *cdatas = IGFD_GetUserDatas(cfd);
          if (cdatas) {
            printf("GetUserDatas : %s\n", (const char *)cdatas);
            memcpy(sDatas,        cdatas,       strlen(cdatas) + 1);
          }
          struct IGFD_Selection csel = IGFD_GetSelection(cfd, IGFD_ResultMode_KeepInputFile); // multi selection
          printf("Selection :\n");
          for (int i = 0; i < (int)csel.count; i++) {
            printf("(%i) FileName %s => path %s\n", i, csel.table[i].fileName, csel.table[i].filePathName);
          }
          // action

          memcpy(sFilePathName, cfilePathName, strlen(cfilePathName) + 1);
          memcpy(sFileDirPath,   cfilePath,     strlen(cfilePath) + 1);
          memcpy(sFilter,       cfilter,       strlen(cfilter) + 1);

          // destroy
          if (cfilePathName) { free(cfilePathName); }
          if (cfilePath)     { free(cfilePath); }
          if (cfilter)       { free(cfilter); }
          IGFD_Selection_DestroyContent(&csel);
        }
        IGFD_CloseDialog(cfd);
      }
      // End FileOpenDialog

      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);

      igNewLine();
      igSeparator();
      igText("Selected file = %s", sFilePathName);
      igText("Dir           = %s", sFileDirPath);
      igText("Filter        = %s", sFilter);
      igText("Datas         = %s", sDatas);

      igEnd();
    }

    render(window);
  }
  // Destroy ImGuiFileDialog
  IGFD_Destroy(cfd);
}

//------
// main
//------
int main(int argc, char *argv[]) {
  Window *window = createImGui(1024, 768);
  setupFonts(); // See ../utils/setupFonts.c

  gui_main(window);

  destroyImGui(window);
}
