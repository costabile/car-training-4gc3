#ifndef Track_h
#define Track_h

#include <vector>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include "Square.h"
#include "Vector2D.h"
#include "Constants.h"

using namespace std;

class Track
{
	protected:
	
		Point2D			position;
		float			width;
		float			shiftAmount;
		vector<Square*>	walls;
		float			wallSize;
		int				trackLength;
		bool			randomTrack;

	public:

		Track(Point2D position, float width, float shiftAmount, bool random);
		~Track();

		vector<Square*> GetWalls(void)
		{
			return walls;
		}

		void Update();

		void Draw();
};

#endif