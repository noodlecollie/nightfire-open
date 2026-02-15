#include "client/cl_uifs.h"
#include "common/fscallback.h"
#include "common/engine_mempool.h"

qboolean CL_UIFS_OpenReadOnlyFile(const char* path, ui_gl_filesystem_handle_t* outHandle)
{
	if ( !path || !path[0] || !outHandle )
	{
		return false;
	}

	file_t* file = FS_Open(path, "rb", true);

	if ( file )
	{
		*outHandle = (ui_gl_filesystem_handle_t)file;
		return true;
	}
	else
	{
		*outHandle = 0;
		return false;
	}
}

void CL_UIFS_CloseFile(ui_gl_filesystem_handle_t handle)
{
	file_t* file = (file_t*)handle;

	if ( file )
	{
		FS_Close(file);
	}
}

size_t CL_UIFS_ReadFromFile(ui_gl_filesystem_handle_t handle, void* outBuffer, size_t bufferSize)
{
	if ( !outBuffer || bufferSize < 1 )
	{
		return 0;
	}

	file_t* file = (file_t*)handle;

	if ( !file )
	{
		return 0;
	}

	return (size_t)FS_Read(file, outBuffer, bufferSize);
}

qboolean CL_UIFS_SeekFile(ui_gl_filesystem_handle_t handle, int64_t offset, ui_gl_filesystem_origin origin)
{
	file_t* file = (file_t*)handle;

	if ( !file )
	{
		return false;
	}

	int fsOrigin;

	switch ( origin )
	{
		case UI_GL_FS_SEEK_CUR:
		{
			fsOrigin = SEEK_CUR;
			break;
		}

		case UI_GL_FS_SEEK_END:
		{
			fsOrigin = SEEK_END;
			break;
		}

		case UI_GL_FS_SEEK_SET:
		{
			fsOrigin = SEEK_SET;
			break;
		}

		default:
		{
			return false;
		}
	}

	return FS_Seek(file, (fs_offset_t)offset, fsOrigin) == 0;
}

size_t CL_UIFS_TellFile(ui_gl_filesystem_handle_t handle)
{
	file_t* file = (file_t*)handle;
	return file ? (size_t)FS_Tell(file) : 0;
}

size_t CL_UIFS_FileLength(ui_gl_filesystem_handle_t handle)
{
	file_t* file = (file_t*)handle;
	return file ? (size_t)FS_FileLength(file) : 0;
}

uint8_t* CL_UIFS_LoadFileData(const char* path, size_t* outLength)
{
	if ( !path || !path[0] || !outLength )
	{
		return NULL;
	}

	fs_offset_t length = 0;
	byte* data = FS_LoadFile(path, &length, true);

	if ( !data )
	{
		*outLength = 0;
		return false;
	}

	*outLength = length;
	return data;
}

void CL_UIFS_FreeFileData(uint8_t* data)
{
	if ( data )
	{
		Mem_Free(data);
	}
}
