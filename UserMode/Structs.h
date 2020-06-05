#pragma once

class vector3
{
public:
	float x;
	float y;
	float z;

	vector3()
	{
		x = y = z = 0;
	}

	vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vector3& operator+(vector3 arg)
	{
		x += arg.x;
		y += arg.y;
		z += arg.z;
		return *this;
	}
};



struct ViewMatrix
{
	float matrix[4][4];
	float* operator[ ](int index)
	{
		return matrix[index];
	}
};



struct Vector1
{
	float x, y, z;
};

