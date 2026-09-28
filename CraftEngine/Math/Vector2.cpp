#include "Vector2.h"
#include "MathDefines.h"
#include <cmath>
#include <cassert>

namespace Craft
{
	const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
	const Vector2 Vector2::One;
	const Vector2 Vector2::Right;
	const Vector2 Vector2::Up;

	Vector2::Vector2(float x, float y) : x(x), y(y)
	{
	}

	float Vector2::Length() const
	{
		// 파타고라스 정리 - sqrt = root씌우기
		return std::sqrt(x * x + y * y);
	}

	float Vector2::LengthSquared() const
	{
		return x * x + y * y;
	}

	Vector2 Vector2::Noramalized() const
	{
		// 길이가 1인 벡터로 만드는 연산 -> 각 성분을 length로 나누기
		float length = Length();

		// 0 나누지 못하도록 방지
		if (length <= KindaSmallNumber)
		{
			assert(false && "length should not be zero");
			return Vector2::Zero;
		}

		return Vector2(x / length, y / length);
	}

	float Vector2::Dot(const Vector2& other) const
	{
		// 두 가지 방식으로 계산 가능
		// 엔진에서는 쉬운 계산 법으로 구현
		return x * other.x + y * other.y;

		// A dot B = |A| x |B| x Cos(theta)
		// -> 사이각(theta) 구하는 게 번거로우니 위 방법으로 진행

		// 내적의 기하학적인 의미가 무엇이냐 - 조명 연산??
	}
	
	bool Vector2::operator==(const Vector2& other) const
	{
		return x == other.x && y == other.y;
	}
	
	bool Vector2::operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}

	Vector2& Vector2::operator=(const Vector2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}
	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}
	Vector2& Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2& Vector2::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vector2 Vector2::operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 Vector2::operator*(float scale) const
	{
		return Vector2(x * scale, y * scale);
	}

	Vector2& Vector2::operator*=(const Vector2& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vector2 Vector2::operator/(const Vector2& other) const
	{
		if (std::abs(other.x) <= KindaSmallNumber
			|| std::abs(other.y) <= KindaSmallNumber)
		{
			assert(false && "other.x and other.y should not be near 0");
			return Vector2::Zero;
		}
		return Vector2(x / other.x, y / other.y);
	}

	Vector2 Vector2::operator/(float scale) const
	{
		if (std::abs(scale) <= KindaSmallNumber)
		{
			assert(false && "scale should not be near 0");
			return Vector2::Zero;
		}
		return Vector2(x / scale, y / scale);
	}

	Vector2& Vector2::operator/=(const Vector2& other)
	{
		if (std::abs(other.x) <= KindaSmallNumber
			|| std::abs(other.y) <= KindaSmallNumber)
		{
			assert(false && "other.x and other.y should not be near 0");
			return *this;
		}
		x /= other.x;
		y /= other.y;
		return *this;
	}

	Vector2& Vector2::operator/=(float scale)
	{
		if (std::abs(scale) <= KindaSmallNumber)
		{
			assert(false && "scale should not be near 0");
			return *this;
		}
		x /= scale;
		y /= scale;
		return *this;
	}
}