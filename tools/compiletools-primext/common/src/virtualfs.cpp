/*
virtualfs.cpp - virtual file system, operating into memory
Copyright (C) 2015 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "BuildPlatform/PlatformID.h"

#if XASH_WIN32()
#include <windows.h>
#include <direct.h>
#include <io.h>
#endif

#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include "CompileTools/conprint.h"
#include "CompileTools/cmdlib.h"
#include "CompileTools/file_system.h"
#include "CompileTools/zone.h"
#include "CRTLib/crtlib.h"

#define MAX_TOKEN 2048  // parse token length

/*
=============================================================================

VIRTUAL FILE SYSTEM - READ\WRITE DATA INTO MEMORY

=============================================================================
*/
/*
====================
VFS_Create

Create an empty virtualfile or from buffer
====================
*/
vfile_t* VFS_Create(const byte* buffer, size_t buffsize)
{
	vfile_t* file = (vfile_t*)Mem_Alloc(sizeof(vfile_t));

	if ( buffsize <= 0 )
		buffsize = FILE_BUFF_SIZE;  // empty file

	file->length = file->buffsize = buffsize;
	file->buff = (byte*)Mem_Alloc(file->buffsize);
	if ( buffer )
		memcpy(file->buff, buffer, buffsize);
	file->offset = 0;

	return file;
}

/*
====================
VFS_Read

Reading the virtual file
====================
*/
size_t VFS_Read(vfile_t* file, void* buffer, size_t buffersize)
{
	size_t read_size = 0;

	if ( buffersize == 0 )
		return 1;

	if ( !file )
		return 0;

	// check for enough room
	if ( file->offset >= file->length )
		return 0;  // hit EOF

	if ( (file->offset + buffersize) <= file->length )
	{
		memcpy(buffer, file->buff + file->offset, buffersize);
		file->offset += buffersize;
		read_size = buffersize;
	}
	else
	{
		size_t reduced_size = file->length - file->offset;
		memcpy(buffer, file->buff + file->offset, reduced_size);
		file->offset += reduced_size;
		read_size = reduced_size;
	}

	return read_size;
}

/*
====================
VFS_Write

Write to the virtual file
====================
*/
size_t VFS_Write(vfile_t* file, const void* buf, size_t size)
{
	if ( !file )
	{
		return (size_t)-1;
	}

	if ( (file->offset + size) >= file->buffsize )
	{
		size_t newsize = file->offset + size + FILE_BUFF_SIZE;

		if ( file->buffsize < newsize )
		{
			// reallocate buffer now
			file->buff = (byte*)Mem_Realloc(file->buff, newsize);
			file->buffsize = newsize;  // merge buffsize
		}
	}

	// write into buffer
	if ( buf )
	{
		memcpy(file->buff + file->offset, buf, size);
	}

	file->offset += size;

	if ( file->offset > file->length )
	{
		file->length = file->offset;
	}

	return size;
}

/*
====================
VFS_Insert

Insert new portion at current position (not overwrite)
====================
*/
size_t VFS_Insert(vfile_t* file, const void* buf, size_t size)
{
	byte* backup;
	size_t rp_size;

	if ( !file || !file->buff || size <= 0 )
	{
		return (size_t)-1;
	}

	if ( (file->length + size) >= file->buffsize )
	{
		size_t newsize = file->length + size + FILE_BUFF_SIZE;
		if ( file->buffsize < newsize )
		{
			// reallocate buffer now
			file->buff = (byte*)Mem_Realloc(file->buff, newsize);
			file->buffsize = newsize;  // update buffsize
		}
	}

	// backup right part
	rp_size = file->length - file->offset;
	backup = (byte*)Mem_Alloc(rp_size);
	memcpy(backup, file->buff + file->offset, rp_size);

	// insert into buffer
	memcpy(file->buff + file->offset, buf, size);
	file->offset += size;

	// write right part buffer
	memcpy(file->buff + file->offset, backup, rp_size);
	Mem_Free(backup);

	if ( (file->offset + rp_size) > file->length )
	{
		file->length = file->offset + rp_size;
	}

	return file->length;
}

/*
====================
VFS_GetBuffer

Get buffer pointer
====================
*/
byte* VFS_GetBuffer(vfile_t* file)
{
	if ( !file )
		return NULL;
	return file->buff;
}

/*
====================
VFS_GetSize

Get buffer size
====================
*/
size_t VFS_GetSize(vfile_t* file)
{
	if ( !file )
		return 0;
	return file->length;
}

/*
====================
VFS_Tell

get current position
====================
*/
size_t VFS_Tell(vfile_t* file)
{
	if ( !file )
		return 0;
	return file->offset;
}

