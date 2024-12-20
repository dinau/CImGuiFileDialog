import std/[strformat,strutils,pegs]

const tmName = "theme_gold.cpp"
const outName = "theme_gold.c"
var sOut:string

sOut =  """
#include "appImGui.h"

void styleGold(void) {
	// Gold style by CookiePLMonster from ImThemes
	ImGuiStyle* style = igGetStyle();
"""

for line in readfile(tmName).split("\n"):
  if line =~ peg"\s+'style.'{@} 'ImVec4(' {@'f'}', ' {@'f'}', ' {@'f'}', '  {@'f'}');'":
    sOut =  sOut &  fmt"        style->{matches[0]}{{.x = {matches[1]} ,.y = {matches[2]}, .z = {matches[3]}, .w = {matches[4]}}};" & "\n"
  elif line =~ peg"\s+'style.'{@} 'ImVec2(' {@'f'}', '  {@'f'}');'":
    sOut =  sOut & fmt"        style->{matches[0]}{{.x = {matches[1]} ,.y = {matches[2]}}};"  & "\n"


sOut &=  "}" & "\n"
writeFile(outName,sOut)
