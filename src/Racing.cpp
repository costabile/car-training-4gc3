#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>

#include "Flood3\Flood\MultilayerPerceptron\MultilayerPerceptron.h"
#include "Flood3\Flood\Utilities\Vector.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#elif __WIN32__
	#include <windows.h>
	#include <gl\gl.h>
	#include <gl\glu.h>	
	#include <gl\glaux.h>
#else
	#include <GL/glut.h>
#endif

#include "Car.h"
#include "Track.h"

using namespace std;
using namespace Flood;

/*******************************************************************************
 Define Global Variables
*******************************************************************************/
Car* car = new Car(carStart + Vector2D(carStartOffset,0.0f), 1.0f, 1.0f, 1.0f, 10.0f);
Track* track;
MultilayerPerceptron* network;

bool youLost = false;
bool gameStarted = false;
bool training = true;
bool simulating = false;
bool randomTrack = false;
int score = 0;
int updateCount = 0;

fstream file;
int snapshotCount = 0;

/*******************************************************************************
 Start Function Definitions
*******************************************************************************/

void DrawScore(void)
{
	// Draw score on screen
	char text[30] = "";
	sprintf_s(text, "Score: %i", score);
	glRasterPos3f(-44.0f, -24.0f, drawDepth);
	for(int i = 0; i <= 29; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

bool PointInsideRectangle(Point2D point, Square* wall)
{
	Point2D centre = wall->GetPosition();
	float width = wall->GetWidth();
	float height = wall->GetHeight();

	if (point.x >= centre.x - width/2 &&
		point.x <= centre.x + width/2 &&
		point.y >= centre.y - height/2 &&
		point.y <= centre.y + height/2)
		return true;
	else
		return false;
}

bool CheckCollision(Square* car, Square* wall)
{
	Point2D position = car->GetPosition();
	float width = car->GetWidth();
	float height = car->GetHeight();

	if (PointInsideRectangle(Point2D(position.x - width/2, position.y - height/2), wall) ||
		PointInsideRectangle(Point2D(position.x - width/2, position.y + height/2), wall) ||
		PointInsideRectangle(Point2D(position.x + width/2, position.y + height/2), wall) ||
		PointInsideRectangle(Point2D(position.x + width/2, position.y - height/2), wall))
		return true;
	else
		return false;
}

void OpenFile(char* fileName)
{
	if (fileName == "data.dat")
		file.open(fileName,	ios::out | ios::app);
	else if (fileName == "net.dat")
		file.open(fileName,	ios::in);
}

void CloseFile(void)
{
	file.close();
}

void SaveDataSnapshot(void)
{
	vector<Square*>	walls = track->GetWalls();

	file << car->GetPosition().x << " " << car->GetVelocity().x;
	for (unsigned i = startIndex; i < startIndex + numInputs - 2; i++)
	{
		file << " " << walls.at(i*2)->GetPosition().x;
	}
	file << " " << car->GetForce().x << endl;
}

void LoadNeuralNet(char* neuralNetFile)
{
	string lineString = "";
	float value = 0;
	vector<float> weightAndBias;
	Vector<double> floodWeightAndBias;

	OpenFile(neuralNetFile);

	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, lineString);
			if (lineString != "")
			{
				const char* line = lineString.c_str();
				sscanf_s(line, "%f", &value);
				weightAndBias.push_back(value);
			}
		}
		CloseFile();

		floodWeightAndBias.set_size(weightAndBias.size());
		for (unsigned i = 0; i < weightAndBias.size(); i++)
			floodWeightAndBias[i] = weightAndBias.at(i);
	}
	else
		cout << "Unable to open file." << endl;

	// Set up neural network object and load weight and bias values into it
	Vector<int> hiddenNeurons(numHiddenLayers,numHiddenNeurons);
	network = new MultilayerPerceptron(numInputs, hiddenNeurons, numOutputs);

	const Vector<MultilayerPerceptron::LayerActivationFunction> activationFunctions(numHiddenLayers,MultilayerPerceptron::Logistic);

	network->set_hidden_layers_activation_function(activationFunctions);
	network->set_output_layer_activation_function(MultilayerPerceptron::Linear);
	network->set_neural_parameters(floodWeightAndBias);
}

/*******************************************************************************
 Function		:	windowSetup
 Input			:	void
 Output			:	void
 Description	:	This function is the initial window setup function which 
					enables OpenGL server-side features and sets the window 
					background color.
 *******************************************************************************/
