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




#include "BoundingField.h"


BoundingField::BoundingField(const float& rad)
{
	myObjType=30;
	myPos=Vektor3f(0.0f, 0.0f, 0.0f);
	myOri=Vektor3f(0.0f, 0.0f, 1.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);
	
	myRotX.rotX(0);
	myRotY.rotY(0);
	myRotZ.rotZ(0);
	rotating = false;
	moving = false;
	myMovDir=Vektor3f(0.0f, 0.0f, 0.0f);
	speed=0.0f;
	
	myBoundRad = rad;
	shipBoundRad = 0.0f;
	myWidth = 0.0f;
	myHeight = 0.0f;
	myLength = 0.0f;
	isbox = false;
	
	myCollMesh = NULL;
	node = NULL;
	tri = NULL;
	triSize = 0;
	
	mySubObjects = new SpaceObjectList();
}



BoundingField::BoundingField(const float& width, const float& height, const float& length)
{
	myObjType=30;
	myPos=Vektor3f(0.0f, 0.0f, 0.0f);
	myOri=Vektor3f(0.0f, 0.0f, 1.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);
	
	myRotX.rotX(0);
	myRotY.rotY(0);
	myRotZ.rotZ(0);
	rotating = false;
	moving = false;
	myMovDir=Vektor3f(0.0f, 0.0f, 0.0f);
	speed=0.0f;
	
	shipBoundRad = 0.0f;
	
	myWidth = width;
	myHeight = height;
	myLength = length;
	
	myBoundRad = sqrtf(myWidth*myWidth + myHeight*myHeight + myLength*myLength);
	
	isbox = true;
	
	myCollMesh = NULL;
	node = NULL;
	tri = NULL;
	triSize = 0;
	
	mySubObjects = new SpaceObjectList();
}


BoundingField::~BoundingField()
{
	delete mySubObjects;
}


#include"BoundingField_moc.cpp"
