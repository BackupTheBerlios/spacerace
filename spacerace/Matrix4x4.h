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




#ifndef _H_MATRIX_4X4_H
#define _H_MATRIX_4X4_H

#include "Vektor3f.h"
#include <stdio.h>
#include <math.h>



class Matrix4x4  
{
public:
	Matrix4x4();
	Matrix4x4(const Vektor3f &a, const Vektor3f &b, const Vektor3f &c, const Vektor3f &d);
	
	virtual ~Matrix4x4();
	float m[4][4];
	
	void show();
	
    
	inline Vektor3f operator*(const Vektor3f &mul) const
	{
		float w=1/(m[3][0]*mul.x + m[3][1]*mul.y + m[3][2] *mul.z + m[3][3]);
		Vektor3f erg(
			w*(m[0][0]*mul.x + m[0][1]*mul.y + m[0][2] *mul.z + m[0][3]),//*mul.w,
			w*(m[1][0]*mul.x + m[1][1]*mul.y + m[1][2] *mul.z + m[1][3]),//*mul.w,
			w*(m[2][0]*mul.x + m[2][1]*mul.y + m[2][2] *mul.z + m[2][3]));//*mul.w);
		//erg.setW(m[3][0]*mul.x + m[3][1]*mul.y + m[3][2] *mul.z + m[3][3]/*mul.w*/);
		return erg;
	}
	
	
	
	inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mul)const
	{
		Matrix4x4 erg;
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
	
	
	
	
	inline Matrix4x4 Matrix4x4::transpose(void) const
	{
		Matrix4x4 erg;
		for (int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				erg.m[i][j]=m[j][i];
			};
			return erg;
	}
	
	
	
};

#endif 