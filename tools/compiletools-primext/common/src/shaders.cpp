/*
shaders.cpp - parsing quake3 shaders for map-compile tools
Copyright (C) 2018 Uncle Mike

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "CompileTools/cmdlib.h"
#include "CompileTools/file_system.h"
#include "CompileTools/shaders.h"
#include "CompileTools/scriptlib.h"
#include "CompileTools/bspfile.h"
#include "CompileTools/zone.h"
#include "CompileTools/conprint.h"
#include "CRTLib/crtlib.h"

shaderInfo_t* shaderInfo = NULL;
int numShaderInfo;
surfaceParm_t surfaceParams[] = {
	{"default", CONTENTS_NONE, FSHADER_DEFAULT},
	{"lightgrid", CONTENTS_NONE, FSHADER_REMOVE},
	{"antiportal", CONTENTS_NONE, FSHADER_REMOVE},
	{"skip", CONTENTS_EMPTY, FSHADER_SKIP},
	{"origin", CONTENTS_ORIGIN, FSHADER_REMOVE},
	{"areaportal", CONTENTS_NONE, FSHADER_REMOVE},
	{"trans", CONTENTS_NONE, FSHADER_DETAIL},  // FIXME: this should be CONTENTS_TRANSLUCENT
	{"detail", CONTENTS_SOLID, FSHADER_DETAIL},
	{"structural", CONTENTS_SOLID, FSHADER_DEFAULT},
	{"hint", CONTENTS_EMPTY, FSHADER_HINT},
	{"nodraw", CONTENTS_SOLID, FSHADER_NODRAW},
	{"alphashadow", CONTENTS_NONE, FSHADER_DEFAULT},
	{"lightfilter", CONTENTS_NONE, FSHADER_DEFAULT},
	{"nolightmap", CONTENTS_NONE, FSHADER_NOLIGHTMAP},
	{"pointlight", CONTENTS_NONE, FSHADER_DEFAULT},
	{"nonsolid", CONTENTS_EMPTY, FSHADER_NOCLIP},
	{"trigger", CONTENTS_SOLID, FSHADER_TRIGGER},
	{"water", CONTENTS_WATER, FSHADER_NOLIGHTMAP},
	{"slime", CONTENTS_SLIME, FSHADER_NOLIGHTMAP},
	{"lava", CONTENTS_LAVA, FSHADER_DEFAULT},
	{"clip", CONTENTS_SOLID, FSHADER_CLIP},
	{"playerclip", CONTENTS_SOLID, FSHADER_CLIP},
	{"monsterclip", CONTENTS_EMPTY, FSHADER_CLIP},  // FIXME
	{"nodrop", CONTENTS_NONE, FSHADER_REMOVE},
	{"clusterportal", CONTENTS_NONE, FSHADER_REMOVE},
	{"donotenter", CONTENTS_NONE, FSHADER_REMOVE},
	{"botclip", CONTENTS_NONE, FSHADER_REMOVE},
	{"fog", CONTENTS_FOG, FSHADER_DEFAULT},
	{"sky", CONTENTS_SKY, FSHADER_NOLIGHTMAP},
	{"slick", CONTENTS_NONE, FSHADER_DEFAULT},
	{"noimpact", CONTENTS_NONE, FSHADER_DEFAULT},
	{"nomarks", CONTENTS_NONE, FSHADER_DEFAULT},
	{"ladder", CONTENTS_NONE, FSHADER_DEFAULT},
	{"nodamage", CONTENTS_NONE, FSHADER_DEFAULT},
	{"metalsteps", CONTENTS_NONE, FSHADER_DEFAULT},
	{"snowsteps", CONTENTS_NONE, FSHADER_DEFAULT},
	{"woodsteps", CONTENTS_NONE, FSHADER_DEFAULT},
	{"dmgthrough", CONTENTS_NONE, FSHADER_DEFAULT},
	{"flesh", CONTENTS_NONE, FSHADER_DEFAULT},
	{"nosteps", CONTENTS_NONE, FSHADER_DEFAULT},
	{"nodlight", CONTENTS_NONE, FSHADER_DEFAULT},
	{"dust", CONTENTS_NONE, FSHADER_DEFAULT},
	{NULL, 0, 0},
};

/*
=================
ApplySurfaceParm

applies a named surfaceparm to the supplied flags
=================
*/
bool ApplySurfaceParm(const char* name, int* contents, int* compileFlags)
{
	int fake;
	surfaceParm_t* sp;

	if ( name == NULL )
		return false;

	if ( contents == NULL )
		contents = &fake;

	if ( compileFlags == NULL )
		compileFlags = &fake;

	for ( sp = surfaceParams; sp->name != NULL; sp++ )
	{
		if ( !Q_stricmp(name, sp->name) )
		{
			if ( sp->contents != CONTENTS_NONE )
				*contents = sp->contents;
			SetBits(*compileFlags, sp->compileFlags);

			return true;
		}
	}

	return false;
}

