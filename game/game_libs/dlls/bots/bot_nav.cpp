
/**************************************************
 * Rho-Bot Source Code                            *
 * by Eric Bieschke (rhobot@botepidemic.com)      *
 * http://www.botepidemic.com/rhobot/             *
 **************************************************
 * You are free to do whatever you want with this *
 * source. All I ask is that (A) you tell me what *
 * you do with it so that I can check it out, (B) *
 * you credit me for any of my code that you use  *
 * in your project, and (C) if you are now, or    *
 * later become involved in the gaming industry   *
 * and think I would be a valuable contribution   *
 * to the team, contact me.   =]                  *
 **************************************************
 * If you have questions about the source, please *
 * don't hesitate to give me a ring. If you make  *
 * improvements to this source that you'd like to *
 * share with others, please let me know and I'll *
 * release your changes with the next source code *
 * release (you will be fully credited for all of *
 * your work). If I inadvertantly did not credit  *
 * either botman or Rich Whitehouse for code taken*
 * from their respective open-source bots, I      *
 * apologize (I did my best to comment what was   *
 * theirs), and if you let me know, I will credit *
 * them in the next source release.               *
 **************************************************/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "client.h"
#include "bot.h"
#include "nodes.h"
#include "PlatformLib/String.h"
#include "PlatformLib/File.h"

///////////////////////////////////////////////////////////////////////////////
// ActionChooseDirection
///////////////////////////////////////////////////////////////////////////////

DIRECTION CBaseBot::ActionChooseDirection(void)
{
	int arrayX = WorldGraph.FConvertGlobalToArray(pev->origin[VEC3_X]);
	int arrayY = WorldGraph.FConvertGlobalToArray(pev->origin[VEC3_Y]);
	int arrayZ = WorldGraph.FConvertGlobalToArray(pev->origin[VEC3_Z]);

	int AreaNavPropensityTotal =
		(WorldGraph.FGetNavPropensity(arrayX + 1, arrayY + 1, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX + 1, arrayY, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX + 1, arrayY - 1, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX, arrayY + 1, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX, arrayY - 1, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX - 1, arrayY + 1, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX - 1, arrayY, arrayZ) +
		 WorldGraph.FGetNavPropensity(arrayX - 1, arrayY - 1, arrayZ));

	if ( AreaNavPropensityTotal == 0 )
	{
		return UNKNOWN;
	}

	// 29-July-2001:	ensures randomDirectionSelector will be > AreaNavPropensityTotal
	//	int randomDirectionSelector = RANDOM_LONG( 0, AreaNavPropensityTotal );
	int randomDirectionSelector;
	if ( AreaNavPropensityTotal == 1 )
		randomDirectionSelector = 1;
	else
		randomDirectionSelector = RANDOM_LONG(1, AreaNavPropensityTotal);
	AreaNavPropensityTotal--;
	// 29-July-2001: end

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX + 1, arrayY, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return NORTH;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX + 1, arrayY - 1, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return NORTHEAST;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX, arrayY - 1, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return EAST;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX - 1, arrayY - 1, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return SOUTHEAST;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX - 1, arrayY, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return SOUTH;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX - 1, arrayY + 1, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return SOUTHWEST;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX, arrayY + 1, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return WEST;
	}

	AreaNavPropensityTotal -= WorldGraph.FGetNavPropensity(arrayX + 1, arrayY + 1, arrayZ);
	if ( randomDirectionSelector > AreaNavPropensityTotal )
	{
		return NORTHWEST;
	}

	return UNKNOWN;
}

///////////////////////////////////////////////////////////////////////////////
// FConvertArrayToGlobal
///////////////////////////////////////////////////////////////////////////////

float CGraph::FConvertArrayToGlobal(int array)
{
	// 31-July-2001: return the center of the array box
	//	return ( array * NAV_GRIDBOX_SIZE ) - 4096;
	return ((array + 0.5f) * NAV_GRIDBOX_SIZE) - 4096;
	// 31-July-2001: end
}

///////////////////////////////////////////////////////////////////////////////
// FConvertGlobalToArray
///////////////////////////////////////////////////////////////////////////////

