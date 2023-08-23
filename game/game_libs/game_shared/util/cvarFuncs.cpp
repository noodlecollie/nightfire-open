#include "util/cvarFuncs.h"
#include "PlatformLib/String.h"

Vector2D CvarStringToVector2D(const char* str, bool* ok)
{
	float x = 0.0f;
	float y = 0.0f;

	const int numValues = PlatformLib_SScanF(str, "%f %f", &x, &y);

	if ( numValues < 2 )
	{
		if ( ok )
		{
			*ok = false;
		}

		return Vector2D();
	}

	if ( ok )
	{
		*ok = true;
	}

	return Vector2D(x, y);
}

void Vector2DToCvarString(const Vector2D& vec, char* buffer, size_t length)
{
	PlatformLib_SNPrintF(buffer, length, "%f %f", vec.x, vec.y);
}
