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





#ifndef _SPACEOBJECT_H
#define _SPACEOBJECT_H 1

#include "Vektor3f.h"
#include "RotMatrix.h"
#include "State.h"
#include <qobject.h>
#include <stdio.h>

#include "Mesh.h"
#include "Matrix4x4.h"

class SpaceObject : public QObject
{
	Q_OBJECT


	public:

		Vektor3f myX;
//		Vektor3f sphr1,sphr2;
		
		inline void setPos(const Vektor3f& pos)	{ myPos=pos; };

		inline virtual void setLR(int i) {};
		inline virtual void setUD(int i) {};


		inline SpaceObject()
		{
			myObjType=-1;
			myPos=Vektor3f(0.0f, 0.0f, 0.0f);
			myOri=Vektor3f(0.0f, 0.0f, 1.0f);
			myUp=Vektor3f(0.0f, 1.0f, 0.0f);
			myX=myOri.kreuz(myUp);
			speed=0;
			myBoundRad=0.1f;

			CheckPointPassed=false;
			CheckPointAllowed=false;

			myRotX.rotX(0);
			myRotY.rotY(0);
			myRotZ.rotZ(0);
			myMovDir=Vektor3f(0.0f,0.0f,0.0f);
			rotating = false;
			moving = false;

			myCollMesh = NULL;
			node = NULL;
			tri = NULL;
			triSize = 0;
		}


		virtual inline ~SpaceObject() {}


		inline virtual void reCalc()
		{
			if (rotating)
			{
				myOri = myRotX * myOri;
				myUp = myRotX * myUp;

				myOri = myRotY * myOri;
				myUp = myRotY * myUp;

				myOri = myRotZ * myOri;
				myUp = myRotZ * myUp;

				myX=myOri.kreuz(myUp);

			};

			if (moving)
			{
				myPos = myPos + ( myMovDir*speed );
			};
		};


		inline Vektor3f getPos() const { return myPos; };

		inline Vektor3f getOri()  const { return myOri; };

		inline Vektor3f getUp()  const { return myUp; };

		inline float getBoundRad()  const { return myBoundRad; };

		inline void setBoundRad(const float& r)   {  myBoundRad=r; };

		inline int getObjType()  const { return myObjType; };

		inline float getSpeed()  const { return speed; };

		inline void setSpeed(const float& s) { speed=s; };

		inline virtual int getSpeedSet()  const {return 0;}; 

		inline virtual int getSpeedMax()  const {return 0;}; 

		inline virtual int getSpeedReal()  const {return 0;}; 

		inline virtual int getLife() const {return -1;};
		
		inline virtual int getFuel() const {return -1;};

		virtual inline Vektor3f getPartPos(int i) const {return myPos;}
		virtual inline float getPartOpac(int i) const {return 0.0f;}
		virtual inline void setPartOpac(int , float ) {}

		inline State getState() const
		{
			State myState;
			myState.objType=myObjType;
			myState.ori=myOri;
			myState.pos=myPos;
			myState.up=myUp;
		//	myState.show();
			return myState;
		};

		inline void setState(const State& st)
		{
			myObjType=st.objType;
			myOri=st.ori;
			myPos=st.pos;
			myUp=st.up;
			myX=myOri.kreuz(myUp);
			myX.normalize();
		};




