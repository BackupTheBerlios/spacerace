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




#ifndef _BOUNDINGFIELD_H
#define _BOUNDINGFIELD_H 1


//#include <qobject.h>

#include "Vektor3f.h"
#include "SpaceObject.h"
#include "SpaceObjectList.h"


class BoundingField :public SpaceObject
{


	Q_OBJECT


	public:

		BoundingField(const float&);	// sphere
		BoundingField(const float&, const float&, const float&);	// box
		virtual ~BoundingField();


		// falls Schiff in das Field eintritt
		inline bool collisionPoly(const Vektor3f& before,const  Vektor3f& after,const  float& shbrad)
		{

			float tmpBoundRad = myBoundRad;
			myBoundRad = shbrad;


/*			if (isbox)
			{
// *** hier oder im ship: bei box-field teste ob in _box_, nicht ob in bounding-_sphere_
			};
*/

			SpaceObject* sobj;

			for (int i=0; i<mySubObjects->getCount(); i++)	// teste alle subObjects auf Kollision
			{

				sobj = mySubObjects->getSpObj(i);

				if ( collisionBoundSphere(before,after,sobj) )		// if bounding spheres collide?
				{
					if (sobj->collisionPoly(before,after,shbrad))		// then exact collision detection of object itself
					{
						myBoundRad = tmpBoundRad;
						return true;
					};
				};

			};

			myBoundRad = tmpBoundRad;
			return false;
		};



		inline void addItem( SpaceObject* s ) { mySubObjects->addItem(s); }; // SubObjekt hinzufügen

		inline void reset() { mySubObjects->reset(); };	// alle SubObjekte löschen

		inline void resetGates() { mySubObjects->resetGates(); }; // falls Gates im Field, diese reseten

		inline SpaceObject* getSpObj(const int& nr)const { return mySubObjects->getSpObj(nr); } 	// SubObjekt[nr] zurückgeben

		inline int getCount() const { return mySubObjects->getCount(); }	// Anzahl der SubObjekte

		inline Vektor3f getBox() { return Vektor3f(myWidth,myHeight,myLength); };	// falls Box, Ausdehnung zurückgeben


		inline virtual void reCalc()
		{
			//SpaceObject::reCalc();	// falls sich das Field auch bewegen soll

			SpaceObject* sobj;

			Vektor3f dist;	// Abstand FieldMitte-SubObjektPos
			Vektor3f mov;	// Movement-Vektor eines SubObjekts, nicht ständig neu erzeugen, wird unten gebraucht


			for (int i=0; i<(mySubObjects->getCount()); i++)	// alle SubObjekte durchgehen
			{
				sobj = mySubObjects->getSpObj(i);	// SubObjekt holen
				sobj->reCalc();		// SubObjekt bewegen

				dist = (sobj->getPos())-myPos;	// Abstand von Field-Mitte zu SubObjekt unabhängig von Orientierung
				float objBR = sobj->getBoundRad();	// SubObjekt BoundingRadius

				if (isbox)	// falls Box-Field
				{
					if ( (fabs(dist.x)+objBR) > myWidth )	// falls außerhalb x-Beschränkung
					{
						mov = sobj->getMovDir();
						mov.x = -mov.x;
						sobj->setMovement(mov);
					};

					if ( (fabs(dist.y)+objBR) > myHeight )	// falls außerhalb y-Beschränkung
					{
						mov = sobj->getMovDir();
						mov.y = -mov.y;
						sobj->setMovement(mov);
					};

					if ( (fabs(dist.z)+objBR) > myLength )	// falls außerhalb z-Beschränkung
					{
						mov = sobj->getMovDir();
						mov.z = -mov.z;
						sobj->setMovement(mov);
					};
				}
				else	// falls Sphere-Field
				{
					if ( dist.sqrLength() > (myBoundRad-objBR)*(myBoundRad-objBR) )	// falls Objekt+objBoundRad außerhalb Field-Kugel
					{
						dist.normalize();
						mov = (sobj->getMovDir()) - dist;	// dann Bewegungsrichtung verbiegen
						mov.normalize();
						sobj->setMovement(mov);
					};	// if
				};

			};	// for


		};	// reCalc


		bool isbox;



	private:

		float shipBoundRad;
		float myWidth;
		float myHeight;
		float myLength;

		SpaceObjectList* mySubObjects;

};

#endif
