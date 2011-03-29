#ifndef Constants_h
#define Constants_h

#include "Vector2D.h"

// Window
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Physics
const float timeStep = 0.001f;
const float slowDownRatio = 0.999f;

// Positioning and Drawing
const Point2D carStart = Point2D(0.0f, -7.5f);
const float carStartOffset = 0.0f;
const float drawDepth = -20.0f;

// Track Parameters / Difficulty
const int updateTime = 80;
const float trackWidth = 7.5f;
const int numTrackRows = 100;
const float shiftAmount = 0.2f;
const float maxShift = 7.5f;

// Neural Network
const int snapshotInterval = 50;
const int numInputs = 14;
const int numHiddenLayers = 1;
const int numHiddenNeurons = 20;
const int numOutputs = 1;

const int startIndex = 8;

#endif