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




#ifndef _MISSILE_H
#define _MISSILE_H 1


#include "Vektor3f.h"
#include "SpaceObject.h"
#include "SpaceObjectList.h"

class Missile : public SpaceObject  
{
public:
	Missile(SpaceObject *,Vektor3f,Vektor3f,int);
	virtual ~Missile();
	virtual void reCalc();
    virtual inline int getLife() const {return myLife;}
	void traceRoute();
	
private:
	SpaceObject* target;
	Vektor3f routeNode[8];
	Vektor3f routeGradient;
	int myLife;
	int updInt; //updateintervall->lazy updates-dadurch hoffentlich coole Raketenbahnen :)
	int tickCount;
	int pattern,factor;
	
};

#endif 
