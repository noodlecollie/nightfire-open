#include "common/botprofiles.h"
#include "common/engine_mempool.h"
#include "MemPool/MemPool.h"
#include "common.h"

#define MAX_PROFILES 64

typedef struct BotProfile
{
	char* profileName;
	char* skin;
	char* playerName;
} BotProfile;

static MemPool_Handle g_BotMemPool;
static qboolean g_MemPoolAllocated = false;
static BotProfile* g_BotProfiles = NULL;
static size_t g_NumBotProfiles = 0;

static BotProfile* CreateBotProfile()
{
	return (BotProfile*)Mem_Calloc(g_BotMemPool, sizeof(BotProfile));
}

static void DestroyBotProfile(BotProfile* profile)
{
	if ( profile->profileName )
	{
		Mem_Free(profile->profileName);
	}

	if ( profile->skin )
	{
		Mem_Free(profile->skin);
	}

	if ( profile->playerName )
	{
		Mem_Free(profile->playerName);
	}

	Mem_Free(profile);
}

static char* StrDup(const char* str)
{
	if ( !str || !(*str) )
	{
		return (char*)Mem_Calloc(g_BotMemPool, 1);
	}

	size_t length = Q_strlen(str);
	char* out = (char*)Mem_Malloc(g_BotMemPool, length + 1);
	Q_strcpy(out, length + 1, str);
	return out;
}

static void IncrementDataPointers(char* cursor, char** data, int* length)
{
	if ( cursor )
	{
		// Decrement length by how much we parsed.
		*length -= (int)(cursor - (*data));
		*data = cursor;
	}
	else
	{
		// No more data.
		*data += *length;
		*length = 0;
	}
}

static qboolean ParseSingleLine(char** data, int* length)
{
	char* cursor = *data;
	int tokenLength = 0;

	char profileName[64];

	cursor = COM_ParseFileSafe(cursor, profileName, sizeof(profileName), PFILE_HANDLENEWLINE, &tokenLength, NULL);

	if ( !cursor || tokenLength < 1 || profileName[0] == '\n' )
	{
		// Empty line or end of file. This is fine, just quit here.
		IncrementDataPointers(cursor, data, length);
		return true;
	}

	char skin[64];
	cursor = COM_ParseFileSafe(cursor, skin, sizeof(skin), PFILE_HANDLENEWLINE, &tokenLength, NULL);

	if ( !cursor || tokenLength < 1 || skin[0] == '\n' )
	{
		Con_Printf(S_WARN "Failed to parse bot profile skin\n");
		return false;
	}

	char playerName[64];
	cursor = COM_ParseFileSafe(cursor, playerName, sizeof(playerName), PFILE_HANDLENEWLINE, &tokenLength, NULL);

	if ( tokenLength < 1 || playerName[0] == '\n' )
	{
		Con_Printf(S_WARN "Failed to parse bot profile player name\n");
		return false;
	}

	// If we have more data, make sure it's the end of the line
	if ( cursor )
	{
		char endOfLine[2];
		cursor = COM_ParseFileSafe(cursor, endOfLine, sizeof(endOfLine), PFILE_HANDLENEWLINE, &tokenLength, NULL);

		if ( cursor && (tokenLength < 1 || endOfLine[0] != '\n') )
		{
			Con_Printf(S_WARN "Expected end of line after end of bot profile %zu\n", g_NumBotProfiles + 1);
			return false;
		}
	}

	if ( g_NumBotProfiles < MAX_PROFILES )
	{
		BotProfile* profile = &g_BotProfiles[g_NumBotProfiles++];

		profile->profileName = StrDup(profileName);
		profile->playerName = StrDup(playerName);
		profile->skin = StrDup(skin);

		Con_DPrintf(
			"Created bot profile %zu: %s is named \"%s\" (%s)\n",
			g_NumBotProfiles,
			profile->profileName,
			profile->playerName,
			profile->skin
		);
	}
	else
	{
		Con_Printf(S_WARN "Reached maximum of %zu bot profiles, ignoring further profiles\n", MAX_PROFILES);
	}

	IncrementDataPointers(cursor, data, length);
	return true;
}

static qboolean ParseFromFileData(const char* fileData, int fileLength)
{
	g_BotProfiles = (BotProfile*)Mem_Malloc(g_BotMemPool, MAX_PROFILES * sizeof(BotProfile));
	g_NumBotProfiles = 0;

	char* cursor = (char*)fileData;
	qboolean parseSucceeded = true;

	while ( parseSucceeded && fileLength > 0 )
	{
		parseSucceeded = ParseSingleLine(&cursor, &fileLength);
	}

	if ( parseSucceeded )
	{
		if ( g_NumBotProfiles > 0 )
		{
			Con_Printf("Successfully parsed %zu bot profiles from profile list\n", g_NumBotProfiles);

			// Trim unused elements off the list.
			g_BotProfiles = Mem_Realloc(g_BotMemPool, g_BotProfiles, g_NumBotProfiles * sizeof(BotProfile));
		}
		else
		{
			Con_Printf("Parsed 0 bot profiles from profile list\n", g_NumBotProfiles);
			Mem_Free(g_BotProfiles);
			g_BotProfiles = 0;
		}
	}
	else
	{
		Mem_Free(g_BotProfiles);
		g_BotProfiles = NULL;
		g_NumBotProfiles = 0;
	}

	return parseSucceeded;
}

