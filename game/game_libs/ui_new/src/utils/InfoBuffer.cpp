#include "utils/InfoBuffer.h"
#include "EnginePublicAPI/com_model.h"

const char* Info_ValuePtrForKey(const char* s, const char* key)
{
	char pkey[MAX_INFO_STRING];
	static char value[2][MAX_INFO_STRING];  // use two buffers so compares work without stomping on each other
	static int valueindex;
	char* o;

	valueindex ^= 1;

	if ( *s == '\\' )
	{
		s++;
	}

	while ( 1 )
	{
		o = pkey;

		while ( *s != '\\' && *s != '\n' )
		{
			if ( !*s )
			{
				return "";
			}

			*o++ = *s++;
		}

		*o = 0;
		s++;

		o = value[valueindex];

		while ( *s != '\\' && *s != '\n' && *s )
		{
			if ( !*s )
			{
				return "";
			}

			*o++ = *s++;
		}

		*o = 0;

		if ( !strcmp(key, pkey) )
		{
			return value[valueindex];
		}

		if ( !*s )
		{
			break;
		}

		s++;
	}

	return "";
}

Rml::String Info_ValueStrForKey(const char* s, const char* key)
{
	return Rml::String(Info_ValuePtrForKey(s, key));
}
