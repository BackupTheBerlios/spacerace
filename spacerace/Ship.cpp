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




#include "Ship.h"


Ship::Ship()
{
	myObjType = 0;
	myPos=Vektor3f(0.0f, 0.0f, 0.0f);
	myOri=Vektor3f(1.0f, 0.0f, 0.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);
	
	myRotX.rotX(0);
	myRotY.rotY(0);
	myRotZ.rotZ(0);
	myMovDir=Vektor3f(0.0f,0.0f,0.0f);
	rotating = false;
	moving = false;
	
	k1=false;
	k2=false;
	k3=false;
	k4=false;
	k5=false;
	k6=false;
	k7=false;
	k8=false;
	k9=false;
	kpl=false;
	kmi=false;
	ktab=false;
	
	leftright=0;
	updown=0;
	cwccw=0;
	
	tickCounter=0;
	
	speed=0.0f;
	speed_real=0;
	speed_set=20;
	speed_max=MAX_SPEED*1.5f;
	
	mouseMode=0;
	// *** TO-DO: ***  An Tickrate anpassen!!!!!!!!!
	//	ACHTUNG Änderungen hier betreffen neuerdings auch das Intro!!!
	
	max_yRot = 0.0022f;
	max_xRot = 0.0028f;
	max_zRot = 0.0023f;
	
	
	myBoundRad=1.0f;	// Radius of Ship Bounding Sphere, anpassen!
	
	sndKlonk = new MySound("sounds/hourglass.wav");
	
	fuel=1000;
	
}



Ship::~Ship()
{
	delete sndKlonk;
	printf("Ship deleted\n");
}



