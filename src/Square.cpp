#include "Square.h"

Square::Square(Point2D p, float w, float h)
{
	position = p;
	width = w;
	height = h;
}

Square::~Square()
{
}

void Square::Draw()
{
	glBegin(GL_QUADS);
		glVertex2f(position.x - width/2, position.y - height/2);		// Bottom Left
		glVertex2f(position.x - width/2, position.y + height/2);		// Top Left
		glVertex2f(position.x + width/2, position.y + height/2);		// Top Right
		glVertex2f(position.x + width/2, position.y - height/2);		// Bottom Right
	glEnd();
}