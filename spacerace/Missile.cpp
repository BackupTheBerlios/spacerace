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




#include "Missile.h"

Missile::Missile(SpaceObject* so, Vektor3f origin,Vektor3f dir, int type)
{

	myPos=origin;
	myOri=Vektor3f(1.0f, 0.0f, 0.0f);
	myUp=Vektor3f(0.0f, 1.0f, 0.0f);

	myRotX.rotX(0.2f);
	myRotY.rotY(0.2f);
	myRotZ.rotZ(0.2f);
	myMovDir=Vektor3f(1.0f,0.0f,0.0f);
	rotating = false;
	moving = true;
	target=so;
	tickCount=0;
	routeNode[0]=myPos;
	routeNode[6]=myPos;
		if (type==0) //rakete
	{	myObjType = 30;
		myLife=3000; //60 Sekunden
		updInt=5; 
		speed=1.1f;
		routeNode[7]=origin+dir;
	}
	else 
	{
		myObjType=31; //laser
		speed=1.3f;
		routeNode[7]=origin+dir;
		myLife=300;
	}
	if(target!=NULL)
	routeGradient=target->getPos()-myPos;
	routeGradient.normalize();
	
	

}

Missile::~Missile()
{
  
}

void Missile::reCalc()
{
	
	if (myObjType==30)
	{
		if ((target->getPos()-myPos).length() <=1.5f)
		{
			printf("Krachbumm!\n");
			myLife=0;
			return;
		}
		if (tickCount==updInt) //lazy update um die trägheit der rakete zu "simulieren"
		{
			
			if ((myOri.dot(target->getPos()-myPos)) >=0) //noch nicht am Ziel vorbeigeschossen!
			{
				printf("target lost!\n");
				routeNode[6]=myPos;
				if(target!=NULL)
				routeNode[7]=target->getPos();
			}
			else 
			{
				pattern++;
				routeNode[6]=myPos;
				if (pattern<=18*factor)
				myOri=myRotX*myOri;
				else
				{
					myOri=myRotY*myOri;
				}
				if (pattern>=36*factor)
					myOri=myRotZ*myOri;
				if (pattern>=54*factor) 
				{
					pattern=0;
					if( factor==1)
						factor=2;
					else factor=1;
				}
				
				routeNode[7]=myOri+myPos;
			};
			tickCount=0;
		};
		tickCount++;
		routeGradient=routeNode[7]-routeNode[6];
		routeGradient.normalize();
		myMovDir=routeGradient;
		myOri=routeGradient;
		myPos=myPos+myMovDir*speed;
		if (myLife) myLife--;
	}
	else //myType==31
	{
		tickCount++;
		routeGradient=routeNode[7]-routeNode[6];
		routeGradient.normalize();
		myMovDir=routeGradient;
		myOri=routeGradient;
		myPos=myPos+myMovDir*speed;
		if(myLife) myLife--;
	}
}



void Missile::traceRoute()
{

}

#include "Missile_moc.cpp"