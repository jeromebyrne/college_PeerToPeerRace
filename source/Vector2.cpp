#include "precompiled.h"
#include "Vector2.h"

Vector2::Vector2() : X(0), Y(0)
{
}
Vector2::Vector2(float x = 0, float y = 0): X(x), Y(y)
{
}

Vector2::~Vector2(void)
{
}
float Vector2::Distance(Vector2 dest)
{
	float X2_X1_plus_Y2_Y1;

	X2_X1_plus_Y2_Y1 = (dest.X - X) * (dest.X - X) + (dest.Y - Y) * (dest.Y - Y);

	float distance = sqrt(X2_X1_plus_Y2_Y1);

	return distance;
}

float Vector2::Distance(Vector2 src, Vector2 dest)
{
	float X2_X1_plus_Y2_Y1;

	X2_X1_plus_Y2_Y1 = (dest.X - src.X) * (dest.X - src.X) + (dest.Y - src.Y) * (dest.Y - src.Y);

	float distance = sqrt(X2_X1_plus_Y2_Y1);

	return distance;
}
float Vector2::Length()
{
	return sqrt((X * X) + (Y * Y));
}
void Vector2::Normalise()
{
	float length = Length();

	// prevent divide by 0
	if(length > 0)
	{
		X /= length;
		Y /= length;
	}
	
}
