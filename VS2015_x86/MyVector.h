#pragma once
#include <math.h>

// Approximination because my visual studio doesnt find pi in math.h
# define PI_2 6.283

class MyVector {
public:

	// Constructors
	MyVector() {
		x = 0;
		y = 0;
		z = 0;
	}
	MyVector(float inputX, float inputY, float inputZ) {
		x = inputX;
		y = inputY;
		z = inputZ;
	};


	bool operator==(const MyVector &Vector)const {
		return x == Vector.x && y == Vector.y && z == Vector.z;
	}

	//MyVector arithmetic functions
	MyVector operator+(const MyVector &Vector)const {
		return MyVector(x + Vector.x, y + Vector.y, z + Vector.z);
	}
	MyVector operator-(const MyVector &Vector)const {
		return MyVector(x - Vector.x, y - Vector.y, z - Vector.z);
	}
	MyVector operator*(const MyVector &Vector)const {
		return MyVector(x * Vector.x, y * Vector.y, z * Vector.z);
	}
	MyVector operator/(const MyVector &Vector)const {
		return MyVector(x / Vector.x, y / Vector.y, z / Vector.z);
	}

	// Overload for scalar functions
	MyVector  operator+(float scalar)const {
		return MyVector(x + scalar, y + scalar, z + scalar);
	}
	MyVector  operator-(float scalar)const {
		return MyVector(x - scalar, y - scalar, z - scalar);
	}
	MyVector  operator*(float scalar)const {
		return MyVector(x * scalar, y * scalar, z * scalar);
	}
	MyVector  operator/(float scalar)const {
		return MyVector(x / scalar, y / scalar, z / scalar);
	}


	//MyVector cross(MyVector Vector);

	float dotProduct(const MyVector &Vector)const {
		return x*Vector.x+y*Vector.y+z*Vector.z;
	}

	float length() const {
		return (float) sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	MyVector normalize()const {
		float length = (float) sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		if (length == 0) {
			return MyVector(0, 0, 0);
		}

		return MyVector(x / length, y / length, z / length);

	}

	float AngelBetweenVectors(const MyVector &Vector)const {
		float LengthVector1 = (float) sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		float LengthVector2 = Vector.length();

		if (LengthVector1 == 0 || LengthVector1 == 0) {
			return 0;
		}

		return static_cast<float>(acos(dotProduct(Vector)) / (LengthVector1 * LengthVector2) * 360 / PI_2);
	}

	MyVector SetMaxLength(float MaxLength) const {
		float l = length();
		if (l > MaxLength) {
			return normalize() * MaxLength;
		}
		
		return *this;
	}

	float x;
	float y;
	float z;
};