#pragma once
#include "Boid.h"
#include <vector>

class Swarm {
public:
	float PerceptionRadius = 50;

	std::vector<MyVector> DirectionTargets;



	float BlindSpot = 20;
	float MaxAcceleration = 0.5f;
	float MaxVelocity = 1;

	Swarm(std::vector<Boid> *inputBoids) : boids(inputBoids) {

	}
	Swarm();

	void UpdateSwarm(float time);
	void UpdateAcceleration();
	std::vector<Boid> *boids;

private:
	
	void UpdateBoid(Boid &b);
	std::vector<boidInRange> getBoidsInRange(const Boid &b);
	float CohesionWeight, AlignmentWeight, SeperationWeight = 1;
	float WanderWeight = 0.1f;
};