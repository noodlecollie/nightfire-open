#include "CompileTools/conprint.h"
#include "randomfloat.h"

#define IA 16807
#define IM 2147483647
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1 + (IM - 1) / NTAB)
#define MAX_RANDOM_RANGE 0x7FFFFFFFUL

// fran1 -- return a random floating-point number on the interval [0,1)
//
#define AM (1.0f / IM)
#define EPS 1.2e-7f
#define RNMX (1.0f - EPS)

static int m_idum = 0;
static int m_iy = 0;
static int m_iv[NTAB];

static int GenerateRandomNumber(void)
{
	int j, k;

	if ( m_idum <= 0 || !m_iy )
	{
		if ( -(m_idum) < 1 )
		{
			m_idum = 1;
		}
		else
		{
			m_idum = -(m_idum);
		}

		for ( j = NTAB + 7; j >= 0; j-- )
		{
			k = (m_idum) / IQ;
			m_idum = IA * (m_idum - k * IQ) - IR * k;

			if ( m_idum < 0 )
			{
				m_idum += IM;
			}

			if ( j < NTAB )
			{
				m_iv[j] = m_idum;
			}
		}

		m_iy = m_iv[0];
	}

	k = (m_idum) / IQ;
	m_idum = IA * (m_idum - k * IQ) - IR * k;

	if ( m_idum < 0 )
	{
		m_idum += IM;
	}

	j = m_iy / NDIV;

	// We're seeing some strange memory corruption in the contents of s_pUniformStream.
	// Perhaps it's being caused by something writing past the end of this array?
	// Bounds-check in release to see if that's the case.
	if ( j >= NTAB || j < 0 )
	{
		MsgDev(D_WARN, "GenerateRandomNumber had an array overrun: tried to write to element %d of 0..31.\n", j);
		j = (j % NTAB) & 0x7fffffff;
	}

	m_iy = m_iv[j];
	m_iv[j] = m_idum;

	return m_iy;
}

float RandomFloat(float flLow, float flHigh)
{
	// float in [0,1)
	float fl = AM * static_cast<float>(GenerateRandomNumber());

	if ( fl > RNMX )
	{
		fl = RNMX;
	}

	return (fl * (flHigh - flLow)) + flLow;  // float in [low,high)
}
