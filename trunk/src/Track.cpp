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
		//static track created in *absolute* coordinates
		const int ran[numTrackRows] = {
			 1, 2, 3, 5, 7, 9,13,11, 9, 8,
			 5, 3, 2, 1, 3, 5, 7, 8,11,13,
			11, 8, 7, 5, 3, 1, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
			 8,13,18,23,18,13, 8, 3,-2,-7,
			-12,-17,-13,-9,-4, 1, 1, 1, 1, 1,
			 1, 1, 1, 1, 1,-2,-4,-6,-9,-5,
			-3, 2, 7,12,17,22,26,22,18,13,
			 9, 4, 6, 9,13,14,14,11,12, 8,
			 7, 6, 5, 4, 3, 2, 1, 1, 1, 1
		};
		float presetShifts[numTrackRows];

		for (int i = 0; i < numTrackRows; i++)
			presetShifts[i] = ran[i]*shiftAmount;

		for (int i = 0; i < numTrackRows; i++)
		{
			if (i == 0)
			{
				leftWallPiece = new Square(Point2D(presetShifts[i], -trackWidth), wallSize, wallSize);
				rightWallPiece = new Square(Point2D(presetShifts[i], -trackWidth), wallSize, wallSize);
			}
			else
			{	
				leftWallPiece = new Square(Point2D(presetShifts[i], wallSize + walls.at(2*i-2)->GetPosition().y), wallSize, wallSize);
				rightWallPiece = new Square(Point2D(presetShifts[i] + trackWidth, wallSize + walls.at(2*i-1)->GetPosition().y), wallSize, wallSize);
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