/*
=================
AllocShaderInfo

allocates and initializes a new shader
=================
*/
static shaderInfo_t* AllocShaderInfo(void)
{
	shaderInfo_t* si;

	if ( shaderInfo == NULL )
	{
		shaderInfo = (shaderInfo_t*)Mem_Alloc(sizeof(shaderInfo_t) * MAX_SHADER_INFO);
		numShaderInfo = 0;
	}

	if ( numShaderInfo == MAX_SHADER_INFO )
		COM_FatalError("MAX_SHADER_INFO limit exceeded\n");

	si = &shaderInfo[numShaderInfo];
	numShaderInfo++;

	ApplySurfaceParm("default", &si->contents, &si->flags);

	si->lightmapSampleOffset = DEFAULT_LIGHTMAP_SAMPLE_OFFSET;
	si->vertexShadows = true;
	si->vertexScale = 1.0;

	Matrix3x3_LoadIdentity(si->mod);

	return si;
}

/*
=================
LoadShaderImages

loads a shader's images
=================
*/
static void LoadShaderImages(shaderInfo_t* si)
{
	char shaderPath[MAX_SHADERPATH];

	Q_snprintf(shaderPath, sizeof(shaderPath), "%s.tga", si->name);

	if ( FS_FileExists(si->editorImagePath, false) )
		Q_strncpy(si->imagePath, si->editorImagePath, sizeof(si->imagePath));
	else if ( FS_FileExists(shaderPath, false) )
		Q_strncpy(si->imagePath, shaderPath, sizeof(si->imagePath));
	else if ( FS_FileExists(si->lightImagePath, false) )
		Q_strncpy(si->imagePath, si->lightImagePath, sizeof(si->imagePath));
	else if ( FS_FileExists(si->implicitImagePath, false) )
		Q_strncpy(si->imagePath, si->implicitImagePath, sizeof(si->imagePath));

	// leave as error
	if ( !COM_CheckString(si->imagePath) )
	{
		Q_strcpy(si->imagePath, sizeof(si->imagePath), shaderPath);
	}

	if ( !COM_CheckString(si->implicitImagePath) && FS_FileExists(si->imagePath, false) )
	{
		Q_strcpy(si->implicitImagePath, sizeof(si->implicitImagePath), si->imagePath);
	}
}

/*
=================
FinishShader

loads a shader's images
=================
*/
static void FinishShader(shaderInfo_t* si)
{
	si->finished = true;
}

/*
=================
ShaderInfoForShader

finds a shaderinfo for a named shader
=================
*/
shaderInfo_t* ShaderInfoForShader(const char* shaderName)
{
	char shader[MAX_SHADERPATH];
	shaderInfo_t* si;

	// strip off extension
	Q_snprintf(shader, sizeof(shader), "textures/%s", shaderName);
	COM_StripExtension(shader);

	for ( int i = 0; i < numShaderInfo; i++ )
	{
		si = &shaderInfo[i];

		if ( !Q_stricmp(shader, si->name) )
		{
			if ( !si->finished )
			{
				LoadShaderImages(si);
				FinishShader(si);
			}
			return si;
		}
	}

	si = AllocShaderInfo();
	Q_strcpy(si->name, sizeof(si->name), shader);
	SetBits(si->flags, FSHADER_DEFAULTED);
	LoadShaderImages(si);
	FinishShader(si);

	return si;
}