/*
====================
VFS_Eof

indicates at reached end of virtual file
====================
*/
bool VFS_Eof(vfile_t* file)
{
	if ( !file )
		return 1;
	return (file->offset == file->length) ? true : false;
}

/*
====================
VFS_Print

Print a string into a file
====================
*/
size_t VFS_Print(vfile_t* file, const char* msg)
{
	return VFS_Write(file, msg, Q_strlen(msg));
}

/*
====================
VFS_IPrint

Insert a string into a file
====================
*/
size_t VFS_IPrint(vfile_t* file, const char* msg)
{
	return VFS_Insert(file, msg, Q_strlen(msg));
}

/*
====================
VFS_VPrintf

Print a formatted string into a buffer
====================
*/
size_t VFS_VPrintf(vfile_t* file, const char* format, va_list ap)
{
	int len;
	char* tempbuff;
	int buff_size = MAX_TOKEN;

	while ( 1 )
	{
		tempbuff = (char*)Mem_Alloc(buff_size);
		len = Q_vsnprintf(tempbuff, buff_size, format, ap);
		if ( len >= 0 && len < buff_size )
		{
			break;
		}

		Mem_Free(tempbuff);
		buff_size <<= 1;
		tempbuff = NULL;
	}

	size_t len2 = VFS_Write(file, tempbuff, len);
	Mem_Free(tempbuff);
	return len2;
}

/*
====================
VFS_VIPrintf

Insert a formatted string into a buffer
====================
*/
size_t VFS_VIPrintf(vfile_t* file, const char* format, va_list ap)
{
	int len;
	char* tempbuff;
	int buff_size = MAX_TOKEN;

	while ( 1 )
	{
		tempbuff = (char*)Mem_Alloc(buff_size);
		len = Q_vsnprintf(tempbuff, buff_size, format, ap);
		if ( len >= 0 && len < buff_size )
			break;

		Mem_Free(tempbuff);
		buff_size <<= 1;
		tempbuff = NULL;
	}

	size_t len2 = VFS_Insert(file, tempbuff, len);
	Mem_Free(tempbuff);
	return len2;
}

/*
====================
VFS_Printf

Print a formatted string into a buffer
====================
*/
size_t VFS_Printf(vfile_t* file, const char* format, ...)
{
	size_t result;
	va_list args;

	va_start(args, format);
	result = VFS_VPrintf(file, format, args);
	va_end(args);
	return result;
}

/*
====================
VFS_IPrintf

Print a formatted string into a buffer
====================
*/
size_t VFS_IPrintf(vfile_t* file, const char* format, ...)
{
	size_t result;
	va_list args;

	va_start(args, format);
	result = VFS_VIPrintf(file, format, args);
	va_end(args);
	return result;
}

/*
====================
VFS_Seek

seeking into buffer
====================
*/
int VFS_Seek(vfile_t* file, int64_t offset, int whence)
{
	if ( !file )
	{
		return -1;
	}

	// compute the file offset
	switch ( whence )
	{
		case SEEK_CUR:
		{
			offset += file->offset;
			break;
		}

		case SEEK_SET:
		{
			break;
		}

		case SEEK_END:
		{
			offset += file->length;
			break;
		}

		default:
		{
			return -1;
		}
	}

	if ( offset < 0 || (size_t)offset > file->length )
	{
		return -1;
	}

	file->offset = (size_t)offset;
	return 0;
}

/*
====================
VFS_Getc

Get the next character of a file
====================
*/
char VFS_Getc(vfile_t* file)
{
	char c;
	if ( !VFS_Read(file, &c, 1) )
		return EOF;
	else
		return c;
}

/*
====================
VFS_Gets

Get the newline
====================
*/
int VFS_Gets(vfile_t* file, byte* string, size_t bufsize)
{
	int c;
	size_t end = 0;

	while ( 1 )
	{
		c = VFS_Getc(file);

		if ( c == '\r' || c == '\n' || c < 0 )
			break;

		if ( end < bufsize - 1 )
		{
			string[end++] = (char)c;
		}
	}
	string[end] = 0;

	// remove \n following \r
	if ( c == '\r' )
	{
		c = VFS_Getc(file);

		if ( c != '\n' )
		{
			VFS_Seek(file, (size_t)-1, SEEK_CUR);  // rewind
		}
	}

	return c;
}

/*
====================
VFS_Close

Free the memory
====================
*/
void VFS_Close(vfile_t* file)
{
	if ( !file )
		return;

	if ( file->buff )
		Mem_Free(file->buff);
	Mem_Free(file);  // himself
}
