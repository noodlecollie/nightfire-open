#pragma once

#include "BuildPlatform/Utils.h"

// These codes are Xash-specific - see sys_con.c
#define S_COL_BLACK 0
#define S_COL_RED 1
#define S_COL_GREEN 2
#define S_COL_YELLOW 3
#define S_COL_BLUE 4
#define S_COL_CYAN 5
#define S_COL_MAGENTA 6
#define S_COL_WHITE 7

#define S_COLSTR(val) "^" PLAT_STRINGIFY_VALUE(val)

#define S_NOTE S_COLSTR(S_COL_GREEN) "Note:" S_COLSTR(S_COL_WHITE) " "
#define S_WARN S_COLSTR(S_COL_YELLOW) "Warning:" S_COLSTR(S_COL_WHITE) " "
#define S_ERROR S_COLSTR(S_COL_RED) "Error:" S_COLSTR(S_COL_WHITE) " "
#define S_USAGE "Usage: "
#define S_USAGE_INDENT "       "

#define S_OPENGL_NOTE S_COLSTR(S_COL_GREEN) "OpenGL Note:" S_COLSTR(S_COL_WHITE) " "
#define S_OPENGL_WARN S_COLSTR(S_COL_YELLOW) "OpenGL Warning:" S_COLSTR(S_COL_WHITE) " "
#define S_OPENGL_ERROR S_COLSTR(S_COL_RED) "OpenGL Error:" S_COLSTR(S_COL_WHITE) " "