int CGraph::FConvertGlobalToArray(float location)
{
	// 30-July-2001: add bounds checking
	int iArray = static_cast<int>((4096 + location) / NAV_GRIDBOX_SIZE);
	return (BoundsCheck(iArray));
	//	return (4096 + location) / NAV_GRIDBOX_SIZE;
	// 30-July-2001: end
}

// 30-July-2001: add bounds checking
///////////////////////////////////////////////////////////////////////////////
// BoundsCheck
///////////////////////////////////////////////////////////////////////////////

int CGraph::BoundsCheck(int array)
{
	if ( array < 0 )
		return 0;
	else if ( array > NAV_ARRAY_MAX )
		return NAV_ARRAY_MAX;
	else
		return array;
}
// 30-July-2001: end

///////////////////////////////////////////////////////////////////////////////
// FGetNavPropensity
///////////////////////////////////////////////////////////////////////////////

NAV_ARRAY_TYPE CGraph::FGetNavPropensity(int arrayX, int arrayY, int arrayZ)
{
	// 30-July-2001: add bounds checking.  Out of bounds, return 0
	if ( arrayX != BoundsCheck(arrayX) || arrayY != BoundsCheck(arrayY) || arrayZ != BoundsCheck(arrayZ) )
	{
		return 0;
	}
	// 30-July-2001: end

	if ( NavigationArray[arrayX][arrayY][arrayZ] < 0 )
	{
		return 0;
	}
	else
	{
		return NavigationArray[arrayX][arrayY][arrayZ];
	}
}

///////////////////////////////////////////////////////////////////////////////
// FLoadBotNav
///////////////////////////////////////////////////////////////////////////////

