#pragma once
#include "Precompiled.hpp"

struct ViewMatrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}
	const float* operator[ ](int index) const noexcept {
		return matrix[index];
	}

	float matrix[4][4];
};


class Vector_t {
public:
	float x, y, z;
	Vector_t(float x = 0.0f, float y = 0.0f, float z = 0.0f) :x(x),y(y),z(z) { }

	Vector_t operator + (const Vector_t& other) {
		return{ x + other.x,y + other.y,z + other.z };
	}

	Vector_t operator - (const Vector_t& other) {
		return{ x - other.x,y - other.y,z - other.z };
	}

	const Vector_t W2S(ViewMatrix_t vm) const {
		float _x = vm[0][0] * x + vm[0][1] * y + vm[0][2] * z + vm[0][3];
		float _y = vm[1][0] * x + vm[1][1] * y + vm[1][2] * z + vm[1][3];

		float w = vm[3][0] * x + vm[3][1] * y + vm[3][2] * z + vm[3][3];
		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = GetSystemMetrics(SM_CXFULLSCREEN) * .5f;
		float y = GetSystemMetrics(SM_CYFULLSCREEN) * .5f;

		x += 0.5f * _x * GetSystemMetrics(SM_CXFULLSCREEN) + 0.5f;
		y -= 0.5f * _y * GetSystemMetrics(SM_CYFULLSCREEN) + 0.5f;
		return Vector_t(x, y, w);
	}

};