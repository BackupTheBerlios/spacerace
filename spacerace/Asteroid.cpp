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



#include "Asteroid.h"



Asteroid::Asteroid(Mesh* collM)
{
	myObjType=20;
	myPos=Vektor3f(0.0f, 0.0f, 0.0f);
	myOri=Vektor3f(0.0f, 0.0f, 1.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);
	
	
	// Standartwerte setzen:
	// zufällige Rotation
	float zuf1 = ((float)(rand()%100))/10000.0f;	// rot-geschwindigkeit je feste raum-achse
	float zuf2 = ((float)(rand()%100))/10000.0f;
	float zuf3 = ((float)(rand()%100))/10000.0f;
	if ( (rand()%2)==0 ) { zuf1 = -zuf1; };		// positive oder negative drehrichtung?
	if ( (rand()%2)==0 ) { zuf2 = -zuf2; };
	if ( (rand()%2)==0 ) { zuf3 = -zuf3; };
	setEigenRot(zuf1,zuf2,zuf3);
	
	// zufällige Bewegung
	float zufmov = ((float)((rand()%3000)+1))/55000;	// mov-geschwindigkeit
	Vektor3f zMov(zuf1,zuf2,zuf3);
	zMov.normalize();
	setMovement(zMov,zufmov);
	
	myCollMesh = collM;
	tri = myCollMesh->myTri;
	node = myCollMesh->myNode;
	triSize = myCollMesh->myTriSize;
	
	myBoundRad=myCollMesh->myBoundRad;	// Radius of Asteroid Bounding Sphere
}



Asteroid::~Asteroid()
{

}


#include"Asteroid_moc.cpp"