		inline virtual bool collisionPoly(const Vektor3f& before,const  Vektor3f& after,const  float& shbrad)
		{

			float ShipBoundRad = shbrad;

			Vektor3f movement = after-before;

		//---Fuer die sphere-poly prüfen, ob mehrere Tests erforderlich sind
			int parts=1;//= Anzahl der Tests
			float partLen=1.0f;
			if (movement.length()>(ShipBoundRad))
			{
					parts= (int)((movement.length()/ShipBoundRad)+0.5f);
					partLen=1.0f/(float)parts;
			}
			
			Vektor3f step;
			step=movement*partLen;


			float SqrShipBoundRad=ShipBoundRad*ShipBoundRad;	
		//---

			for (int polyNr=0;polyNr<triSize;polyNr++) 
			{
				// Eckpunkte des Dreiecks
				Vektor3f t1 = node[ tri[polyNr].w[0] ];
				Vektor3f t2 = node[ tri[polyNr].w[1] ];
				Vektor3f t3 = node[ tri[polyNr].w[2] ];


				Vektor3f xa = myOri.kreuz(myUp);
				xa.normalize();
				Matrix4x4 transform(myUp,xa,myOri,myPos);

				t1=transform*t1;
				t2=transform*t2;
				t3=transform*t3;
				

				// zwei Spann-Vektoren des Dreiecks
				Vektor3f v1 = t2-t1;
				Vektor3f v2 = t3-t1;

		//-----------OPTIMIZE sphere-coll----------
		//Werte werden hier berechnet, damit sie bei mehreren Spheren
		//nicht mehrmals kalkuliert werden müssen...
				Vektor3f v3 = t2-t3;

				float v1_len=v1.length();
				float v2_len=v2.length();
				float v3_len=v3.length();

				Vektor3f v1_normal=v1/v1_len;
				Vektor3f v2_normal=v2/v2_len;
				Vektor3f v3_normal=v3/v3_len;
		//-----------

				// Normale des Dreiecks
				Vektor3f tNorm = v2.kreuz(v1);
			//	tNorm.normalize();

				//Vektor3f movNorm = movement;
				Vektor3f nMov=movement*-1.0f;
			//	movNorm.normalize();

				float ori = movement.dot(v2.kreuz(v1));		// (mov)*(v2xv1) > 0 bedeutet in diesem Fall, daß 
													// die Normale des Polygons in Richtung Kamera zeigt
													// (also eigentlich entgegen des Move-Vektors)

				if (ori>0)	// Test1, nur Kolli. wir uns in Richtung Dreieck bewegen
				{
					//printf("Ori OK\n");

					float detM =v1.x*v2.y*nMov.z  + v2.x*nMov.y*v1.z + nMov.x*v1.y*v2.z 
							   -nMov.x*v2.y*v1.z  - v1.x*nMov.y*v2.z - v2.x*v1.y*nMov.z;
					//Determinate aus v1 v2 nMov

					if (detM==0) { break; };//falls det=0 Mov parallel zur Ebene

					Vektor3f Q = before-t1;

			
					float detL1 =Q.x*v2.y*nMov.z  + v2.x*nMov.y*Q.z + nMov.x*Q.y*v2.z 
							   -nMov.x*v2.y*Q.z  - Q.x*nMov.y*v2.z - v2.x*Q.y*nMov.z;
					//Determinate aus Q v2 nMov

					float lambda = detL1 / detM;

					
					float detL2 =v1.x*Q.y*nMov.z  + Q.x*nMov.y*v1.z + nMov.x*v1.y*Q.z 
							   -nMov.x*Q.y*v1.z  - v1.x*nMov.y*Q.z - Q.x*v1.y*nMov.z;
					//Determinate aus v1 Q nMov

					float my = detL2 / detM;


					if ( (lambda>=0) && (my>=0) && ((lambda+my)<=1) )	// Test2a, der Schnittpunkt der mov-Gerade mit der Dreiecks-Ebene liegt im Dreieck
					{

						Vektor3f SchnittP = t1 + v1*lambda + v2*my;

						Vektor3f dist = SchnittP-before;

						if (dist.length() < (movement.length()+ShipBoundRad))	// Test3a, die Entfernung vom Schnittpunkt zum Spheren-Mittelpunkt muß kleiner mov+sphRad sein
						{
		//					printf("  **  POLYG hit\n");
							return true;
						};

					}
					else	// Test 2b Schnittpunkt der mov-Gerade mit der Dreiecks-Ebene liegt außerhalb des Dreiecks.
					{
						Vektor3f SchnittP = t1 + v1*lambda + v2*my;


						//parts= Anzahl der Tests
						//partLen Laenge der Segmente

						Vektor3f testCen;
						testCen=before;

						for (int j=1;j<=parts;j++)
						{
							testCen=testCen+step;
							Vektor3f F;// = closestPointOnLine(t1, t2, testCen);
								

							//-----------------1st Edge-----------
							Vektor3f vVector1 = testCen - t1;

							float t = v1_normal.dot(vVector1);

							if (t <= 0) 
								F = t1;
							else
							{
								if (t >= v1_len) 
									F = t2;
								else
									F = t1 + (v1_normal * v1_len); 			
							}


							float distance = (F-testCen).length();
							if((F-testCen).sqrLength() < SqrShipBoundRad)		// Test3b, Abstand zum Fußpunkt < Radius
							{
		//						printf("sphere-poly col 1\n");
								return true;
							}
								

							//______________________2nd Edge_____________

							t = v2_normal.dot(testCen - t1);

							if (t <= 0) 
								F= t1;
							else
							{
								if (t >= v2_len) 
									F= t3;
								else
									F=t1 + (v2_normal * t);
							}

							if((F-testCen).sqrLength() < SqrShipBoundRad)		// Test3b, Abstand zum Fußpunkt < Radius
							{
		//						printf("sphere-poly col 3\n");
								return true;
							}

							//-------------3rd Edge-------------------------
							//F = closestPointOnLine(t3, t2, testCen);
							vVector1 = testCen - t3;
							t = v3_normal.dot(vVector1);

							if (t <= 0) 
								F= t3;
							else
							{
								if (t >= v3_len) 
									F= t2;
								else
								{					
									F=  t3 + (v3_normal * t);					
								}
							}

							//distance = (F-testCen).length();
							if((F-testCen).sqrLength() < SqrShipBoundRad)		// Test3b, Abstand zum Fußpunkt < Radius
							{
		//						printf("sphere-poly col 2\n");
								return true;
							}


						}//for all spheres
					}// if else
				};// ori
			}//for all polys

			//printf(" no hit \n");
			return false;
		};




