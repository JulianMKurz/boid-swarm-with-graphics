#pragma once
#include "MyVector.h"

class Boid {
public: 
	MyVector Position;
	MyVector Velocity;
	MyVector Acceleration;

	Boid(MyVector p, MyVector v) {
		Position = p;
		Velocity = v;
	}
	Boid() {}
};

class boidInRange {
public:
	Boid *boid;
	MyVector Direction;
	float Distance;
	boidInRange(Boid *inBoid, MyVector inDirection, float inDistance) : 
		boid(inBoid),
		Direction(inDirection),
		Distance(inDistance) {}
	boidInRange() {}
};