qboolean BotProfiles_ParseList(const char* listPath)
{
	if ( !g_MemPoolAllocated )
	{
		g_BotMemPool = Mem_AllocPool("Bot Profiles");
		g_MemPoolAllocated = true;
	}

	if ( !listPath || !(*listPath) )
	{
		return false;
	}

	int fileLength = 0;
	byte* fileData = COM_LoadFile(listPath, &fileLength);

	if ( !fileData || fileLength < 1 )
	{
		Con_Printf(S_WARN "Could not open bot profile file: %s\n", listPath);

		if ( fileData )
		{
			Mem_Free(fileData);
		}

		return false;
	}

	const qboolean success = ParseFromFileData((const char*)fileData, fileLength);

	Mem_Free(fileData);
	return success;
}

void BotProfiles_Clear(void)
{
	if ( g_BotProfiles )
	{
		Mem_Free(g_BotProfiles);
		g_BotProfiles = NULL;
	}

	g_NumBotProfiles = 0;

	if ( g_MemPoolAllocated )
	{
		Mem_FreePool(&g_BotMemPool);
		g_MemPoolAllocated = false;
	}
}

size_t BotProfiles_Count(void)
{
	return g_NumBotProfiles;
}

#if XASH_ENGINE_TESTS()
#include "common/tests.h"

#define ASSERT_NOT_INITIALISED() \
	TASSERT(!g_MemPoolAllocated); \
	TASSERT(!g_BotProfiles); \
	TASSERT_EQi((int)g_NumBotProfiles, 0);

#define INITIALISE() \
	if ( !g_MemPoolAllocated ) \
	{ \
		g_BotMemPool = Mem_AllocPool(" Bot Profiles"); \
		g_MemPoolAllocated = true; \
	}

static void Test_CheckNullFile(void)
{
	ASSERT_NOT_INITIALISED()

	const qboolean success = BotProfiles_ParseList(NULL);

	TASSERT(!success);
	TASSERT(g_MemPoolAllocated);
	TASSERT(!g_BotProfiles);
	TASSERT_EQi((int)g_NumBotProfiles, 0);

	BotProfiles_Clear();
	ASSERT_NOT_INITIALISED()
}

static void Test_CheckEmptyFiles(void)
{
	static const char* const CONTENTS_LIST[] = {
		"",
		"   ",
		"\t\t\t",
		"\n\n\n",
		" \t\n",
		"  \n   \n\n  ",
	};

	for ( size_t index = 0; index < SIZE_OF_ARRAY(CONTENTS_LIST); ++index )
	{
		Msg("Test_CheckEmptyFiles: Running iteration %zu\n", index + 1);

		ASSERT_NOT_INITIALISED()
		INITIALISE()

		const char* data = CONTENTS_LIST[index];
		const qboolean success = ParseFromFileData(data, (int)Q_strlen(data));

		// Parsing should still succeed, as an empty file is not technically invalid.
		TASSERT(success);

		TASSERT(g_MemPoolAllocated);
		TASSERT(!g_BotProfiles);
		TASSERT_EQi((int)g_NumBotProfiles, 0);

		BotProfiles_Clear();
		ASSERT_NOT_INITIALISED()
	}
}

static void Test_CheckValidFile(void)
{
	static const char* const CONTENTS =
		"bot_one models/player/mp_jaws.mdl Jaws\n"
		"bot_two models/player/mp_pussy_galore.mdl \"Pussy Galore\"\n";

	ASSERT_NOT_INITIALISED()
	INITIALISE()

	const qboolean success = ParseFromFileData(CONTENTS, (int)Q_strlen(CONTENTS));

	TASSERT(success);
	TASSERT(g_MemPoolAllocated);
	TASSERT(g_BotProfiles);
	TASSERT_EQi((int)g_NumBotProfiles, 2);

	TASSERT_STR(g_BotProfiles[0].profileName, "bot_one");
	TASSERT_STR(g_BotProfiles[0].skin, "models/player/mp_jaws.mdl");
	TASSERT_STR(g_BotProfiles[0].playerName, "Jaws");

	TASSERT_STR(g_BotProfiles[1].profileName, "bot_two");
	TASSERT_STR(g_BotProfiles[1].skin, "models/player/mp_pussy_galore.mdl");
	TASSERT_STR(g_BotProfiles[1].playerName, "Pussy Galore");

	BotProfiles_Clear();
	ASSERT_NOT_INITIALISED()
}