		inline bool collisionBoundSphere(const Vektor3f& before,const  Vektor3f& after,const  SpaceObject* so) const
		{

			float objBoundRad=so->getBoundRad();		// Radius of Objects Bounding Sphere

			Vektor3f objPos = so->getPos();
			Vektor3f movement = after-before;
			Vektor3f distance = (objPos-before);

			float sqrMovLength = movement.sqrLength();

			float dist = distance.length();
			float distMinusRadSum = dist - myBoundRad - objBoundRad;


			// nullter Test
			if (distMinusRadSum < 0) 
			{
				return true;
			};

			// erster Test, wenn ( movement < abstand ) dann keine Kollision, Richtung nicht berücksichtigt
			if ( sqrMovLength < (distMinusRadSum*distMinusRadSum) ) { return false; };


			// zweiter Test, falls dotproduct <= 0 dann bewegen sich die Kugeln nicht aufeinander zu, also kann keine Kollision stattfinden
			Vektor3f movementN = movement;
			movementN.normalize();
			float DdotM = movementN.dot(distance);		// (distance on movement-vector to closest point to objPos)


		// wegen nulltem test überflüssig, da immer true sobald innerhalb
		/*	int objT = so->getObjType();
			if ( !((objT==o_Gate1) || (objT==o_Gate2)) && (DdotM <= 0) )
			{
				//  raus weil Kollision beim Gate auch von innen möglich, also z.B. Flug von Tormittelpunkt nach aussen muß auch Kollision mit Torumrandung ergeben
				return false;
			};
		*/

			// dritter Test, wenn der Abstand am nähesten Punkt größer ist als die Summe der Radien, dann keine Kollision
			float sqrRadSum = (myBoundRad+objBoundRad)*(myBoundRad+objBoundRad);
			float sqrF = (dist*dist)-(DdotM*DdotM);		// quadrat des Abstandes von objPos zum nähesten Punkt zu objPos auf movement-vektor (Lot-Fußpunkt)
			if ( sqrF >= sqrRadSum ) { return false; };

			// viertens, Ausnahmeregelung falls es kein richtiges Dreieck gibt, damit nicht wurzel(x<0)
			float sqrT = sqrRadSum - sqrF;
			if (sqrT<0) { return false; };

			// fünftens, travel = die Distanz die wir auf dem movement-Vektor zurücklegen können, bis wir mit dem objBounding-Sphere zusammenstoßen
			// wenn diese Distanz länger als der movement-Vektor ist, dann keine Kollision
			float travel = DdotM - sqrtf(sqrT);
			if ( sqrMovLength < (travel*travel) ) { return false; };

		//	printf("** oboundsphere **\n");
			return true;

		};



		bool CheckPointPassed; 
		bool CheckPointAllowed;


		inline void setEigenRot(const float& rX, const float& rY, const float& rZ)
		{
			if ( (rX!=0.0f) || (rY!=0.0f) || (rZ!=0.0f) )
			{
				myRotX.rotVektor3f(myX,rX);
				myRotY.rotVektor3f(myUp,rY);
				myRotZ.rotVektor3f(myOri,rZ);
				rotating = true;
			}
			else
			{
				rotating = false;
			};
		};


		inline void setMovement(const Vektor3f& direction, const float& sp)
		{
			if ( ((direction.x!=0.0f) || (direction.y!=0.0f) || (direction.z!=0.0f)) && (sp!=0.0f) )
			{
				myMovDir = direction;
				myMovDir.normalize();
				speed = sp;
				moving = true;
			}
			else
			{
				myMovDir=Vektor3f(0.0f,0.0f,0.0f);
				speed=0.0f;
				moving = false;
			}
		};


		inline void setMovement(const Vektor3f& direction)
		{
			if ( ((direction.x!=0.0f) || (direction.y!=0.0f) || (direction.z!=0.0f)) && (speed!=0.0f))
			{
				myMovDir = direction;
				myMovDir.normalize();
				moving = true;
			}
			else
			{
				myMovDir=Vektor3f(0.0f,0.0f,0.0f);
				speed=0.0f;
				moving = false;
			}
		};


		inline Vektor3f getMovDir()  const { return myMovDir; };




	protected:

		Vektor3f myPos;		// Position
		Vektor3f myOri;		// Orientation
		Vektor3f myUp;		// Up
	
		float speed;		// Speed
		RotMatrix myRotX;	// Rotationsmatrizen
		RotMatrix myRotY;
		RotMatrix myRotZ;
		Vektor3f myMovDir;	// Bewegungsrichtung
		bool rotating;		// rotierend?
		bool moving;		// bewegend?

		float myBoundRad;	// Radius for Bounding-Sphere

		int myObjType;		// Identifier for Object Type


		Mesh* myCollMesh;
		Vektor3f *node;
		Face *tri;
		int triSize;

};

#endif 
