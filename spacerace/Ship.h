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




#ifndef _SHIP_H
#define _SHIP_H 1

#include "Vektor3f.h"
#include "RotMatrix.h"
#include "State.h"
#include "SpaceObjectList.h"

#include <qobject.h>
#include <stdio.h>

#include "MySound.h"



class Ship :public SpaceObject
{
	Q_OBJECT
	public:

		Ship();
		virtual ~Ship();

		void reCalc(const SpaceObjectList*);
		inline virtual int getSpeedSet() const {return speed_set;};
		inline virtual void setSpeedSet(int sps) { speed_set = sps; };
		inline virtual int getSpeedMax() const {return speed_max;};
		inline virtual int getSpeedReal() const {return speed_real;};
		inline virtual void setSpeedReal(int spr) { speed_real = spr; };
		inline virtual void setPos(const Vektor3f& v) { myPos=v; };
		inline virtual int getFuel() const {return fuel;};

		inline virtual void setLR(int i) {lrMax=i;};
		inline virtual void setUD(int i) {udMax=i;};
		
		bool mouseMode;
		int leftright;
		int updown;
		int cwccw;
		int lrMax,udMax;

		
		bool k1;
		bool k2;
		bool k3;
		bool k4;
		bool k5;
		bool k6;
		bool k7;
		bool k8;
		bool k9;
		bool kpl;
		bool kmi;
		bool ktab;



	private:

		MySound * sndKlonk;

		Vektor3f newPos;

		int fuel;

		
		float max_yRot;
		
		float max_xRot;
		
		float max_zRot;

		int speed_max;
		int speed_set;
		int speed_real;

		int tickCounter;



	public slots:
		void sltKey(int);
		void sltKeyRelease(int);


};

#endif