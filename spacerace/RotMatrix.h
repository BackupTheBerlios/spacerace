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




#ifndef _ROTMATRIX_H
#define _ROTMATRIX_H 1

#include "Vektor3f.h"

enum Achse
{
	XA,YA,ZA
};

class RotMatrix 
{

public:

	RotMatrix();

	RotMatrix(Vektor3f,Vektor3f,Vektor3f);

	virtual ~RotMatrix();
	float m[3][3];

	void rotX(const float &);
   	void rotY(const float &);
	void rotZ(const float &);
	void rotVektor3f(const Vektor3f&,const float &);


	inline RotMatrix(const Vektor3f& vec,const float &angle)
	{
		float cosA = cosf(angle), sinA = sinf(angle), invCosA = 1.0f - cosA;

		m[0][0]=invCosA*vec.x*vec.x + cosA;			m[0][1]=invCosA*vec.x*vec.y + sinA*vec.z;	m[0][2]=invCosA*vec.x*vec.z - sinA*vec.y;
		m[1][0]=invCosA*vec.x*vec.y - sinA*vec.z;	m[1][1]=invCosA*vec.y*vec.y + cosA;			m[1][2]=invCosA*vec.y*vec.z + sinA*vec.x;
		m[2][0]=invCosA*vec.x*vec.z + sinA*vec.y;	m[2][1]=invCosA*vec.y*vec.z - sinA*vec.x;	m[2][2]=invCosA*vec.z*vec.z + cosA;
	}


	inline Vektor3f operator*(const Vektor3f &mul) const
	{

		Vektor3f erg(
		(m[0][0]*mul.x + m[0][1]*mul.y + m[0][2] *mul.z),
		(m[1][0]*mul.x + m[1][1]*mul.y + m[1][2] *mul.z),
		(m[2][0]*mul.x + m[2][1]*mul.y + m[2][2] *mul.z));
		return erg;
	}




inline void show(void) const
{

	printf("Matrix:\n");
	for (int i=0;i<3;i++)
		printf("%.3f %.3f %.3f\n",m[i][0],m[i][1],m[i][2]);
}

inline RotMatrix operator*(const RotMatrix& mul)const
{
	RotMatrix erg;
	for (int i=0;i<4;i++)
		for(int j=0;j<4;j++)
	{
		{
		erg.m[i][j]=m[i][0]*mul.m[0][j] + m[i][1]*mul.m[1][j] + 
					m[i][2]*mul.m[2][j] + m[i][3]*mul.m[3][j];
		};
	};
	return erg;
}


inline float det(void)const
{
	return	m[0][0]*m[1][1]*m[2][2]  + m[0][1]*m[1][2]*m[2][0] + m[0][2]*m[1][0]*m[2][1] 
			- m[0][2]*m[1][1]*m[2][0] - m[0][0]*m[1][2]*m[2][1] - m[0][1]*m[1][0]*m[2][2];
}



};

#endif