/*
=================
ParseShaderFile

parses a shader file into discrete shaderInfo_t
=================
*/
static void ParseShaderFile(const char* filename)
{
#define SHADER_TEXT_MAX_LENGTH 16384

	char* shaderText;
	char* filedata;
	size_t filesize;
	shaderInfo_t* si = NULL;
	char* suffix;

	// NoodleCollie: This is currently horrible because this code appends
	// to this buffer using strcat, which will essentially call strlen
	// many times on a string that could be enormous. I won't touch this
	// now because I don't want to break anything that was already here,
	// but I do want to register my displeasure.
	shaderText = (char*)Mem_Alloc(SHADER_TEXT_MAX_LENGTH, C_STRING);

	if ( !shaderText )
	{
		return;
	}

	shaderText[0] = '\0';

	// load the shader
	filedata = (char*)FS_LoadFile(filename, &filesize, false);
	ParseFromMemory(filedata, filesize);

	while ( 1 )
	{
		// copy shader text to the shaderinfo
		if ( si != NULL && shaderText[0] != '\0' )
		{
			Q_strcat(shaderText, sizeof(shaderText), "\n");
			si->shaderText = copystring(shaderText);
		}

		shaderText[0] = '\0';

		if ( !GetToken(true) )
		{
			break;
		}

		// shader name is initial token
		si = AllocShaderInfo();
		Q_strncpy(si->name, token, sizeof(si->name));

		// ignore ":q3map" suffix
		suffix = Q_strstr(si->name, ":q3map");
		if ( suffix != NULL )
		{
			*suffix = '\0';
		}

		/* handle { } section */
		if ( !GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, true) )
		{
			break;
		}

		if ( Q_strcmp(token, "{") )
		{
			if ( si != NULL )
			{
				COM_FatalError(
					"%s, line %d: { not found!\nFound instead: %s\nLast known shader: %s",
					filename,
					scriptline,
					token,
					si->name);
			}
			else
			{
				COM_FatalError("%s, line %d: { not found!\nFound instead: %s", filename, scriptline, token);
			}
		}

		while ( 1 )
		{
			if ( !GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, true) )
			{
				break;
			}

			if ( !Q_strcmp(token, "}") )
			{
				break;
			}

			// -----------------------------------------------------------------
			// shader stages (passes)
			// -----------------------------------------------------------------

			// parse stage directives
			if ( !Q_strcmp(token, "{") )
			{
				while ( 1 )
				{
					if ( !GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, true) )
					{
						break;
					}

					if ( !Q_strcmp(token, "}") )
					{
						break;
					}

					// digest any images
					if ( !Q_stricmp(token, "map") || !Q_stricmp(token, "clampMap") || !Q_stricmp(token, "animMap") )
					{
						// skip one token for animated stages
						if ( !Q_stricmp(token, "animMap") )
						{
							GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);
						}

						GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);

						if ( token[0] != '*' && token[0] != '$' )
						{
							Q_strcpy(si->implicitImagePath, sizeof(si->implicitImagePath), token);
							COM_DefaultExtension(si->implicitImagePath, sizeof(si->implicitImagePath), ".tga");
						}
					}
				}
			}

			// -----------------------------------------------------------------
			// surfaceparm * directives
			// -----------------------------------------------------------------
			else if ( !Q_stricmp(token, "surfaceparm") )
			{
				GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);

				if ( !ApplySurfaceParm(token, &si->contents, &si->flags) )
				{
					MsgDev(D_WARN, "unknown surfaceparm: \"%s\"\n", token);
				}
			}

			// -----------------------------------------------------------------
			// image directives
			// -----------------------------------------------------------------

			// qer_editorimage <image>
			else if ( !Q_stricmp(token, "qer_editorImage") )
			{
				GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);

				Q_strcpy(si->editorImagePath, sizeof(si->editorImagePath), token);
				COM_DefaultExtension(si->editorImagePath, sizeof(si->editorImagePath), ".tga");
			}

			// q3map_lightimage <image>
			else if ( !Q_stricmp(token, "q3map_lightImage") )
			{
				GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);

				Q_strcpy(si->lightImagePath, sizeof(si->lightImagePath), token);
				COM_DefaultExtension(si->lightImagePath, sizeof(si->lightImagePath), ".tga");
			}

			// skyparms <outer image> <cloud height> <inner image>
			else if ( !Q_stricmp(token, "skyParms") )
			{
				GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);

				if ( Q_stricmp(token, "-") && Q_stricmp(token, "full") )
				{
					Q_strcpy(si->skyParmsImageBase, sizeof(si->skyParmsImageBase), token);

					// use top image as sky light image
					if ( si->lightImagePath[0] == '\0' )
					{
						Q_snprintf(si->lightImagePath, sizeof(si->lightImagePath), "%s_up.tga", si->skyParmsImageBase);
					}
				}

				// skip rest of line
				GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);
				GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false);
			}

			// -----------------------------------------------------------------
			// skip
			// -----------------------------------------------------------------

			// ignore all other tokens on the line
			while ( TokenAvailable() && GetTokenAppend(shaderText, SHADER_TEXT_MAX_LENGTH, false) )
			{
				// Skip
			}
		}
	}

	Mem_Free(filedata, C_FILESYSTEM);
	Mem_Free(shaderText, C_STRING);

#undef SHADER_TEXT_MAX_LENGTH
}

void LoadShaderInfo(void)
{
	search_t* search = FS_Search("scripts/*.shader", true, false);
	if ( !search )
		return;

	for ( int i = 0; i < search->numfilenames; i++ )
		ParseShaderFile(search->filenames[i]);
	Mem_Free(search, C_FILESYSTEM);

	MsgDev(D_INFO, "%9d shaderInfo\n", numShaderInfo);
}

void FreeShaderInfo(void)
{
	shaderInfo_t* si;

	for ( int i = 0; i < numShaderInfo; i++ )
	{
		si = &shaderInfo[i];
		freestring(si->shaderText);
	}

	Mem_Free(shaderInfo);
	shaderInfo = NULL;
}
