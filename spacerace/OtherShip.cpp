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




#include "OtherShip.h"



OtherShip::OtherShip(Mesh* collM)
{
	myObjType = o_OtherShip;
	myPos=Vektor3f(0.0f, 0.0f, 0.0f);
	myOri=Vektor3f(1.0f, 0.0f, 0.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);
	myX=myOri.kreuz(myUp);
	myX.normalize();
	
	myCollMesh = collM;
	tri = myCollMesh->myTri;
	node = myCollMesh->myNode;
	triSize = myCollMesh->myTriSize;
	
	myBoundRad=myCollMesh->myBoundRad;	// Radius of Asteroid Bounding Sphere
	
	myRotX.rotX(0);
	myRotY.rotY(0);
	myRotZ.rotZ(0);
	myMovDir=Vektor3f(0.0f,0.0f,0.0f);
	rotating = false;
	moving = false;
}

OtherShip::~OtherShip()
{
	
}

/*bool OtherShip::collision(const Vektor3f& before,const  Vektor3f& after,const  float& shbrad)
{
//TO-DO=====

  Vektor3f movement = after-before;
  Vektor3f distance = (this->getPos()-before);
		// zweiter Test, falls dotproduct <= 0 dann bewegen sich die Kugeln nicht aufeinander zu, also kann keine Kollision stattfinden
		// (distance on movement-vector to closest point to objPos)
		if (movement.dot(distance) <= 0) { return false; };
		
		  printf("OtherShip "); 
		  return true;
		  };
*/