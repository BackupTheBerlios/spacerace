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




#include "Matrix4x4.h"


Matrix4x4::Matrix4x4()
{
	for (int i=0;i<4;i++){
		for (int j=0;j<4;j++){
			if (i==j)
				m[i][j]=1;
			else
				m[i][j]=0;            //Einheitsmatrix
		}
	}
}

Matrix4x4::~Matrix4x4()
{
}

Matrix4x4::Matrix4x4(const Vektor3f &a, const Vektor3f &b, const Vektor3f &c, const Vektor3f &d)
{
	
	m[0][0]=a.x;	m[0][1]=b.x;	m[0][2]=c.x; 	m[0][3]=d.x;
	m[1][0]=a.y;	m[1][1]=b.y;	m[1][2]=c.y;	m[1][3]=d.y;
	m[2][0]=a.z;	m[2][1]=b.z;	m[2][2]=c.z;	m[2][3]=d.z;
	m[3][0]=0.0f;	m[3][1]=0.0f;	m[3][2]=0.0f;	m[3][3]=1.0f;
}


void Matrix4x4::show(void)
{
	printf("Homogene Matrix\n");	
	for (int i=0;i<4;i++)
	{
		printf("( %.3f %.3f %.3f %.3f)\n",m[i][0],m[i][1], m[i][2],m[i][3] );
	};
}
