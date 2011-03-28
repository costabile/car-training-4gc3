#ifndef Vector2D_h
#define Vector2D_h

#include "math.h"

class Vector2D
{
	public:
		
		float	x;
		float	y;
		
		Vector2D() {}
		
		Vector2D(float r, float s)
		{
			x = r;
			y = s;
		}
		
		Vector2D& Set(float r, float s)
		{
			x = r;
			y = s;
			return (*this);
		}
		
		float& operator [](long k)
		{
			return ((&x)[k]);
		}
		
		const float& operator [](long k) const
		{
			return ((&x)[k]);
		}
		
		Vector2D& operator +=(const Vector2D& v)
		{
			x += v.x;
			y += v.y;
			return (*this);
		}
		
		Vector2D& operator -=(const Vector2D& v)
		{
			x -= v.x;
			y -= v.y;
			return (*this);
		}
		
		Vector2D& operator *=(float t)
		{
			x *= t;
			y *= t;
			return (*this);
		}
		
		Vector2D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			return (*this);
		}
		
		Vector2D& operator &=(const Vector2D& v)
		{
			x *= v.x;
			y *= v.y;
			return (*this);
		}
		
		Vector2D& Normalize(void)
		{
			return (*this *= sqrt(x * x + y * y));
		}
};


inline Vector2D operator -(const Vector2D& v)
{
	return (Vector2D(-v.x, -v.y));
}

inline Vector2D operator +(const Vector2D& v1, const Vector2D& v2)
{
	return (Vector2D(v1.x + v2.x, v1.y + v2.y));
}

inline Vector2D operator -(const Vector2D& v1, const Vector2D& v2)
{
	return (Vector2D(v1.x - v2.x, v1.y - v2.y));
}

inline Vector2D operator *(const Vector2D& v, float t)
{
	return (Vector2D(v.x * t, v.y * t));
}

inline Vector2D operator *(float t, const Vector2D& v)
{
	return (Vector2D(t * v.x, t * v.y));
}

inline Vector2D operator /(const Vector2D& v, float t)
{
	float f = 1.0F / t;
	return (Vector2D(v.x * f, v.y * f));
}

inline float operator *(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}

inline Vector2D operator &(const Vector2D& v1, const Vector2D& v2)
{
	return (Vector2D(v1.x * v2.x, v1.y * v2.y));
}

inline bool operator ==(const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}

inline bool operator !=(const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.x != v2.x) || (v1.y != v2.y));
}
	
class Point2D : public Vector2D
{
	public:
		
		Point2D() {}
		
		Point2D(float r, float s) : Vector2D(r, s) {}
		
		Vector2D& GetVector2D(void)
		{
			return (*this);
		}
		
		const Vector2D& GetVector2D(void) const
		{
			return (*this);
		}
		
		Point2D& operator =(const Vector2D& v)
		{
			x = v.x;
			y = v.y;
			return (*this);
		}
		
		Point2D& operator *=(float t)
		{
			x *= t;
			y *= t;
			return (*this);
		}
		
		Point2D& operator /=(float t)
		{
			float f = 1.0F / t;
			x *= f;
			y *= f;
			return (*this);
		}
};


inline Point2D operator -(const Point2D& p)
{
	return (Point2D(-p.x, -p.y));
}

inline Point2D operator +(const Point2D& p1, const Point2D& p2)
{
	return (Point2D(p1.x + p2.x, p1.y + p2.y));
}

inline Point2D operator +(const Point2D& p, const Vector2D& v)
{
	return (Point2D(p.x + v.x, p.y + v.y));
}

inline Point2D operator -(const Point2D& p, const Vector2D& v)
{
	return (Point2D(p.x - v.x, p.y - v.y));
}

inline Vector2D operator -(const Point2D& p1, const Point2D& p2)
{
	return (Vector2D(p1.x - p2.x, p1.y - p2.y));
}

inline Point2D operator *(const Point2D& p, float t)
{
	return (Point2D(p.x * t, p.y * t));
}

inline Point2D operator *(float t, const Point2D& p)
{
	return (Point2D(t * p.x, t * p.y));
}

inline Point2D operator /(const Point2D& p, float t)
{
	float f = 1.0F / t;
	return (Point2D(p.x * f, p.y * f));
}


inline float Dot(const Vector2D& v1, const Vector2D& v2)
{
	return (v1 * v2);
}

inline Vector2D ProjectOnto(const Vector2D& v1, const Vector2D& v2)
{
	return (v2 * (v1 * v2));
}

inline float Magnitude(const Vector2D& v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

inline float InverseMag(const Vector2D& v)
{
	return (sqrt(v.x * v.x + v.y * v.y));
}

inline float SquaredMag(const Vector2D& v)
{
	return (v.x * v.x + v.y * v.y);
}
	
#endif
