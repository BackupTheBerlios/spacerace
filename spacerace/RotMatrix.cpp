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




#include "RotMatrix.h"


RotMatrix::RotMatrix()
{
	
}



RotMatrix::RotMatrix(Vektor3f a, Vektor3f b, Vektor3f c)
{
	
	m[0][0]=a.x;	m[0][1]=b.x;	m[0][2]=c.x;
	m[1][0]=a.y;	m[1][1]=b.y;	m[1][2]=c.y;
	m[2][0]=a.z;	m[2][1]=b.z;	m[2][2]=c.z;
	
}



RotMatrix::~RotMatrix()
{
	
}

void RotMatrix::rotY(const float &a)
{
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			if (i==j)
				m[i][j]=1;
			else
				m[i][j]=0;            //Einheitsmatrix
		}
	}
	//
	m[0][0]= cosf(a)    ;	m[0][2]= sinf(a)       ;		//
	// [	0			   1		0				  0		]
	m[2][0]=-sinf(a)    ;	m[2][2]= cosf(a)       ;		//
	// [	0			   0		0				  1		]
	
}

void RotMatrix::rotX(const float & a)
{
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			if (i==j)
				m[i][j]=1;
			else
				m[i][j]=0;            //Einheitsmatrix
		}
	}
	//
	// [    1			   0			    0			      0 	]
	/* [	0*/		m[1][1]= cosf(a); 	m[1][2]= sinf(a);	//0		]
	/* [	0*/		m[2][1]=-sinf(a);	m[2][2]= cosf(a);	//0		]
	// [	0			   0		        0				  1		]
	
}


void RotMatrix::rotZ(const float & a)
{
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			if (i==j)
				m[i][j]=1;
			else
				m[i][j]=0;            //Einheitsmatrix
		}
	}
	//
	/* [*/ m[0][0]= cosf(a);	m[0][1]= sinf(a);	  //0			0 	]
	/* [*/ m[1][0]=-sinf(a);	m[1][1]= cosf(a);	  //0			0 	]
	// [		0						0		        1		  	0	]
	// [		0						0		        0			1	]
	
}



void RotMatrix::rotVektor3f(const Vektor3f &vec, const float & angle)
{
	float cosA = cosf(angle), sinA = sinf(angle), invCosA = 1.0f - cosA;
	
	m[0][0]=invCosA*vec.x*vec.x + cosA;			m[0][1]=invCosA*vec.x*vec.y + sinA*vec.z;	m[0][2]=invCosA*vec.x*vec.z - sinA*vec.y;
	m[1][0]=invCosA*vec.x*vec.y - sinA*vec.z;	m[1][1]=invCosA*vec.y*vec.y + cosA;			m[1][2]=invCosA*vec.y*vec.z + sinA*vec.x;
	m[2][0]=invCosA*vec.x*vec.z + sinA*vec.y;	m[2][1]=invCosA*vec.y*vec.z - sinA*vec.x;	m[2][2]=invCosA*vec.z*vec.z + cosA;
	
}
