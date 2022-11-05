#pragma once

#include "../common.h"

#define FLOAT32_NAN_BITS (uint32)0x7FC00000	// not a number!

#define VEC_T_NAN (vec_t)FLOAT32_NAN_BITS

typedef float vec_t;

class VectorByValue;

//=========================================================
// 3D Vector
//=========================================================
class Vector
{
public:
	// Members
	vec_t x, y, z;

	// Construction/destruction:
	Vector(void);
	Vector(vec_t X, vec_t Y, vec_t Z);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f);

	// TODO: Math operations
	//		can either copy them from source sdk or implement manually

};

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline Vector::Vector(void)
{
	x = y = z = VEC_T_NAN;
}

inline Vector::Vector(vec_t X, vec_t Y, vec_t Z)
{
	x = X; y = Y; z = Z;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector::Init(vec_t ix, vec_t iy, vec_t iz)
{
	x = ix; y = iy; z = iz;
}