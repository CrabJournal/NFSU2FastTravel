#pragma once
#include <intrin.h>
#include <windows.h>

#define IN
#define OUT

// since Intel Ivy Bridge rep movsb was enhanced, but rep movsb/rep stosb even worked quickly before
// however it may have high internal startup
// "The large overhead for choosing and setting up the right method is mainly due to the lack of microcode branch prediction"
#define R_memcpy(DST, SRC, _SIZE)  __movsb((BYTE*)(DST), (BYTE*)(SRC), _SIZE)
#define R_memset(PTR, VAL, _SIZE) __stosb((BYTE*)PTR, (BYTE)VAL, _SIZE)

struct vector2 {
	float x, y;
};

struct vector3 {
	float x, y, z;
	inline operator __m128() const { return _mm_loadu_ps((float*)this); }
};
struct vector3a:vector3 {
	DWORD align;
	inline vector3a(__m128 t) { _mm_storeu_ps((float*)this, t); }
	inline void operator=(const __m128 t) {
		_mm_storeu_ps((float*)this, t);
	}
	vector3a(){ }
};
inline void SaveM128InV3(__m128 src, vector3 &dst) {
	_mm_storel_pi((__m64*)&dst, src);
	src = _mm_shuffle_ps(src, src, 0b00'00'00'10);
	_mm_store_ss((float*)&dst.z, src);
}

struct Quanternion {
	float w, x, y, z;
};

#if _M_IX86_FP >= 1 // SSE

inline float _sqrt(float F) {
	return _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(F)));
}

inline __m128 operator|(__m128 a, __m128 b) {
	return _mm_or_ps(a, b);
}

inline __m128 operator&(__m128 a, __m128 b) {
	return _mm_and_ps(a, b);
}
inline __m128 operator&(__m128 a, DWORD b) {
	return _mm_and_ps(a, _mm_load_ps1((float*)&b));
}
inline __m128 operator&(DWORD a, __m128 b) {
	return b & a;
}

inline __m128 operator^(__m128 a, __m128 b) {
	return _mm_xor_ps(a, b);
}
inline __m128 operator^(__m128 a, DWORD b) {
	return _mm_xor_ps(a, _mm_load_ps1((float*)&b));
}

inline __m128 operator~(__m128 a) {
	return a^_mm_cmpeq_ps(a, a);
}


inline __m128 operator+(__m128 a, __m128 b) {
	return _mm_add_ps(a, b);
}
inline __m128 operator-(__m128 a, __m128 b) {
	return _mm_sub_ps(a, b);
}
inline __m128 operator*(__m128 a, __m128 b) {
	return _mm_mul_ps(a, b);
}
inline __m128 operator/(__m128 a, __m128 b) {
	return _mm_div_ps(a, b);
}

inline __m128 operator*(__m128 a, float b) {
	return a * _mm_load_ps1(&b);
}
inline __m128 operator/(__m128 a, float b) {
	return a / _mm_load_ps1(&b);
}
inline __m128 operator*(float a, __m128 b) {
	return b * a;
}

inline __m128 zero_m128() {
	return _mm_setzero_ps();
}

#if _M_IX86_FP >= 2 // SSE 2

inline int R_dtoi(const double x) {
	return _mm_cvttsd_si32(_mm_set_sd(x));
}

inline __m128 operator>>(__m128 a, int imm8) {
	return _mm_castsi128_ps(_mm_srli_epi32(_mm_castps_si128(a), imm8));
}
inline __m128 operator<<(__m128 a, int imm8) {
	return _mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(a), imm8));
}
inline __m128 abs(__m128 a) {
	return (a << 1) >> 1; // it's should be faster than load ss, shuffle and AND then
}

#else

inline __m128 abs(__m128 a) {
	return a & 0x7FFF'FFFF;
}

#endif // SSE 2

#else // no SSE

#include <math.h>
#define _sqrt sqrt

#endif

