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



       
#ifndef _PAINT_WIDGET_H
#define _PAINT_WIDGET_H 1


// Die Schnittstellen zu den benoetigten Qt-Klassen
#include <qwidget.h>
#include <qtimer.h>
#include <qimage.h>

#include<qwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>

#include <stdio.h>
#include <qfiledialog.h> 
#include <qstring.h> 
#include <qtextstream.h>
#include <qstringlist.h>
#include <vector>
#include <math.h>
#include <windows.h>

#include <qgl.h>            // Header File For The QT OpenGL32 Library
#include "glext.h"


#include "SpaceObjectList.h"
#include "Gate.h"
#include "BoundingField.h"
#include "Ship.h"


#include "enum_names.h"
#include "Menu.h"
#include "Nebula.h"



class PaintWidget:public QGLWidget	       //erbt QGLWidget
{

	struct Face
	{
		unsigned int w[3];
	};


	struct quadparticles
	{
		Vektor3f pos,lo,lu,ro,ru;
		float life,fade;
		bool active;
	};

	
	struct particles
	{
		Vektor3f pos,moveVec;
		float life,fade;
		bool active;
	};

		struct objFace
	{
		unsigned int w[9];
	};

	struct Tex
	{
		float w[2];
	};




	Q_OBJECT


	protected:	
	
		//OpenGL						//Einstellungen:		
		void initializeGL();			//Rendermode, Licht, Persp....

		void paintGL();					//Szene zeichnen
		void checkError();

		//QT
		void mousePressEvent(QMouseEvent*);	
		void mouseReleaseEvent(QMouseEvent*);	
		void mouseMoveEvent(QMouseEvent*);
		void paintEvent(QPaintEvent*);
		void resizeEvent(QResizeEvent*);
		void keyPressEvent( QKeyEvent* );
			
		QCursor* cursor;
		//Eigene Fkt
		void showCab();
		void setCameraState(const State&,const Vektor3f&);
		
		GLuint loadTexture(char *filename, bool mipMap=false);
		GLuint loadAlphaTexture(char *filename, bool mipMap=false);
		void OrthoMode(const int&, const int&, const int&, const int&);
		void PerspectiveMode();
//		void renderDigit(const int&, const int&, const int&, const float&, const float&, const float&);
		void drawCockpit();
		void drawRadar();//TO-DO
		void drawLensFlare(const Vektor3f& ,bool bOcc=false);
		void drawArrow(const State&);//TO-DO
		void drawGauge(const int&, const int&, const int&, const int&, int, const int& steps=15, const bool& arrow=false);
		void resetDeadCloudParticles(quadparticles[]);
		void renderCloudParticles(quadparticles[]);
		void resetDeadParticles(particles[]);
		void renderParticles(particles[]);
		void drawThruster(particles[], const Vektor3f&, Vektor3f&, const float&, bool burner=false);
		void updateThruster(particles[]);
		void targeting(State*);
		void drawMainMenu(Menu*);
		void drawNebula(SpaceObject*);
//		void drawDebris(particles[]);

		GLvoid BuildFont(GLvoid);
		GLvoid glPrint(const char *fmt, ...);
		GLuint fontBase;

		GLuint message;



	public:

		bool menuMode;

		// Konstruktor/Destruktor
		PaintWidget(QWidget * parent=0, const char * name=0, WFlags f=0);
		~PaintWidget();
		void resizeGL( int,int );		//Fenstergrösse, Perspektive....
	
		bool afterburner;
		int aftFuel;
		int shake_strength;
		Vektor3f shakePos;


		int targAquired;
		int zoomFactor; //radar

		bool cockpitVisible;
		bool hinterherflieg;
		bool show_BoundingSpheres;
		bool show_CheckSpheres;

		float alpha;
		int iClouds;
		int flashLight;
		bool bRainbow;
		bool bFog;
		bool bFlash;
		float fFogFar;
		GLfloat FogColor[4];
		float FogAlpha;
		float AmbScale;
		float AmbColor[4];
		float DiffScale;
		float DiffColor[4];

		Ship* Schiff;
		float cpRoll,cpLR,cpUD;
		
		bool NV_OCC;
		int sunPixels;

		bool jStickDet,jStick;
		JOYINFOEX stick;

		float mScalX,mScalY;
		
		inline void setFirstGate(Gate* fG) { firstGate = fG; };
		void setTarget(SpaceObject*);

		void updateJoystick();



	private:

		GLfloat	rtri;				// Angle For The Triangle ( NEW )
		GLfloat	rquad;				// Angle For The Quad ( NEW )

		Vektor3f camPos;
		Vektor3f camOri;
		Vektor3f camUp;
		Vektor3f camRealOri;
		Vektor3f camX;


		SpaceObjectList* sList;	//nötig für radar und pfeil
		SpaceObject* target;
		int gTextures[50],w,h,minutes,seconds,time;
		Gate* targetGate;	//vom softwareergonomischen Standpunkt muss das in den controller
							//faulheitstechnisch isses jetzt mal eben hier ;)
		int oldGate; //um aus Faulheit das Signal das ein Tor passed wurde zu umgehen
		float maxDist,hScal,wScal;    
		GLuint base;
		particles particle[200];
		particles exhaustParticle[100];
		quadparticles qParticle[2000];
//		particles debris[30];

		GLsizei listRange;

		bool loadMesh(const QString&,bool bObjNormals=false);

		Gate* firstGate;
		Menu* mainMenu;
		
		int bMulti;
		int iMode;
		int iTrack;

		SpaceObject* tempSpObj;


	public slots:

		void sltLoad(void);
		void sltResize(QResizeEvent*);
		void sltInput(SpaceObjectList*);
		void sltMenu(Menu*);

		void drawMessage(int texName);


};

#endif
