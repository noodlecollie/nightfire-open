#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "mathlib.h"

static inline int FastChecksum( const void *const buffer, int bytes )
{
	char* buf = (char*)buffer;
	int checksum = 0;

	while( bytes-- )
	{
		checksum = rotl( checksum, 4 ) ^ (*buf);
		buf++;
	}

	return checksum;
}

#endif // CHECKSUM_H
