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




#include "Gate.h"


Gate::Gate(Mesh* collM)
{
	myObjType=10;
	myPos=Vektor3f(0.0f, 0.0f, 0.0f);
	myOri=Vektor3f(0.0f, 0.0f, 1.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);
	myX=myOri.kreuz(myUp);
	myX.normalize();

	myRotX.rotX(0);
	myRotY.rotY(0);
	myRotZ.rotZ(0);
	rotating = false;
	moving = false;

	myMovDir=Vektor3f(0.0f, 0.0f, 0.0f);
	speed=0.0f;

	myCollMesh = collM;
	tri = myCollMesh->myTri;
	node = myCollMesh->myNode;
	triSize = myCollMesh->myTriSize;

	myBoundRad=myCollMesh->myBoundRad;	// Radius of Gate Bounding Sphere
	CheckSphereBoundRad=7.5f;	// Radius of Checkpoint-Detection Sphere, ANPASSEN!!
	checkBothSides = false;		// beidseidiges Durchfliegen

	succGate = NULL;
}


Gate::~Gate()
{

}



bool Gate::collCheckSphere(const Vektor3f &before, const Vektor3f &after, const float &sbrad) const
{

	Vektor3f movement = after-before;
	Vektor3f distance = (myPos-before);

	float sqrMovLength = movement.sqrLength();

	float RadSum = CheckSphereBoundRad+ sbrad;

	float dist = distance.length();

	float distMinusRadSum = dist - CheckSphereBoundRad - sbrad;


	// erster Test, wenn ( movement < abstand ) dann keine Kollision, Richtung nicht ber�cksichtigt
	if ( sqrMovLength < (distMinusRadSum*distMinusRadSum) ) { return false; };

	// zweiter Test a, falls dotproduct <= 0 dann bewegen sich die Kugeln nicht aufeinander zu, also kann keine Kollision stattfinden
	Vektor3f movementN = movement;
	movementN.normalize();
	float DdotM = movementN.dot(distance);		// (distance on movement-vector to closest point to objPos)
	if (DdotM <= 0) { return false; };

	// zweiter Test b, richtige Durchflugrichtung durch Tor
	if (!checkBothSides)
	{
		float OdotM = movementN.dot(myOri);
		if (OdotM <= 0) { return false; };
	};

	// dritter Test, wenn der Abstand am n�hesten Punkt gr��er ist als die Summe der Radien, dann keine Kollision
	float sqrRadSum = (CheckSphereBoundRad+sbrad) * (CheckSphereBoundRad+sbrad);
	float sqrF = (dist*dist)-(DdotM*DdotM);		// quadrat des Abstandes von objPos zum n�hesten Punkt zu objPos auf movement-vektor (Lot-Fu�punkt)
	if ( sqrF >= sqrRadSum ) { return false; };

	// viertens, Ausnahmeregelung falls es kein richtiges Dreieck gibt, damit nicht wurzel(x<0)
	float sqrT = sqrRadSum - sqrF;
	if (sqrT<0) { return false; };

	// f�nftens, travel = die Distanz die wir auf dem movement-Vektor zur�cklegen k�nnen, bis wir mit dem objBounding-Sphere zusammensto�en
	// wenn diese Distanz l�nger als der movement-Vektor ist, dann keine Kollision
	float travel = DdotM - sqrt(sqrT);
	if ( sqrMovLength < (travel*travel) ) { return false; };

	// der punkt an dem sie kollidieren, (brauchen wir eigentlich nicht, da genauere Koll.Abfrage folgt)
	//Vektor3f collPoint = movementN * travel;

	return true;
};



#include"gate_moc.cpp"
