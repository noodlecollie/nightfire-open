#include "rmlui/FileInterfaceImpl.h"
#include "udll_int.h"

Rml::FileHandle FileInterfaceImpl::Open(const Rml::String& path)
{
	ui_gl_filesystem_handle_t handle = 0;
	const bool success = gUiGlFuncs.filesystem.openReadOnlyFile(path.c_str(), &handle);

	return static_cast<Rml::FileHandle>(success ? handle : 0);
}

void FileInterfaceImpl::Close(Rml::FileHandle file)
{
	gUiGlFuncs.filesystem.closeFile(file);
}

size_t FileInterfaceImpl::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	return gUiGlFuncs.filesystem.readFromFile(file, buffer, size);
}

bool FileInterfaceImpl::Seek(Rml::FileHandle file, long offset, int origin)
{
	ui_gl_filesystem_origin fsOrigin;

	switch ( origin )
	{
		case SEEK_CUR:
		{
			fsOrigin = UI_GL_FS_SEEK_CUR;
			break;
		}

		case SEEK_SET:
		{
			fsOrigin = UI_GL_FS_SEEK_SET;
			break;
		}

		case SEEK_END:
		{
			fsOrigin = UI_GL_FS_SEEK_END;
			break;
		}

		default:
		{
			return false;
		}
	}

	return gUiGlFuncs.filesystem
		.seekFile(static_cast<ui_gl_filesystem_handle_t>(file), static_cast<int64_t>(offset), fsOrigin);
}

size_t FileInterfaceImpl::Tell(Rml::FileHandle file)
{
	return gUiGlFuncs.filesystem.tellFile(static_cast<ui_gl_filesystem_handle_t>(file));
}

size_t FileInterfaceImpl::Length(Rml::FileHandle file)
{
	return gUiGlFuncs.filesystem.fileLength(static_cast<ui_gl_filesystem_handle_t>(file));
}

bool FileInterfaceImpl::LoadFile(const Rml::String& path, Rml::String& out_data)
{
	out_data.clear();

	size_t length = 0;
	uint8_t* data = gUiGlFuncs.filesystem.loadFileData(path.c_str(), &length);

	if ( !data )
	{
		return false;
	}

	if ( length > 0 )
	{
		// This string may end up one byte longer than it needs to be,
		// but better to make absolutely sure that we do have a null terminator.
		out_data.resize(length + 1, 0);
		memcpy(&out_data[0], data, length);
	}

	gUiGlFuncs.filesystem.freeFileData(data);
	return true;
}
