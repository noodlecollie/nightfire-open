#include "common/buildnum.h"
#include "CRTLib/crtlib.h"

int Q_buildnum(void)
{
	static const char* date = __DATE__;
	static const char* mon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	static const char mond[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	static int b = 0;
	int m = 0;
	int d = 0;
	int y = 0;

	if ( b != 0 )
	{
		return b;
	}

	for ( m = 0; m < 11; m++ )
	{
		if ( !Q_strnicmp(&date[0], mon[m], 3) )
		{
			break;
		}

		d += mond[m];
	}

	d += Q_atoi(&date[4]) - 1;
	y = Q_atoi(&date[7]) - 1900;
	b = d + (int)((y - 1) * 365.25f);

	if ( ((y % 4) == 0) && m > 1 )
	{
		b += 1;
	}

	b -= 41728;  // Apr 1 2015

	return b;
}

int Q_buildnum_compat(void)
{
	// do not touch this! Only author of Xash3D can increase buildnumbers!
	return 4529;
}
