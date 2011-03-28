#ifndef Car_h
#define Car_h

#include <cmath>
#include "Square.h"
#include "Constants.h"
#include "Vector2D.h"

class Car : public Square
{
	protected:
		
		float		mass;
		Vector2D	force;
		Vector2D	velocity;
		Vector2D	acceleration;
		float		thrust;

		void CalcAcceleration();
		void CalcVelocity();
		void CalcPosition();
		
	public:

		Car(Point2D position, float width, float height, float mass, float carThrust);
		~Car();

		float GetMass(void)
		{
			return mass;
		}

		Vector2D GetForce(void)
		{
			return force;
		}

		void SetForce(Vector2D f)
		{
			force = f;
		}

		Vector2D GetVelocity(void)
		{
			return velocity;
		}

		Vector2D GetAcceleration(void)
		{
			return acceleration;
		}

		float GetThrust(void)
		{
			return thrust;
		}

		void Update();
};

#endif