int CGraph::FLoadBotNav(const char* szMapName)
{
	char navFileName[128];

	PlatformLib_SNPrintF(navFileName, sizeof(navFileName), "rhodmc/nav/%s.nav ", szMapName);

	ALERT(at_console, navFileName);

	slurp_array(WorldGraph.NavigationArray, navFileName);

	ALERT(at_console, "NavigationArray Loaded.\n");

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// slurp_array (Ben)
///////////////////////////////////////////////////////////////////////////////

void CGraph::slurp_array(
	NAV_ARRAY_TYPE space_array[NAV_ARRAY_SIZE][NAV_ARRAY_SIZE][NAV_ARRAY_SIZE],
	const char* filename)
{
	std::ifstream navFile(filename);
	char line[25];  // probably replace with something sane

	while ( navFile.getline(line, sizeof(line)) )
	{
		parse_nav_line(space_array, line);
	}
	navFile.close();
}

///////////////////////////////////////////////////////////////////////////////
// parse_nav_line (Ben)
///////////////////////////////////////////////////////////////////////////////

void CGraph::parse_nav_line(NAV_ARRAY_TYPE space_array[NAV_ARRAY_SIZE][NAV_ARRAY_SIZE][NAV_ARRAY_SIZE], char* line)
{
	char xBuf[6], yBuf[6], zBuf[6], valueBuf[12];
	// read in the stuff (nextLine function is a good thing)
	nextItem(xBuf, &line, '.');
	nextItem(yBuf, &line, '.');
	nextItem(zBuf, &line, '=');
	nextItem(valueBuf, &line, '\n');
	int x, y, z, value;
	x = atoi(xBuf);
	y = atoi(yBuf);
	z = atoi(zBuf);
	value = atoi(valueBuf);
	space_array[x][y][z] = static_cast<NAV_ARRAY_TYPE>(value);
}

///////////////////////////////////////////////////////////////////////////////
// nextItem (Ben)
///////////////////////////////////////////////////////////////////////////////

void nextItem(char* dest, char** line, char delimiter, char comment)
{
	// should work. returns the next item in this file, and bumps the buffer pointer accordingly
	while ( **line != delimiter && **line && **line != comment )
	{
		*dest = **line;
		dest++;
		(*line)++;
	}
	if ( **line != comment && strlen(*line) != 0 )
		(*line)++;  // preserve comments
	*dest = '\0';  // add null terminator
}

///////////////////////////////////////////////////////////////////////////////
// HalfNavArray
///////////////////////////////////////////////////////////////////////////////

void CGraph::HalfNavArray(void)
{
	for ( int i = 0; i < NAV_ARRAY_SIZE; i++ )
	{
		for ( int j = 0; j < NAV_ARRAY_SIZE; j++ )
		{
			for ( int k = 0; k < NAV_ARRAY_SIZE; k++ )
			{
				if ( NavigationArray[i][j][k] != 1 )
				{
					NavigationArray[i][j][k] /= 2;
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// InitNavArray
///////////////////////////////////////////////////////////////////////////////

void CGraph::InitNavArray(void)
{
	for ( int i = 0; i < NAV_ARRAY_SIZE; i++ )
	{
		for ( int j = 0; j < NAV_ARRAY_SIZE; j++ )
		{
			for ( int k = 0; k < NAV_ARRAY_SIZE; k++ )
			{
				NavigationArray[i][j][k] = 1;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// IsInWorld
///////////////////////////////////////////////////////////////////////////////

BOOL CGraph::IsInWorld(const Vector& location)
{
	if ( location.x > 4096 )
		return FALSE;
	if ( location.y > 4096 )
		return FALSE;
	if ( location.z > 4096 )
		return FALSE;
	if ( location.x < -4096 )
		return FALSE;
	if ( location.y < -4096 )
		return FALSE;
	if ( location.z < -4096 )
		return FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// MarkLocationFavorable
///////////////////////////////////////////////////////////////////////////////

void CGraph::MarkLocationFavorable(const Vector& location)
{
	if ( !IsInWorld(location) )
	{
		return;
	}

	int arrayX = FConvertGlobalToArray(location.x);
	int arrayY = FConvertGlobalToArray(location.y);
	int arrayZ = FConvertGlobalToArray(location.z);

	if ( NavigationArray[arrayX][arrayY][arrayZ] < 32767 )
	{  // protecting NavigationArray from NAV_ARRAY_TYPE wraparound
		NavigationArray[arrayX][arrayY][arrayZ]++;
	}
	else
	{
		HalfNavArray();
	}
}

///////////////////////////////////////////////////////////////////////////////
// MarkLocationUnfavorable
///////////////////////////////////////////////////////////////////////////////

void CGraph::MarkLocationUnfavorable(const Vector& location)
{
	if ( !IsInWorld(location) )
	{
		return;
	}

	int arrayX = FConvertGlobalToArray(location.x);
	int arrayY = FConvertGlobalToArray(location.y);
	int arrayZ = FConvertGlobalToArray(location.z);

	if ( NavigationArray[arrayX][arrayY][arrayZ] > -32767 )
	{  // protecting NavigationArray from NAV_ARRAY_TYPE wraparound
		NavigationArray[arrayX][arrayY][arrayZ]--;
	}
	else
	{
		HalfNavArray();
	}
}

///////////////////////////////////////////////////////////////////////////////
// SaveNavToFile
///////////////////////////////////////////////////////////////////////////////

void CGraph::SaveNavToFile(void)
{
	FILE* navFile;
	char navFileName[50] = "default";

	PlatformLib_SNPrintF(navFileName, sizeof(navFileName), "rhodmc/nav/%s.nav", (char*)STRING(gpGlobals->mapname));

	navFile = PlatformLib_FOpen(navFileName, "w");

	char printThis[64];

	for ( int i = 0; i < NAV_ARRAY_SIZE; i++ )
	{
		for ( int j = 0; j < NAV_ARRAY_SIZE; j++ )
		{
			for ( int k = 0; k < NAV_ARRAY_SIZE; k++ )
			{
				if ( WorldGraph.NavigationArray[i][j][k] != 1 )
				{
					PlatformLib_SNPrintF(
						printThis,
						sizeof(printThis),
						"%d.%d.%d=%d\n",
						i,
						j,
						k,
						WorldGraph.NavigationArray[i][j][k]);
					fprintf(navFile, "%s", printThis);
				}
			}
		}
	}

	fclose(navFile);
}
