#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "utils.h"
#include "setupFonts.h"

#define ImGui_GetIO igGetIO

const char* IconFontPath1 =    "../utils/fonticon/fa6/fa-solid-900.ttf";
const char* IconFontPath2 = "../../utils/fonticon/fa6/fa-solid-900.ttf";
char const *iconFontPath;
char const *fontPath;
char sBufFontPath[2048];

/*-----------------
 * getFontPath()
 *----------------*/
const char* WinFontName1   = "meiryo.ttc";
const char* WinFontName2   = "segoeui.ttf"; //,"Seoge UI",14.4) # English region standard font

char* getWinFontPath(char* sBuf, int bufSize, const char* fontName) {
  char* sWinDir = getenv("windir");
  if (sWinDir == NULL) return NULL;
  snprintf(sBuf, bufSize, "%s\\Fonts\\%s", sWinDir, fontName);
  return sBuf;
}
/*-----------------
 * getLinuxFontPath()
 *----------------*/
// For Linux Mint 22 (Ubuntu/Debian family ok ?)
const char* LinuxJpFontName1 = "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc"; // ,"Noto Sans CJK",12.0) # Linux Mint
const char* LinuxJpFontName2 = "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf";  //"LiberationMono" Ubuntu english

/*------------
 * point2px()
 *-----------*/
float point2px(float point) {
  //## Convert point to pixel
  return (point * 96) / 72;
}

void fontError(const char* fontPath){
  printf("Error!: Not found font path: [%s] in %s\n", fontPath, __FILE__);
}

const ImWchar ranges_icon_fonts[]  = {(ImWchar)ICON_MIN_FA, (ImWchar)ICON_MAX_FA, (ImWchar)0};
/*--------------
 * setupFonts()
 *-------------*/
void setupFonts(void) {
  ImGuiIO* pio = ImGui_GetIO();
  ImFontConfig* config  = ImFontConfig_ImFontConfig();

  config->MergeMode = false;

  iconFontPath = IconFontPath1;

  // Setup Icon font
  if(false == existsFile(iconFontPath)) {
    fontError(iconFontPath);
    iconFontPath = IconFontPath2;
    if (false == existsFile(iconFontPath)) {
      fontError(iconFontPath);
      iconFontPath = NULL;
    }
  }
  if (NULL != iconFontPath){
    printf("Found Icon font path: [%s] in %s\n", iconFontPath, __FILE__);
    ImFontAtlas_AddFontFromFileTTF(pio->Fonts, iconFontPath, point2px(14), config , ranges_icon_fonts);
  }

  // Setup (Base font + Icon font)
  fontPath = getWinFontPath(sBufFontPath, sizeof(sBufFontPath), WinFontName1);
  if (false == existsFile(fontPath)) {
    fontError(fontPath);
    fontPath = getWinFontPath(sBufFontPath, sizeof(sBufFontPath), WinFontName2);
    if (false == existsFile(fontPath)) {
      fontError(fontPath);
      fontPath = LinuxJpFontName1;
      if (false == existsFile(fontPath)) {
        fontError(fontPath);
        fontPath = LinuxJpFontName2;
        if (false == existsFile(fontPath)) {
          fontError(fontPath);
          fontPath = NULL;
        }
      }
    }
  }
  if (NULL != fontPath){
    printf("Found font path: [%s]\n",fontPath);
    config->MergeMode = true;
    ImFont* font = ImFontAtlas_AddFontFromFileTTF(pio->Fonts, fontPath, point2px(16)
                                  , config
                                  , ImFontAtlas_GetGlyphRangesJapanese(pio->Fonts));
    if (font == NULL) {
      printf("Error!: AddFontFromFileTTF():  [%s] \n", fontPath);
    }
  }else{
    ImFontAtlas_AddFontDefault(pio->Fonts, NULL);
  }
}
