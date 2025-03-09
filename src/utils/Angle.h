#pragma once
#include <cinttypes>
#include <numbers>
#include <raylib.h>

class Angle
{
	std::uint16_t m_valueInt16;

public:
	Angle(std::uint16_t valueInt16);
	Angle(float radians);
	Angle(Vector2& v1, Vector2& v2);	// Angle from v1 to v2 clockwise
	Angle(Vector2& v);	// angle from x axis to v clockwise

	std::uint16_t int16() const;
	float rad() const;

	void setAngle(float radians);
	void setAngle(std::uint16_t valueInt16);
	void print();

	Angle operator+(const Angle& other) const;
	Angle operator-(const Angle& other) const;
	Angle& operator+=(const Angle& other);
	Angle& operator-=(const Angle& other);
	bool operator==(const Angle& other) const;
	bool operator!=(const Angle& other) const;

private:
	std::uint16_t radToInt16(float radians) const;
	float int16ToRad(std::uint16_t value) const;
	std::uint16_t angleTwoVector2(const Vector2& v1, const Vector2& v2) const;

	static constexpr float TWO_PI{ 2.0f * std::numbers::pi_v<float> };	// static - only one place in memory across all instances of the class
	static constexpr float OVERFLOW_VALUE{ static_cast<float>(UINT16_MAX) + 1.0f };
};
