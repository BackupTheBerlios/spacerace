/********************************************************************************
 *																				*
 *  This file is part of Spacerace.												*
 *	Copyright (C) 2004 by Benjamin Maier, Mark Hoffmann, Alexander Berner		*
 *																				*
 *  Spacerace is free software; you can redistribute it and/or modify			*
 *  it under the terms of the GNU General Public License as published by		*
 *  the Free Software Foundation; either version 2 of the License, or			*
 *  (at your option) any later version.											*
 *																				*
 *  Spacerace is distributed in the hope that it will be useful,				*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of				*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				*
 *  GNU General Public License for more details.								*
 *																				*
 *  You should have received a copy of the GNU General Public License			*
 *  along with Spacerace; if not, write to the Free Software					*
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	*
 *																				*
 ********************************************************************************/





#ifndef _VEKTOR3F_H_
#define _VEKTOR3F_H_

#include <stdio.h>
#include <math.h>

class Vektor3f  
{

public:

	inline Vektor3f(const float& x2=0.0f, const float& y2=0.0f, const float& z2=0.0f)
	{
		x=x2;
		y=y2;
		z=z2;
	}

	virtual ~Vektor3f(){}

	float x,y,z;


	inline	Vektor3f operator+(const Vektor3f& p)const
	{
		return Vektor3f(x + p.x, y + p.y, z + p.z);
	} 

	inline	bool operator==(const Vektor3f& p)const
	{
		return (x==p.x && p.y == y && z == p.z);
	} 


	inline Vektor3f operator*(const Vektor3f& mulK) const 
	{
		return Vektor3f(y * mulK.z - z * mulK.y,
						z * mulK.x - x * mulK.z,
						x * mulK.y - y * mulK.x);
	}


	inline Vektor3f operator*(const float& f) const 
	{
		return Vektor3f(x*f, y*f, z*f);
	}

	inline void mal(const float& f) 
	{
		x*=f; y*=f; z*=f;
	}

	inline Vektor3f operator/(const float& d) const 
	{
		float f=1.0f/d;
		return Vektor3f(x*f, y*f, z*f);
	}


	inline Vektor3f kreuz(const Vektor3f& mulK) const 
	{
		return Vektor3f(y * mulK.z - z * mulK.y,
						z * mulK.x - x * mulK.z,
						x * mulK.y - y * mulK.x);
	}

	inline float dot(const Vektor3f& mul) const 
	{
		return (x * mul.x + y * mul.y + z * mul.z);
	}
	
	inline Vektor3f operator-(const Vektor3f& p) const
	{
		return Vektor3f(x - p.x, y - p.y, z - p.z);
	} 

	inline void minus(const Vektor3f& p) 
	{
		x -= p.x; y -= p.y; z -= p.z;
	} 

	inline void plus(const Vektor3f& p) 
	{
		x += p.x; y += p.y; z += p.z;
	} 

	inline void Vektor3f::normalize()
	{
		//p=length
		float p=sqrtf(x*x+y*y+z*z);
		//	printf("length:%f\n", p);
		if (p!=0) 
		{
			float r=1.0f/p;
			x=x*r;
			y=y*r;
			z=z*r;
		};
	}

	inline float Vektor3f::length() const
	{	
		return sqrtf(x*x+y*y+z*z);
	}

	inline float Vektor3f::sqrLength() const
	{	
		return (x*x+y*y+z*z);
	}


};

#endif 