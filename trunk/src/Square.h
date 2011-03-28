#ifndef Square_h
#define Square_h

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>	
//#include <gl\glaux.h>

#include "Vector2D.h"


class Square
{
	protected:
		
		Point2D	position;
		float	width;
		float	height;
		
	public:

		Square(Point2D position, float width, float height);
		~Square();

		Point2D GetPosition(void)
		{
			return position;
		}

		void SetPosition(Point2D newPos)
		{
			position = newPos;
		}

		float GetWidth(void)
		{
			return width;
		}

		float GetHeight(void)
		{
			return height;
		}

		void Draw();
};

#endif