static void Test_CheckValidFileWithoutTrailingNewline(void)
{
	static const char* const CONTENTS =
		"bot_one models/player/mp_jaws.mdl Jaws\n"
		"bot_two models/player/mp_pussy_galore.mdl \"Pussy Galore\"";

	ASSERT_NOT_INITIALISED()
	INITIALISE()

	const qboolean success = ParseFromFileData(CONTENTS, (int)Q_strlen(CONTENTS));

	TASSERT(success);
	TASSERT(g_MemPoolAllocated);
	TASSERT(g_BotProfiles);
	TASSERT_EQi((int)g_NumBotProfiles, 2);

	TASSERT(Q_strcmp(g_BotProfiles[0].profileName, "bot_one") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[0].skin, "models/player/mp_jaws.mdl") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[0].playerName, "Jaws") == 0)

	TASSERT(Q_strcmp(g_BotProfiles[1].profileName, "bot_two") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[1].skin, "models/player/mp_pussy_galore.mdl") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[1].playerName, "Pussy Galore") == 0)

	BotProfiles_Clear();
	ASSERT_NOT_INITIALISED()
}

static void Test_CheckValidFileWithMultipleTrailingNewlines(void)
{
	static const char* const CONTENTS =
		"bot_one models/player/mp_jaws.mdl Jaws\n"
		"bot_two models/player/mp_pussy_galore.mdl \"Pussy Galore\"\n\n\n\n";

	ASSERT_NOT_INITIALISED()
	INITIALISE()

	const qboolean success = ParseFromFileData(CONTENTS, (int)Q_strlen(CONTENTS));

	TASSERT(success);
	TASSERT(g_MemPoolAllocated);
	TASSERT(g_BotProfiles);
	TASSERT_EQi((int)g_NumBotProfiles, 2);

	TASSERT(Q_strcmp(g_BotProfiles[0].profileName, "bot_one") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[0].skin, "models/player/mp_jaws.mdl") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[0].playerName, "Jaws") == 0)

	TASSERT(Q_strcmp(g_BotProfiles[1].profileName, "bot_two") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[1].skin, "models/player/mp_pussy_galore.mdl") == 0)
	TASSERT(Q_strcmp(g_BotProfiles[1].playerName, "Pussy Galore") == 0)

	BotProfiles_Clear();
	ASSERT_NOT_INITIALISED()
}

static void Test_CheckFilesWithIncompleteLines(void)
{
	static const char* const CONTENTS_LIST[] = {
		"bot_one",
		"bot_one\n",
		"bot_one models/player/mp_jaws.mdl",
		"bot_one models/player/mp_jaws.mdl\n",
	};

	for ( size_t index = 0; index < SIZE_OF_ARRAY(CONTENTS_LIST); ++index )
	{
		Msg("Test_CheckFilesWithIncompleteLines: Running iteration %zu\n", index + 1);
		ASSERT_NOT_INITIALISED()
		INITIALISE()

		const char* data = CONTENTS_LIST[index];
		const qboolean success = ParseFromFileData(data, (int)Q_strlen(data));

		TASSERT(!success);
		TASSERT(g_MemPoolAllocated);
		TASSERT(!g_BotProfiles);
		TASSERT_EQi((int)g_NumBotProfiles, 0);

		BotProfiles_Clear();
		ASSERT_NOT_INITIALISED()
	}
}

static void Test_CheckFilesWithExcessLines(void)
{
	static const char* const CONTENTS_LIST[] = {
		"bot_one models/player/mp_jaws.mdl Jaws Extra\n",
		"bot_one models/player/mp_jaws.mdl Jaws Extra",
		"bot_one models/player/mp_jaws.mdl \"Jaws\" Extra\n",
		"bot_one models/player/mp_jaws.mdl \"Jaws\" Extra",
	};

	for ( size_t index = 0; index < SIZE_OF_ARRAY(CONTENTS_LIST); ++index )
	{
		Msg("Test_CheckFilesWithExcessLines: Running iteration %zu\n", index + 1);
		ASSERT_NOT_INITIALISED()
		INITIALISE()

		const char* data = CONTENTS_LIST[index];
		const qboolean success = ParseFromFileData(data, (int)Q_strlen(data));

		TASSERT(!success);
		TASSERT(g_MemPoolAllocated);
		TASSERT(!g_BotProfiles);
		TASSERT_EQi((int)g_NumBotProfiles, 0);

		BotProfiles_Clear();
		ASSERT_NOT_INITIALISED()
	}
}

void Test_RunBotProfiles(void)
{
	TRUN(Test_CheckNullFile());
	TRUN(Test_CheckEmptyFiles());
	TRUN(Test_CheckValidFile());
	TRUN(Test_CheckValidFileWithoutTrailingNewline());
	TRUN(Test_CheckValidFileWithMultipleTrailingNewlines());
	TRUN(Test_CheckFilesWithIncompleteLines());
	TRUN(Test_CheckFilesWithExcessLines());
}
#endif
