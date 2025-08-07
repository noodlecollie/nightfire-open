/*
avi_stub.c - playing AVI files (stub)
Copyright (C) 2018 a1batross

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
#include "common/common.h"

#if !XASH_WIN32()

int AVI_GetVideoFrameNumber(movie_state_t* Avi, float time)
{
	(void)Avi;
	(void)time;

	return 0;
}

byte* AVI_GetVideoFrame(movie_state_t* Avi, long frame)
{
	(void)Avi;
	(void)frame;

	return NULL;
}

qboolean AVI_GetVideoInfo(movie_state_t* Avi, int* xres, int* yres, float* duration)
{
	(void)Avi;
	(void)xres;
	(void)yres;
	(void)duration;

	return false;
}

qboolean AVI_GetAudioInfo(movie_state_t* Avi, wavdata_t* snd_info)
{
	(void)Avi;
	(void)snd_info;

	return false;
}

int AVI_GetAudioChunk(movie_state_t* Avi, char* audiodata, int offset, int length)
{
	(void)Avi;
	(void)audiodata;
	(void)offset;
	(void)length;

	return 0;
}

void AVI_OpenVideo(movie_state_t* Avi, const char* filename, qboolean load_audio, int quiet)
{
	(void)Avi;
	(void)filename;
	(void)load_audio;
	(void)quiet;
}

movie_state_t* AVI_LoadVideo(const char* filename, qboolean load_audio)
{
	(void)filename;
	(void)load_audio;

	return NULL;
}

int AVI_TimeToSoundPosition(movie_state_t* Avi, int time)
{
	(void)Avi;
	(void)time;

	return 0;
}

void AVI_CloseVideo(movie_state_t* Avi)
{
	(void)Avi;
}

qboolean AVI_IsActive(movie_state_t* Avi)
{
	(void)Avi;

	return false;
}

void AVI_FreeVideo(movie_state_t* Avi)
{
	(void)Avi;
}

movie_state_t* AVI_GetState(int num)
{
	(void)num;

	return NULL;
}

qboolean AVI_Initailize(void)
{
	return false;
}

void AVI_Shutdown(void)
{

}

#else

void AVI_Dummy()
{
}

#endif // !XASH_WIN32()
