#pragma once

#include <stdint.h>
#include "BuildPlatform/Typedefs.h"
#include "EngineInternalAPI/menu_int.h"

void CL_UIFS_Init(void);
void CL_UIFS_ShutDown(void);

qboolean CL_UIFS_OpenReadOnlyFile(const char* path, ui_gl_filesystem_handle_t* outHandle);
void CL_UIFS_CloseFile(ui_gl_filesystem_handle_t handle);
size_t CL_UIFS_ReadFromFile(ui_gl_filesystem_handle_t handle, void* outBuffer, size_t bufferSize);
qboolean CL_UIFS_SeekFile(ui_gl_filesystem_handle_t handle, int64_t offset, ui_gl_filesystem_origin origin);
size_t CL_UIFS_TellFile(ui_gl_filesystem_handle_t handle);
size_t CL_UIFS_FileLength(ui_gl_filesystem_handle_t handle);
uint8_t* CL_UIFS_LoadFileData(const char* path, size_t* outLength);
void CL_UIFS_FreeFileData(uint8_t* data);
ui_gl_filesystem_listing* CL_UIFS_FindFiles(const char* pattern);
void CL_UIFS_FreeListing(ui_gl_filesystem_listing* listing);
size_t CL_UIFS_ListingNumItems(const ui_gl_filesystem_listing* listing);
const char* CL_UIFS_ListingGetCurrentItem(const ui_gl_filesystem_listing* listing);
qboolean CL_UIFS_ListingNextItem(ui_gl_filesystem_listing* listing);
