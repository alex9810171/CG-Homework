#pragma once
//================================================================================================= 
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {

public:
	vec3() {}
	vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	vec3(const vec3& v1) {
		for(int i = 0; i < 3; i ++)
			e[i] = v1.e[i];
	}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }
	float r() const { return e[0]; }
	float g() const { return e[1]; }
	float b() const { return e[2]; }

	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

	inline vec3& operator=(const vec3 &v2);
	inline vec3& operator+=(const vec3 &v2);
	inline vec3& operator-=(const vec3 &v2);
	inline vec3& operator*=(const vec3 &v2);

	inline vec3& operator*=(const float t);
	inline vec3& operator/=(const float t);

	inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline void make_unit_vector();

	float e[3];
};

inline vec3& vec3::operator=(const vec3 &v2) {
	for (int i = 0; i < 3; i++)
		this->e[i] = v2.e[i];
	return *this;
}

inline vec3& vec3::operator+=(const vec3 &v2) {
	for (int i = 0; i < 3; i++)
		this->e[i] = this->e[i] + v2.e[i];
	return *this;
}

inline vec3& vec3::operator-=(const vec3 &v2) {
	for (int i = 0; i < 3; i++)
		this->e[i] = this->e[i] - v2.e[i];
	return *this;
}

inline vec3& vec3::operator*=(const vec3 &v2) {
	for (int i = 0; i < 3; i++)
		this->e[i] = this->e[i] * v2.e[i];
	return *this;
}

inline vec3& vec3::operator*=(const float t) {
	for (int i = 0; i < 3; i++)
		this->e[i] = this->e[i] * t;
	return *this;
}

inline vec3& vec3::operator/=(const float t) {
	for (int i = 0; i < 3; i++)
		this->e[i] = this->e[i] / t;
	return *this;
}

inline std::ostream& operator<<(std::ostream &os, const vec3 &t)
{
	os << '(' << t.e[0] << ", " << t.e[1] << ", " << t.e[2] << ')';
	return os;
}

inline void vec3::make_unit_vector()
{
	float length = sqrt(pow(x(), 2) + pow(y(), 2) + pow(z(), 2));
	for (int i = 0; i < 3; i++)
		e[i] = e[i] / length;
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
	vec3 crossVector;
	crossVector.e[0] = v1.e[2] * v2.e[3] - v1.e[3] * v2.e[2];
	crossVector.e[1] = v1.e[3] * v2.e[1] - v1.e[1] * v2.e[3];
	crossVector.e[2] = v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1];
	return crossVector;
}

inline vec3 unit_vector(vec3 v)
{
	float length = sqrt(pow(v.x(), 2) + pow(v.y(), 2) + pow(v.z(), 2));
	for (int i = 0; i < 3; i++)
		v.e[i] = v.e[i] / length;
	return v;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
	return vec3(v1) += v2;
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
	return vec3(v1) -= v2;
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
	return vec3(v1) *= v2;
}

inline vec3 operator*(float value, const vec3 &v1) {
	return vec3(v1) *= value;
}

inline vec3 operator*(const vec3 &v1, float value) {
	return vec3(v1) *= value;
}

inline vec3 operator/(const vec3 &v1, float value) {
	return vec3(v1) /= value;
}

#endif