﻿#pragma once


#define DECL_FLOAT_OP(op) \
	NiPoint3 operator op(const float n) const \
	{ \
		return NiPoint3(x op n, y op n, z op n); \
	} \
	NiPoint3 operator op##=(const float n) \
	{ \
		return *this = NiPoint3(x op n, y op n, z op n); \
	} \

#define DECL_VEC_OP(op) \
	NiPoint3 operator op(const NiPoint3 v) const \
	{ \
		return NiPoint3(x op v.x, y op v.y, z op v.z); \
	} \
	NiPoint3 operator op##=(const NiPoint3 v) \
	{ \
		return *this = NiPoint3(x op v.x, y op v.y, z op v.z); \
	}

struct NiPoint2
{
	float	x, y;

	NiPoint2() {}
	NiPoint2(float _x, float _y) : x(_x), y(_y) {}
	NiPoint2(const NiPoint2& rhs) { *this = rhs; }
	NiPoint2(__m128 rhs) { *this = rhs; }

	inline void operator=(const NiPoint2& rhs) { _mm_storeu_si64(this, _mm_castps_si128(rhs.PS())); }
	__forceinline void operator=(NiPoint2&& rhs) { _mm_storeu_si64(this, _mm_castps_si128(rhs.PS())); }
	inline void operator=(__m128 rhs) { _mm_storeu_si64(this, _mm_castps_si128(rhs)); }

	inline operator float* () { return &x; }

	inline __m128 PS() const { return _mm_castsi128_ps(_mm_loadu_si64(this)); }

	void Dump() const;
};

struct NiPoint3
{
	float x, y, z;

	void Scale(float scale) {
		x *= scale;
		y *= scale;
		z *= scale;
	};

	void Init(NiPoint3* point)
	{
		x = point->x;
		y = point->y;
		z = point->z;
	};

	NiPoint3() : x(0.f), y(0.f), z(0.f)
	{
	};

	NiPoint3(const float x, const float y, const float z) : x(x), y(y), z(z)
	{
	};


	DECL_FLOAT_OP(*);
	DECL_FLOAT_OP(/ );

	DECL_VEC_OP(+);
	DECL_VEC_OP(-);
	DECL_VEC_OP(*);
	DECL_VEC_OP(/ );

	float length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float length_sqr() const
	{
		return x * x + y * y + z * z;
	}

	NiPoint3 normal() const
	{
		const auto len = length();
		return len == 0.F ? NiPoint3() : NiPoint3(x / len, y / len, z / len);
	}

	static float dot(const NiPoint3& v1, const NiPoint3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static NiPoint3 cross(const NiPoint3& v1, const NiPoint3& v2)
	{
		return NiPoint3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	NiPoint3* Add(NiPoint3* toAdd)
	{
		this->x += toAdd->x;
		this->y += toAdd->y;
		this->z += toAdd->z;
		return this;
	}

	NiPoint3* Subtract(NiPoint3* point)
	{
		this->x -= point->x;
		this->y -= point->y;
		this->z -= point->z;
		return this;
	}

	float CalculateDistSquared(NiPoint3* to)
	{
		float deltaX = (x - to->x);
		float deltaY = (y - to->y);
		float deltaZ = (z - to->z);

		return deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;
	}

	bool Equals(NiPoint3* compare)
	{
		return x == compare->x && y == compare->y && z == compare->z;
	}

	bool FltEquals(const NiPoint3* compare) const
	{
		if (abs(x - compare->x) > FLT_EPSILON) return false;
		if (abs(y - compare->y) > FLT_EPSILON) return false;
		return (abs(z - compare->z) <= FLT_EPSILON);
	}
};

struct NiPoint4
{
	float x, y, z, r;
};

#undef DECL_FLOAT_OP
#undef DECL_VEC_OP