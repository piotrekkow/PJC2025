#include "Angle.h"
#include <iostream>
#include <cmath>

Angle::Angle(std::uint16_t valueInt16) : m_valueInt16{ valueInt16 } {}
Angle::Angle(float radians) : m_valueInt16{ radToInt16(radians) } {}
Angle::Angle(Vector2& v1, Vector2& v2) : m_valueInt16{ angleTwoVector2(v1, v2) } {}
Angle::Angle(Vector2& v)
{
	Vector2 xAxis{ 0.0f, 0.0f };
	m_valueInt16 = angleTwoVector2(xAxis, v);
}

std::uint16_t Angle::int16() const
{
	return m_valueInt16;
}

float Angle::rad() const
{
	return int16ToRad(m_valueInt16);
}

void Angle::setAngle(float radians)
{
	m_valueInt16 = radToInt16(radians);
}
void Angle::setAngle(std::uint16_t valueInt16)
{
	m_valueInt16 = valueInt16;
}
void Angle::print()
{
	std::cout << "int16=" << m_valueInt16 << ", rad=" << rad() << " (" << rad() / std::numbers::pi_v<float> << "pi), overflow at:" << OVERFLOW_VALUE << '\n';
}

Angle Angle::operator+(const Angle& other) const
{
	return Angle(static_cast<std::uint16_t>(m_valueInt16 + other.m_valueInt16));
}

Angle Angle::operator-(const Angle& other) const
{
	return Angle(static_cast<std::uint16_t>(m_valueInt16 - other.m_valueInt16));
}

Angle& Angle::operator+=(const Angle& other)
{
	m_valueInt16 = static_cast<std::uint16_t>(m_valueInt16 + other.m_valueInt16);
	return *this;
}

Angle& Angle::operator-=(const Angle& other)
{
	m_valueInt16 = static_cast<std::uint16_t>(m_valueInt16 - other.m_valueInt16);
	return *this;
}

bool Angle::operator==(const Angle& other) const
{
	return m_valueInt16 == other.m_valueInt16;
}

bool Angle::operator!=(const Angle& other) const
{
	return m_valueInt16 != other.m_valueInt16;
}

std::uint16_t Angle::radToInt16(float radians) const
{
	return static_cast<std::uint16_t>((radians / (2 * std::numbers::pi)) * OVERFLOW_VALUE);
}

float Angle::int16ToRad(std::uint16_t value) const
{
	return (static_cast<float>(value) / OVERFLOW_VALUE) * 2 * std::numbers::pi_v<float>;
}
std::uint16_t Angle::angleTwoVector2(const Vector2& v1, const Vector2& v2) const
{
	float angle{ std::atan2(v2.y,v2.x) - std::atan2(v1.y, v1.x) };
	if (angle < 0)
		angle += 2 * std::numbers::pi_v<float>;
	return radToInt16(angle);
}