#include "Car.h"

Car::Car(Point2D p, float w, float h, float m, float t) : Square(p,w,h)
{
	position = p;
	width = w;
	height = h;
	mass = m;
	force = Vector2D(0.0f, 0.0f);
	velocity = Vector2D(0.0f, 0.0f);
	acceleration = Vector2D(0.0f, 0.0f);
	thrust = t;
}

Car::~Car()
{
}

void Car::CalcAcceleration()
{
	acceleration = force / mass;
}

void Car::CalcVelocity()
{
	velocity += acceleration * timeStep;
	velocity *= slowDownRatio;
}

void Car::CalcPosition()
{
	position += velocity * timeStep;
}

void Car::Update()
{
	CalcAcceleration();
	CalcVelocity();
	CalcPosition();
}