union int_float {
	DWORD i;
	float f;
};

// fabs from cmath casting float to double and backward
inline float R_fabs(float f) {
	int_float fi;
	fi.f = f;
	fi.i &= 0x7FFF'FFFF;
	return fi.f;
}


#define ONLY_SIGN32(N) ( *(unsigned int*)&(N) >> 31 )
const float  ones[4] = {1.0, 1.0, 1.0, 1.0};

inline float GetSinCosNormal(float cossin) {
	float _one = *ones;
	if (R_fabs(cossin) >= _one)
		return 0;
	return _sqrt(_one - cossin * cossin);
}
inline __m128 GetSinCosNormalSSE(__m128 cossin) {
	__m128 _ones = _mm_load_ps(ones);
	return _mm_sqrt_ps(_ones - _mm_min_ps((cossin * cossin), _ones));
}

inline float GetSinCos(float cossin) {
	return _sqrt(*ones - (cossin * cossin));
}
inline __m128 GetSinCosSSE(__m128 cossin) {
	return _mm_sqrt_ps(_mm_load_ps(ones) - (cossin * cossin));
}

/*
 *	if val > limit
 *		return limit
 *	else
 *		if val < -limit
 *			return -limit
 *		else
 *			return val
 */
inline float _vectorcall FitInto(float val, const float limit) {
	__m128 _val = _mm_set_ss(val);
	__m128 _sign = (_val >> 31) << 31;
	__m128 _res = _sign | _mm_min_ss(_val ^ _sign, /* abs(_val) // XORPS is faster, but makes a depency chain */
		_mm_set_ss(limit));
	return _mm_cvtss_f32(_res);
}
inline float FitIntoOne(float val) {
	return FitInto(val, *ones);
}
inline __m128 _vectorcall FitIntoOneSSE(__m128 vals) {
	__m128 signs = (vals >> 31) << 31;
	return signs | _mm_min_ps(vals ^ signs, _mm_load_ps(ones));
}

const DWORD maskCustomSignOnly = 0x4000'0000;
const DWORD maskZeroCustomSign = ~maskCustomSignOnly;

inline float CosSigned(float Sin) {
	int_float sin, cos;
	sin.f = Sin;
	//sin.i &= maskZeroCustomSign;
	cos.f = GetSinCosNormal(sin.f);

	//sin.f = Sin;
	//sin.i &= maskCustomSignOnly;
	//cos.i |= sin.i << 1;

	cos.i |= sin.i << 31; // set sign from sin's lowest bit of mantissa
	return cos.f;
}

inline __m128 SetCosSignFromSrcSSE(vector3 src, __m128 ucoss) {
	return ucoss | (src << 31); // set sign from sin's lowest bit of mantissa

	// return ucoss | ((src & maskCustomSignOnly) << 1);
}

inline void CosSignedSSE(IN OUT vector3* Sins, OUT vector3a* Coss) {

	/*
	__m128 realSin = *Sins & maskZeroSign;
	__m128 coss = GetSinCosSSE(realSin);
	*Coss = SetCosSignFromSrcSSE(*Sins, coss);
	*Sins = realSin;
	*/

	__m128 coss = GetSinCosSSE(*Sins);
	*Coss = SetCosSignFromSrcSSE(*Sins, coss);

	/* may be deleted i think
	DWORD b = *(DWORD*)((float*)Sins + 3);
	*(vector3a*)Sins = (*Sins >> 1) << 1; // zeroing lowest bit of mantissa
	*(DWORD*)((float*)Sins + 3) = b;
	*/
}

inline float SetCosSignTo(float sin, float cossingned) {
	int_float s, c;
	s.f = sin;
	c.f = cossingned;

	s.i &= 0xFFFF'FFFE;
	s.i |= c.i >> 31;
	// s.i |= (c.i >> 31) << 30;	// set sign in highest bit of exponent from cos
	return s.f;
}

inline float abs(vector3* v) {
	return _sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
}