void windowSetup (void)
{
	glShadeModel (GL_SMOOTH);				// Enable Smooth Shading
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);	// Set Background Color
	glClearDepth (1.0f);					// Setup Depth Buffer	
	glEnable (GL_DEPTH_TEST);				// Enable Depth Testing
	glDepthFunc (GL_LEQUAL);	
	glEnable (GL_COLOR_MATERIAL);			// Enable Material Colors
	glEnable(GL_LIGHTING);					// Enable Lighting
	glEnable(GL_LIGHT0);					// Set a Light
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, // Specify Implementation Specific Hints
		GL_FASTEST);
	
	return;
}

/*******************************************************************************
 Function		:	display
 Input			:	void
 Output			:	void
 Description	:	This function is the display callback function.			
*******************************************************************************/
void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer

	// Draw car, track, and score
	glLoadIdentity();
	glTranslatef(0.0f, carStart.y, drawDepth);
	glColor3f(1.0f, 0.0f, 0.0f);
	car->Draw();
	glLoadIdentity();
	glTranslatef(0.0f, carStart.y, drawDepth);
	glColor3f(0.0f, 1.0f, 0.0f);
	track->Draw();
	//DrawScore();
	
	if (gameStarted && youLost == false)
	{
		// Neural Network Data Gathering
		if (training)
		{
			if (snapshotCount >= snapshotInterval)
			{
				SaveDataSnapshot();
				snapshotCount = 0;
			}
			else
				snapshotCount++;
		}
		else if (simulating)
		{
			vector<Square*>	walls = track->GetWalls();
			Vector<double> inputs;
			int wallIndex = startIndex;

			inputs.set_size(numInputs);

			inputs[0] = car->GetPosition().x;
			inputs[1] = car->GetVelocity().x;
			for (int i = 2; wallIndex < startIndex + numInputs - 2; i++)
			{
				inputs[i] = walls.at(wallIndex)->GetPosition().x;
				wallIndex++;
			}

			//Vector<double> practiceInput;
			//practiceInput.setSize(8);
			//practiceInput[0] = 2.69516000000000;
			//practiceInput[1] = -0.117673000000000;
			//practiceInput[2] = -2.55000000000000;
			//practiceInput[3] = -2.35000000000000;
			//practiceInput[4] = -3.35000000000000;
			//practiceInput[5] = -4.35000000000000;
			//practiceInput[6] = -3.95000000000000;
			//practiceInput[7] = -3.35000000000000;
			//Vector<double> output = network->calculateOutput(practiceInput);

			Vector<double> output = network->calculate_output(inputs);
			car->SetForce(Vector2D(output[0], 0.0f));
		}

		// Collision Detection - Only have to check first 2 rows for collision (Squares 0-3)
		//vector<Square*>	walls = track->GetWalls();
		//for (unsigned i = 0; i <= 3; i++)
		//{
		//	if (CheckCollision(car, walls.at(i)))
		//	{
		//		youLost = true;
		//		break;
		//	}
		//}

		// Update car and track
		car->Update();
		if (updateCount >= updateTime)
		{
			track->Update();
			updateCount = 0;
		}

		// Update score and update time
		score++;
		updateCount++;
	}
	else if (youLost)
	{
		if (file.is_open())
			file.close();
	}

	glutSwapBuffers ();	// Swap The Buffers To Not Be Left With A Clear Screen

	return;
}

/*******************************************************************************
 Function		:	reshape
 Input			:	width :: int => new window width in pixels 
					height :: int => new window height in pixels
 Output			:	void
 Description	:	This function is the reshape callback function.
*******************************************************************************/
void reshape (int width, int height)
{
    glViewport (0, 0, width, height);	// Set The Viewport
	glMatrixMode (GL_PROJECTION);		// Select The Projection Matrix	
	glLoadIdentity ();					// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	if (height == 0)
	{
		gluPerspective (80, (float) width, 1.0, 5000.0);
	}	
	else
	{
		gluPerspective (80, (float) width / (float) height, 1.0, 5000.0);
	}
	
	glMatrixMode (GL_MODELVIEW);	// Select The Model View Matrix
	glLoadIdentity ();				// Reset The Model View Matrix

	return;
}