void Ship::reCalc(const SpaceObjectList* sol)
{
	
	
	// schneller / langsamer
	if ((kpl) && (speed_set<speed_max))	{ speed_set=speed_set+20; };
	if ((kmi) && (speed_set>0))	{ speed_set=speed_set-20; };
	if (speed_real<speed_set) { speed_real = speed_real+15; if (speed_real>speed_set) {speed_real=speed_set;}; };
	if (speed_real>speed_set) { speed_real = speed_real-15; if (speed_real<speed_set) {speed_real=speed_set;}; };
	
	
	// Nachbrenner
	if (ktab)
	{
		if (speed_real<3000 && fuel>0) {speed_real = speed_real+45;};
		if (fuel>0) fuel-=4;
	}
	else
	{
		if (speed_real>speed_max) { speed_real = speed_real-10; }; 
		if (fuel<999)fuel+=2;							  
	};
	
	
	speed = ((float)speed_real)/1000;
	
	
	if(!mouseMode)
	{
		
		// drehen links und rechts
		if ( k6 && (leftright<20) ) { leftright++; };
		if ( k4 && (leftright>-20) ) { leftright--; };
		if ( !k6 && (leftright>0) ) { leftright--; };
		if ( !k4 && (leftright<0) ) { leftright++; };
		myRotY.rotVektor3f( myUp, max_yRot * leftright );
		myOri=myRotY*myOri;
		
		// drehen oben und unten
		if ( k8 && (updown<20) ) { updown++; };
		if ( k2 && (updown>-20) ) { updown--; };
		if ( !k8 && (updown>0) ) { updown--; };
		if ( !k2 && (updown<0) ) { updown++; };
		myRotX.rotVektor3f( myX, max_xRot * updown );
		myOri=myRotX*myOri;
		myUp=myRotX*myUp;
		
		
	}
	else //if (mouseMode)
	{
		// drehen links und rechts
		if ( lrMax>0 && (leftright<lrMax) ) { leftright++; };
		if ( lrMax<0 && (leftright>lrMax) ) { leftright--; };
		if ( lrMax<=0 && (leftright>0) ) { leftright--; };
		if ( lrMax>=0 && (leftright<0) ) { leftright++; };
		myRotY.rotVektor3f( myUp, max_yRot * leftright );
		myOri=myRotY*myOri;
		
		// drehen oben und unten
		if ( udMax>0 && (updown<udMax) ) { updown++; };
		if ( udMax<0 && (updown>udMax) ) { updown--; };
		if ( udMax<=0 && (updown>0) ) { updown--; };
		if ( udMax>=0 && (updown<0) ) { updown++; };
		myRotX.rotVektor3f( myX, max_xRot * updown );
		myOri=myRotX*myOri;
		myUp=myRotX*myUp;
		
		
	}
	
	// rollen links und rechts
	if ( k7 && (cwccw<20) ) { cwccw++; };
	if ( k9 && (cwccw>-20) ) { cwccw--; };
	if ( !k7 && (cwccw>0) ) { cwccw--; };
	if ( !k9 && (cwccw<0) ) { cwccw++; };
	myRotZ.rotVektor3f( myOri, max_zRot * cwccw );
	myOri=myRotZ*myOri;
	myUp=myRotZ*myUp;
	
	tickCounter++;
	
	if (tickCounter>15)
	{
		tickCounter=0;
		myOri.normalize();
		myUp.normalize();
	};
	
	myX = myOri.kreuz(myUp);
	myX.normalize();
	
	myMovDir = myOri*speed;		// nicht normalisiert!
	Vektor3f myPos2 = myPos+myMovDir;
	
	
	SpaceObject* sobj;
	
	for (int i=1; i < sol->getCount(); i++)
	{
		
		sobj = sol->getSpObj(i);
		
		if (sobj->getObjType()==o_Nebula) continue;		// falls Nebel, keine Kollision möglich
		
		if ( collisionBoundSphere(myPos,myPos2,sobj) )		// bounding spheres collide?
		{
			
			if (sobj->collisionPoly(myPos,myPos2,myBoundRad))		// exact collision detection of object itself
			{
				//*************************************************
				//*** TO-DO:  finde gutes Abprall-Verhalten  ***
				//*************************************************
				
				speed_real=-speed_real*0.75f;
				speed = ((float)speed_real)/1000;
				myPos2=myPos+myOri*speed;
				
				sndKlonk->play();
				
				break;
			};
			
			
		};	// if !coll
		
	};	// for
	
	myPos=myPos2;
	
};



void Ship::sltKey(int k)
{
	switch (k)
	{
		
	case Key_1:	k1=true;
		break;
		
	case Key_2:	k2=true;	
		break;
		
	case Key_3:	k3=true;
		break;
		
	case Key_4:	k4=true;
		break;
		
	case Key_5:	k5=true;
		break;
		
	case Key_6:	k6=true;
		break;
		
	case Key_7:	k7=true;
		break;
		
	case Key_8:	k8=true;
		break;
		
	case Key_9:	k9=true;
		break;
		
	case Key_Plus:	kpl=true;
		break;
		
	case Key_Minus:	kmi=true;
		break;
		
	case Key_Backspace:	speed_set=0.0f;
		break;
		
	case Key_Tab: ktab=true;
		break;
		
	default: break;
		
	};	// switch
	//printf("key press \n");
}


void Ship::sltKeyRelease(int k)
{
	switch (k)
	{
		
	case Key_1:	k1=false;
		break;
		
	case Key_2:	k2=false;
		break;
		
	case Key_3:	k3=false;
		break;
		
	case Key_4:	k4=false;
		break;
		
	case Key_5:	k5=false;
		break;
		
	case Key_6:	k6=false;
		break;
		
	case Key_7:	k7=false;
		break;
		
	case Key_8:	k8=false;
		break;
		
	case Key_9:	k9=false;
		break;
		
	case Key_Plus:	kpl=false;
		break;
		
	case Key_Minus:	kmi=false;
		break;
		
	case Key_Tab: ktab=false;
		break;
		
	default: break;
		
	};
	
}


#include"ship_moc.cpp"
