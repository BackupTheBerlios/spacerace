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
 *  along with Spacerace; if not, write to the Free Software						*
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	*
 *																				*
 ********************************************************************************/




#include "Nebula.h"


Nebula::Nebula()
{
	
	myObjType=50;
	for (int i=0;i<200;i++)
	{
		
		opac[i]=0.6f;
		Vektor3f randVec;
		randVec.x=-30.0f+rand()%300*0.2f;
		randVec.y=-30.0f+rand()%300*0.2f;
		randVec.z=-30.0f+rand()%300*0.2f;
		float length=8+rand()%250 *0.2f;
		randVec.normalize();
		randVec=randVec*length;

	
		Vektor3f q=myPos;
		q=q+randVec;
		pos[i]=q;
//		printf("pos %d :",i);
//		pos[i].show();
	}

}

Nebula::~Nebula()
{

}
