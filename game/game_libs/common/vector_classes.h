#pragma once

#include <math.h>

typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];	// x,y,z,w

// Some of the pm_shared stuff is C instead of C++, so don't use the classes in this situation.
#ifdef __cplusplus
//=========================================================
// 2DVector - used for many pathfinding and many other
// operations that are treated as planar rather than 3d.
//=========================================================
class Vector2D
{
public:
	inline Vector2D(void): x( 0.0f ), y( 0.0f )						{ }
	inline Vector2D(float X, float Y): x( 0.0f ), y( 0.0f )				{ x = X; y = Y; }
	inline Vector2D operator+(const Vector2D& v)	const	{ return Vector2D( x + v.x, y + v.y );	}
	inline Vector2D operator-(const Vector2D& v)	const	{ return Vector2D( x - v.x, y - v.y );	}
	inline Vector2D operator*(float fl)		const	{ return Vector2D( x * fl, y * fl );	}
	inline Vector2D operator/(float fl)		const	{ return Vector2D( x / fl, y / fl );	}
	operator float *()								{ return &x; } // Vectors will now automatically convert to float * when needed
	operator const float *() const					{ return &x; } // Vectors will now automatically convert to float * when needed

	inline float Length(void)			const	{ return static_cast<float>(sqrt(x * x + y * y ));		}

	inline Vector2D Normalize ( void ) const
	{
		float flLen = Length();
		if( flLen == 0 )
		{
			return Vector2D( 0, 0 );
		}
		else
		{
			flLen = 1 / flLen;
			return Vector2D( x * flLen, y * flLen );
		}
	}

	inline float& operator [](int index)
	{
		return operator float*()[index];
	}

	inline const float& operator [](int index) const
	{
		return operator const float*()[index];
	}

	vec_t	x, y;
};

inline float DotProduct( const Vector2D& a, const Vector2D& b ) { return( a.x * b.x + a.y * b.y ); }
inline Vector2D operator*( float fl, const Vector2D& v ) { return v * fl; }

//=========================================================
// 3D Vector
//=========================================================
class Vector						// same data-layout as engine's vec3_t,
{								//		which is a vec_t[3]
public:
	// Construction/destruction
	inline Vector( void ): x( 0.0f ), y( 0.0f ), z( 0.0f )								{ }
	inline Vector( float X, float Y, float Z ): x( 0.0f ), y( 0.0f ), z( 0.0f )	{ x = X; y = Y; z = Z;				}
	inline Vector( const Vector& v ): x( 0.0f ), y( 0.0f ), z( 0.0f )	{ x = v.x; y = v.y; z = v.z;			}
	inline explicit Vector( const float rgfl[3] ): x( 0.0f ), y( 0.0f ), z( 0.0f )	{ x = rgfl[0]; y = rgfl[1]; z = rgfl[2];	}

	// Operators
	inline Vector operator-( void ) const			{ return Vector( -x, -y, -z );			}
	inline int operator==( const Vector& v ) const		{ return x==v.x && y==v.y && z==v.z;		}
	inline int operator!=( const Vector& v ) const		{ return !( *this==v );				}
	inline Vector operator+( const Vector& v ) const	{ return Vector( x + v.x, y + v.y, z + v.z );	}
	inline Vector operator-( const Vector& v ) const	{ return Vector( x - v.x, y - v.y, z - v.z );	}
	inline Vector operator*( float fl) const		{ return Vector( x * fl, y * fl, z * fl );	}
	inline Vector operator/( float fl) const		{ return Vector( x / fl, y / fl, z / fl );	}

	// Methods
	inline void CopyToArray( float* rgfl ) const		{ rgfl[0] = x, rgfl[1] = y, rgfl[2] = z; }
	inline float Length( void ) const					{ return static_cast<float>(sqrt( x * x + y * y + z * z )); }
	operator float *()								{ return &x; } // Vectors will now automatically convert to float * when needed
	operator const float *() const					{ return &x; } // Vectors will now automatically convert to float * when needed
	inline Vector Normalize( void ) const
	{
		float flLen = Length();
		if( flLen == 0 ) return Vector( 0, 0, 1 ); // ????
		flLen = 1 / flLen;
		return Vector( x * flLen, y * flLen, z * flLen );
	}

	inline Vector2D Make2D( void ) const
	{
		Vector2D	Vec2;

		Vec2.x = x;
		Vec2.y = y;

		return Vec2;
	}
	inline float Length2D( void ) const		{ return static_cast<float>(sqrt( x * x + y * y )); }

	inline float& operator [](int index)
	{
		return operator float*()[index];
	}

	inline const float& operator [](int index) const
	{
		return operator const float*()[index];
	}

	// Members
	vec_t x, y, z;
};

inline Vector operator*( float fl, const Vector& v ) { return v * fl; }
inline Vector CrossProduct( const Vector& a, const Vector& b ) { return Vector( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x ); }

// This is disgusting, but too deeply entrenched to remove now.
#define vec3_t Vector

#endif // __cplusplus

// These are defines rather than inline functions so that they'll work with either a pure array or a vector class.
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}
#define VectorClear(a) {(a)[0]=0.0;(a)[1]=0.0;(a)[2]=0.0;}
#define DotProduct(x, y) ((x)[0] * (y)[0] + (x)[1] * (y)[1] + (x)[2] * (y)[2])
#define DotProduct2D(x, y) ((x)[0] * (y)[0] + (x)[1] * (y)[1])