/*******************************************************************************
 Function		:	idle
 Input			:	void
 Output			:	void
 Description	:	This function is the global idle callbcak function. It
*******************************************************************************/
void idle(void)
{
    glutPostRedisplay ();	// Mark the normal plane of the current window as needing to be redisplayed. 
	return;
}

/*******************************************************************************
 Function		:	keyboardSpecial
 Input			:	key :: int => a GLUT_KEY_* constant for the special key
					x :: int => mouse x-coordinate in window relative 
								coordinates when the key was pressed
					y :: int => mouse y-coordinate in window relative
								coordinates when the key was pressed
 Output			:	void
 Description	:	This function is the keyboard callback function for special 
					keys. Special keys are keys such as Arrow Keys, Page Up, 
					Page Down,  F1, F2,..., 
 *******************************************************************************/
void keyboardSpecial (int key, int x, int y)
{	
	switch (key) 
	{
		case GLUT_KEY_RIGHT:
			car->SetForce(Vector2D(car->GetThrust(),0.0f));
			break;
		case GLUT_KEY_LEFT:
			car->SetForce(Vector2D(-1 * car->GetThrust(),0.0f));
			break;
		case GLUT_KEY_UP:
			gameStarted = true;
			break;
		case GLUT_KEY_DOWN:
			//simulating = !simulating;
			break;
		default:
			break;
	}
	return;
}

void keyboardSpecialUp (int x, int y, int z)
// When a special key is depressed, it should stop accelerating, and start slowing down
{
	car->SetForce(Vector2D(0.0f, 0.0f));
	return;
}

/******************************************************************************
 Function		:	main
 Input			:	argc :: int => 
 					argv :: char** =>	
 Output			:	int =>	This functon return EXIT_SUCCESS (0) upon successful 
							execution.
 Description	:	This function is the main routine.
*******************************************************************************/
int main (int argc, char** argv)
{
	char choice = 0;

	cout << "Please choose (Type 1,2 or 3)" << endl;
	cout << "1. Train neural network on static course" << endl;
	cout << "2. Run neural network racing simulation on static course" << endl;
	cout << "3. Train neural network on random course" << endl;
	cout << "4. Run neural network racing simulation on random course" << endl;
	cout << "5. Play Racing on random course" << endl;
	cout << "6. Exit" << endl;
	choice = fgetc(stdin);

	if (choice == '1')
	{
		cout << endl << "Training neural network on static course..." << endl;
		training = true;
		simulating = false;
		track = new Track(carStart, trackWidth, shiftAmount, false);
		OpenFile("data.dat");
	}
	else if (choice == '2')
	{
		cout << endl << "Running neural network racing simulation on static course..." << endl;
		training = false;
		simulating = true;
		track = new Track(carStart, trackWidth, shiftAmount, false);
		LoadNeuralNet("net.dat");
	}
	else if (choice == '3')
	{
		cout << endl << "Training neural network on random course..." << endl;
		training = true;
		simulating = false;
		track = new Track(carStart, trackWidth, shiftAmount, true);
		OpenFile("data.dat");
	}
	else if (choice == '4')
	{
		cout << endl << "Running neural network racing simulation on random course..." << endl;
		training = false;
		simulating = true;
		track = new Track(carStart, trackWidth, shiftAmount, true);
		LoadNeuralNet("net.dat");
	}
	else if (choice == '5')
	{
		cout << endl << "Playing Racing on random course..." << endl;
		training = false;
		simulating = false;
		track = new Track(carStart, trackWidth, shiftAmount, true);
		LoadNeuralNet("net.dat");
	}
	else if (choice == '6')
	{
		cout << "Exiting" << endl;
		return EXIT_SUCCESS;
	}

    glutInit (&argc, argv);											// Initialize the GLUT library
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);		// Set Display Mode
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);				// Set Window Size
    glutCreateWindow ("Racing - Neural Network");					// Create Top-Level Window
	windowSetup();													// Perform Any Additional Window Setup

    glutDisplayFunc (display);						// Sets Display Callback for the Current Window
    glutReshapeFunc (reshape);						// Set Reshape Callback for the Current Window
    glutIdleFunc (idle);							// Set Global Idle Callback
	glutSpecialFunc (keyboardSpecial);				// Set Keyboard Callback for the Current Window (Used for Special Keys)
	glutSpecialUpFunc(keyboardSpecialUp);			// Set Keyboard Callback for when a special key is depressed

    glutMainLoop();								// Enter the GLUT Event Processing Loop

    return EXIT_SUCCESS;
}