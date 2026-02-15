#pragma once

#include <stdint.h>
#include "BuildPlatform/Typedefs.h"
#include "EngineInternalAPI/menu_int.h"

qboolean CL_UIFS_OpenReadOnlyFile(const char* path, ui_gl_filesystem_handle_t* outHandle);
void CL_UIFS_CloseFile(ui_gl_filesystem_handle_t handle);
size_t CL_UIFS_ReadFromFile(ui_gl_filesystem_handle_t handle, void* outBuffer, size_t bufferSize);
qboolean CL_UIFS_SeekFile(ui_gl_filesystem_handle_t handle, int64_t offset, ui_gl_filesystem_origin origin);
size_t CL_UIFS_TellFile(ui_gl_filesystem_handle_t handle);
size_t CL_UIFS_FileLength(ui_gl_filesystem_handle_t handle);
uint8_t* CL_UIFS_LoadFileData(const char* path, size_t* outLength);
void CL_UIFS_FreeFileData(uint8_t* data);
