#include "Track.h"

Track::Track(Point2D pos, float w, float shift, bool r)
{
	position = pos;
	wallSize = 1.0f;
	width = w;
	shiftAmount = shift;
	trackLength = numTrackRows;
	randomTrack = r;

	// Set up random generator
	srand((unsigned)time(0)); 
    int random; 
    int lowest = -5, highest = 5; 
    int range = (highest - lowest) + 1;

	Square* leftWallPiece;
	Square* rightWallPiece;

	if (randomTrack == true)
	{
		// Initialize random track
		for(int i = 0; i < trackLength; i++)
		{
			random = lowest + int(range * rand() / (RAND_MAX + 1.0)); 

			if (i == 0)
			{
				leftWallPiece = new Square(position + Point2D(-width/2 + random * shiftAmount, 0.0f), wallSize, wallSize);
				rightWallPiece = new Square(position + Point2D(width/2 + random * shiftAmount, 0.0f), wallSize, wallSize);
			}
			else
			{
				leftWallPiece = new Square(walls.at(2*i-2)->GetPosition() + Point2D(random * shiftAmount, wallSize), wallSize, wallSize);
				rightWallPiece = new Square(walls.at(2*i-1)->GetPosition() + Point2D(random * shiftAmount, wallSize), wallSize, wallSize);
			}
			walls.push_back(leftWallPiece);
			walls.push_back(rightWallPiece);
		}
	}
	else
	{
		// Initialize static training track
		const float dir[30] = {1, 1, 1,-1,-1,
							   1, 1, 1,-1,-1,
							   1,-1,-1,-1,-1,
							  -1, 1,-1, 1,-1,
							   1, 1, 1, 1,-1,
							  -1, 1, 1,-1,-1};
		const int ran[30] = {2,4,1,5,5,
							 2,3,5,2,2,
							 2,4,1,5,2,
							 2,5,1,5,4,
							 5,5,5,5,3,
							 2,4,5,5,4};
		float presetShifts[30];

		for (int i = 0; i < 30; i++)
			presetShifts[i] = dir[i]*ran[i]*shiftAmount;

		for (int i = 0; i < trackLength; i++)
		{
			if (i == 0)
			{
				leftWallPiece = new Square(position + Point2D(-width/2 + presetShifts[i], 0.0f), wallSize, wallSize);
				rightWallPiece = new Square(position + Point2D(width/2 + presetShifts[i], 0.0f), wallSize, wallSize);
			}
			else
			{	
				leftWallPiece = new Square(walls.at(2*i-2)->GetPosition() + Point2D(presetShifts[i], wallSize), wallSize, wallSize);
				rightWallPiece = new Square(walls.at(2*i-1)->GetPosition() + Point2D(presetShifts[i], wallSize), wallSize, wallSize);
			}
			walls.push_back(leftWallPiece);
			walls.push_back(rightWallPiece);
		}
	}
}

Track::~Track()
{
}

void Track::Update()
{
	Point2D offsetPoint = Point2D(0.0f, -wallSize);
	Square* tempLeftWall = walls.at(0);
	Square* tempRightWall = walls.at(1);
	
	// Update all track squares except top row
	for (unsigned i = 0; i < walls.size()/2 - 1; i++)
	{
		walls.at(2*i)->SetPosition(walls.at(2*i+2)->GetPosition() + offsetPoint);
		walls.at(2*i+1)->SetPosition(walls.at(2*i+3)->GetPosition() + offsetPoint);
	}

	if (randomTrack == true)
	{
		// Generate new top row
		int lowest = -5, highest = 5; 
		int range = (highest - lowest) + 1;
		int random = lowest + int(range * rand() / (RAND_MAX + 1.0));
		walls.at(trackLength*2-2)->SetPosition(walls.at(trackLength*2-4)->GetPosition() + Point2D(random * shiftAmount, wallSize));
		walls.at(trackLength*2-1)->SetPosition(walls.at(trackLength*2-3)->GetPosition() + Point2D(random * shiftAmount, wallSize));

		// Check top-left box to see if track is going towards edge of screen
		if (fabs(walls.at(trackLength*2-1)->GetPosition().x) > width/2 + maxShift || fabs(walls.at(trackLength*2-2)->GetPosition().x) > width/2 + maxShift)
		{
			walls.at(trackLength*2-2)->SetPosition(walls.at(trackLength*2-2)->GetPosition() + Point2D(-random * 2 * shiftAmount, 0.0f));
			walls.at(trackLength*2-1)->SetPosition(walls.at(trackLength*2-1)->GetPosition() + Point2D(-random * 2 * shiftAmount, 0.0f));
		}
	}
	else
	{
		// Top row gets x positions of bottom row
		walls.at(trackLength*2-2)->SetPosition(tempLeftWall->GetPosition() + Point2D(0.0f, wallSize * (trackLength - 1)));
		walls.at(trackLength*2-1)->SetPosition(tempRightWall->GetPosition() + Point2D(0.0f, wallSize * (trackLength - 1)));
	}
}

void Track::Draw()
{
	for (unsigned i = 0; i < walls.size(); i++)
		walls.at(i)->Draw();
}