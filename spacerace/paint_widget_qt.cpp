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





#include"paint_widget_qt.h"
 /*#define GLH_EXT_SINGLE_FILE 1
#include "glh_extensions.h"*/

//#include <gl\aux.h>



// Konstruktor.
PaintWidget::PaintWidget(QWidget * parent, const char * name, WFlags f)
:QGLWidget(parent,name,0,f)
{
	rtri=0.0f;
	rquad=0.0f;
	
	listRange=40;	// Anzahl der Display Listen
	
	afterburner = false;
	shake_strength=0;
	shakePos = Vektor3f(0.0f,0.0f,0.0f);
	
	zoomFactor=2;
	maxDist=100.0f;
	minutes=0;seconds=0;time=10; //"Uhr"
	firstGate = NULL;
	targetGate = NULL;	//wird im drawradar upgedatet
	oldGate=1;	// ** kann raus?? **
	
	tempSpObj = NULL;
	
	cockpitVisible = true;
	hinterherflieg = false;
	show_BoundingSpheres = false;
	show_CheckSpheres = false;
	wScal=w*0.0009765625f;
	hScal=h*0.0013888888888888888888888888888889f;
	
	menuMode=false;
	bMulti=0;
	iClouds=0;
	fFogFar=200.0f;
	FogColor[0]=0.3f;
	FogColor[1]=0.45f;
	FogColor[2]=0.4f;
	FogColor[3]=1.0f;
	FogAlpha=0.1f;
	AmbColor[0]=0.2f;
	AmbColor[1]=0.2f;
	AmbColor[2]=0.2f;
	AmbColor[3]=1.0f;
	AmbScale=0.05f;
	DiffColor[0]=1.0f;
	DiffColor[1]=1.0f;
	DiffColor[2]=1.0f;
	DiffColor[3]=1.0f;
	DiffScale=0.5f;
	alpha=0;
	
	bRainbow=false;
	
	cpRoll=0.0f,cpLR=0.0f,cpUD=0.0f;
	
	message = 0;
	
	cursor=new QCursor(crossCursor);
	this->setCursor(*cursor);
	
	jStickDet=0;jStick=0;
	
	mScalX=18/(width()*0.5f) ;
	mScalY=18/(height()*0.5f) ;
	
	//test for joystick___
	JOYINFO joyinfo; 
	UINT wNumDevs, wDeviceID; 
	BOOL bDev1Attached, bDev2Attached; 
	
    if((wNumDevs = joyGetNumDevs()) == 0) 
		jStickDet=0; 
    
	bDev1Attached = wNumDevs >0 && joyGetPos(JOYSTICKID1,&joyinfo) != JOYERR_UNPLUGGED; 
    bDev2Attached = wNumDevs == 2 && joyGetPos(JOYSTICKID2,&joyinfo) != 
        JOYERR_UNPLUGGED; 
	
	//printf("dev1: %d ,dev2: %d\n",bDev1Attached, bDev2Attached);
    if(bDev1Attached || bDev2Attached)   // decide which joystick to use
	{
        wDeviceID = bDev1Attached ? JOYSTICKID1 : JOYSTICKID2; 
		jStickDet=1;
	}
    else 
        jStickDet=0; 
	//___test for joystick
	
	//bind joystick to paintwidget___
	
	if (jStickDet)
	{
		joySetCapture(this->winId(), JOYSTICKID1, NULL, FALSE);
		
    } 
	//___bind joystick
	
	stick.dwFlags=JOY_RETURNALL;
	stick.dwSize=52;
	
};



// Destruktor.
PaintWidget::~PaintWidget()
{
	delete tempSpObj;
	joyReleaseCapture(JOYSTICKID1);
};



GLvoid PaintWidget::BuildFont(GLvoid)        // Build Our Bitmap Font
{
	
	HFONT font;          // Windows Font ID
	HFONT oldfont;         // Used For Good House Keeping
	
	fontBase = glGenLists(96);        // Storage For 96 Characters
	
	
	// falls Bajoran-Schrift fehlschlägt
	if  (!(font = CreateFont( -12,       // Height Of Font
		0,        // Width Of Font
		0,        // Angle Of Escapement
		0,        // Orientation Angle
		FW_BOLD,      // Font Weight
		FALSE,       // Italic
		FALSE,       // Underline
		FALSE,       // Strikeout
		ANSI_CHARSET,     // Character Set Identifier
		OUT_TT_PRECIS,     // Output Precision
		CLIP_DEFAULT_PRECIS,   // Clipping Precision
		ANTIALIASED_QUALITY,   // Output Quality
		FF_DONTCARE|DEFAULT_PITCH,  // Family And Pitch
		"Bajoran")	// Font Name
		))
		
		// dann nimm Arial
	{
		printf("Font not Found! Trying to use Arial instead...\n");
		font = CreateFont( -12,       // Height Of Font
			0,        // Width Of Font
			0,        // Angle Of Escapement
			0,        // Orientation Angle
			FW_BOLD,      // Font Weight
			FALSE,       // Italic
			FALSE,       // Underline
			FALSE,       // Strikeout
			ANSI_CHARSET,     // Character Set Identifier
			OUT_TT_PRECIS,     // Output Precision
			CLIP_DEFAULT_PRECIS,   // Clipping Precision
			ANTIALIASED_QUALITY,   // Output Quality
			FF_DONTCARE|DEFAULT_PITCH,  // Family And Pitch
			"Arial");     // Font Name
	};
	
	oldfont = (HFONT)SelectObject(wglGetCurrentDC(), font);    
	wglUseFontBitmaps(wglGetCurrentDC(), 32, 96, fontBase);  
	SelectObject(wglGetCurrentDC(), oldfont);       
	DeleteObject(font);         // Delete The Font
	
};	// BuildFont



GLvoid PaintWidget::glPrint(const char *fmt, ...)     
{
	char  text[256];        // Holds Our String
	va_list  ap;          // Pointer To List Of Arguments
	
	if (fmt == NULL)         // If There's No Text
		return;           // Do Nothing
	
	va_start(ap, fmt);         // Parses The String For Variables
	vsprintf(text, fmt, ap);      
	va_end(ap);           // Results Are Stored In Text
	
	glPushAttrib(GL_LIST_BIT);       // Pushes The Display List Bits
	glListBase(fontBase - 32);        // Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); 
	glPopAttrib();          // Pops The Display List Bits
};	// glPrint



	/*
	void PaintWidget::renderDigit(const int& number, const int& xPos, const int& yPos, const float& r=1.0f, const float& g=1.0f, const float& b=1.0f)
	{
	//TO-DO: das Ganze mit Fonts!
	
	  float texX=0.0f,texY=0.05f,texStepX=0.16f,texStepY=0.22f;
	  texX=0.0f+number*texStepX;
	  if (number>5)
	  {
	  texX=0.0f+(number-6)*texStepX;
	  texY=texStepY+0.019;	
	  }
	  glEnable(GL_TEXTURE_2D);
	  glEnable (GL_BLEND);
	  glDisable(GL_DEPTH_TEST);	
	  
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, gTextures[t_Numbers]);	//zifferntabelle
		glBegin(GL_QUADS);
		glColor4f(r, g, b, 0.8f); // grau
		glTexCoord2f(texX, texY);	glVertex2f(xPos, yPos);
		glTexCoord2f(texX, texY+texStepY);	glVertex2f(xPos,yPos+16);
		glTexCoord2f(texX+texStepX, texY+texStepY);	glVertex2f(xPos+16,yPos+16);
		glTexCoord2f(texX+texStepX, texY);	glVertex2f(xPos+16, yPos);
		glEnd();
		
		  glDisable(GL_BLEND);
		  glDisable(GL_TEXTURE_2D);
		  
			};	// renderDigit
*/


void PaintWidget::renderCloudParticles(quadparticles particle[])
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDepthMask(false);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.001f);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_fog]);
	
	for (int i=0; i<iClouds; i++)
	{
		Vektor3f dist = particle[i].pos - camPos;
		float distSqrLength = dist.sqrLength();
		dist.normalize();
		
		float di = dist.dot(camRealOri);
		
		// falls außerhalb des Sichtfeldes, deaktivieren/reseten
		if( di <= 0.7f) 
		{ particle[i].active = false;	};
		
		// falls nahe am Schiff, langsam ausblenden
		if ( (distSqrLength <= 100.0f) && (particle[i].life > 0.025f) ) //noch an speed anpassen
		{ particle[i].life -= FogAlpha*0.2f; };
		
		
		// alle aktiven Wolkenpartikel durchgehen
		if (particle[i].active)
		{
			
			if (bRainbow)
			{
				glColor4f(fabs(dist.x),fabs(dist.y),fabs(dist.z),particle[i].life);
			}
			else
			{
				glColor4f(FogColor[0],FogColor[1],FogColor[2],particle[i].life);
			};
			
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex3f(particle[i].lo.x, particle[i].lo.y, particle[i].lo.z);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(particle[i].ro.x, particle[i].ro.y, particle[i].ro.z);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(particle[i].ru.x, particle[i].ru.y, particle[i].ru.z);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(particle[i].lu.x, particle[i].lu.y, particle[i].lu.z);
			glEnd(); 
			
		};	// if particle[i]
		
	}	// for iClouds
	
	
	glDepthMask(true);
	glPopAttrib();
	
};	// renderCloudParticles



void PaintWidget::drawNebula(SpaceObject* nebula)
{
	glPushMatrix();
	glLoadIdentity();
	
	gluLookAt(camPos.x+shakePos.x, camPos.y+shakePos.y, camPos.z+shakePos.z,
		camOri.x, camOri.y , camOri.z,  
		      camUp.x , camUp.y , camUp.z);
	
	Vektor3f v=camRealOri;
	Vektor3f w=camUp;
	v=w.kreuz(v);
	v.normalize();
	//v,w spannen die Bildschirmebene auf
	
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_fog]);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LESS,1.0f);
	glAlphaFunc(GL_GREATER,0.003f);
	int step=1;
	
	for (int i=0; i<200;i+=step)
	{
		Vektor3f pos;
		pos=nebula->getPartPos(i)+nebula->getPos();
		//pos.show();
		Vektor3f q;
		q=pos;
		pos=pos-camPos;
		float dist=pos.length();
		pos.normalize();
		
		if(camRealOri.dot(pos) >=0.7)
		{
			float opac=0.6f;
			if (dist<=10.0f)
			{
				opac=0.06f*dist;
				
			}
			
			
			float f,s;
			Vektor3f lo,lu,ro,ru;
			/*	if (bRainbow)
			{*/
			Vektor3f r=q;
			r.normalize();
			glColor4f(fabs(r.x),fabs(r.y),fabs(r.z),opac);
			/*	}
			else
			glColor4f(0.0f,0.0f,1.0f,opac);*/
			
			f=-10.0f;
			s=10.0f;
			lo=(v*f)+(w*s)+q;
			f=-10.0f;
			s=-10.0f;
			lu=(v*f)+(w*s)+q;
			f=10.0f;
			s=10.0f;
			ro=(v*f)+(w*s)+q;
			f=10.0f;
			s=-10.0f;
			ru=(v*f)+(w*s)+q;
			
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);glVertex3f(lo.x, lo.y, lo.z);
			glTexCoord2f(1.0f,0.0f);glVertex3f(ro.x, ro.y, ro.z);
			glTexCoord2f(1.0f,1.0f);glVertex3f(ru.x, ru.y, ru.z);
			glTexCoord2f(0.0f,1.0f);glVertex3f(lu.x, lu.y, lu.z);
			glEnd(); 
			
			
			
		}
		
	}
	
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDepthMask(true);
	glPopMatrix();
	
};



void PaintWidget::drawThruster(particles exhaustParticle[], const Vektor3f& center, Vektor3f& target,const float& scale, bool burner)
{
	float r,s,t;
	Vektor3f q,ur,ul,ll,lr; 
	Vektor3f v=Vektor3f(0.0f,-1.0f,0.0f);//zeigt nach links
	Vektor3f w=Vektor3f(1.0f,0.0f,0.0f);//zeigt nach oben(alles etwas verdreht hier)
	
	glEnable (GL_BLEND);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.5f);
	
	target=target-center;//richtung
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	for (int i=0;i<100;i++)
	{
		
		if (exhaustParticle[i].active)
		{r=exhaustParticle[i].pos.x;
		s=exhaustParticle[i].pos.y;
		t=exhaustParticle[i].pos.z;
		//float scal=exhaustParticle[i].life*0.4f;
		glColor4f(exhaustParticle[i].life,0.93f*exhaustParticle[i].life,0.0f,exhaustParticle[i].life*2.0f);
		/*	if (particle[i].life<=0.1f) 
		glColor4f(0.0f,0.0f,exhaustParticle[i].life,exhaustParticle[i].life*2.0f);*/
		
		//if (t<0.3 && r>-0.0125 && r<0.0125)
		//if(burner) glColor4f(0.0f,0.0f,1.0f,exhaustParticle[i].life*2.0f);
		//else 
		//glColor4f(1.0f,1.0f,0.0f,exhaustParticle[i].life);
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
		if (burner)
			glPointSize(exhaustParticle[i].life*13.0f*scale);
		else
			glPointSize(exhaustParticle[i].life*10.0f*scale);
		q=center+(target*t);//gerade durch das Center in Richtung Ziel
		
		ur=v*r+w*s+q;//(rechte obere ecke)
					 /*r-=scal;
					 ul=v*s+w*r+q;//links oben
					 s-=scal;
					 ll=v*s+w*r+q;//links unten
					 r+=scal;
					 lr=v*s+w*r+q;//rechts unten
					 
					   glBegin(GL_QUADS);
					   glTexCoord2f(0.0f, 0.0f);glVertex3f(ul.x ,ul.y ,ul.z );
					   glTexCoord2f(1.0f, 0.0f);glVertex3f(ur.x ,ur.y , ur.z);
					   glTexCoord2f(1.0f, 1.0f);glVertex3f(lr.x ,lr.y ,lr.z );
					   glTexCoord2f(0.0f, 1.0f);glVertex3f( ll.x,ll.y , ll.z );
		glEnd();*/
		
		glBegin(GL_POINTS);
		glVertex3f(ur.x,ur.y,ur.z);
		glEnd();
		}
		
		glPointSize(1.0f);
		glDisable(GL_POINT_SMOOTH);
		
	}
	
	
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	
}



void PaintWidget::updateThruster(particles exhaustParticle[])
{
	for (int i=0;i<100;i++)
	{
		if (!exhaustParticle[i].active && ((rand()%33)==0))
		{
			exhaustParticle[i].active=true;
			exhaustParticle[i].life=1.0f;
			exhaustParticle[i].fade=0.03f;
			exhaustParticle[i].pos.x=-0.025f+rand()%100*0.0005f;//NICHT der x-Wert.hält als ersatz für r hin
			exhaustParticle[i].pos.y=-0.025f+rand()%100*0.0005f;//-"-y-"-
			exhaustParticle[i].pos.z=0;//-"-z-"-
		}
		else if (exhaustParticle[i].active)
		{
			exhaustParticle[i].life-=exhaustParticle[i].fade;
			exhaustParticle[i].pos.z=1-exhaustParticle[i].life;
			if (exhaustParticle[i].life <0.01)
				exhaustParticle[i].active=false;
			
		}
	}
}



void PaintWidget::resetDeadCloudParticles(quadparticles particle[])
{
	
	// erspart uns in jedem for-Durchlauf 8 Multiplikationen
	Vektor3f o1 = camX  *  10.0f;
	Vektor3f o2 = camX  * -10.0f;
	Vektor3f u1 = camUp *  10.0f;
	Vektor3f u2 = camUp * -10.0f;
	
	Vektor3f q;		// nicht in jedem Schleifendurchlauf neu anlegen
	
	int zuf1;
	
	// alle Wolkenpartikel durchgehen
	for (int i=0; i<iClouds; i++)
	{
		
		if (particle[i].active)		// wenn Partikel aktiv:
		{
			if (particle[i].life <= FogAlpha)			// einblenden??
				particle[i].life += FogAlpha*0.1f;
		}
		else	// sonst:
		{
			particle[i].life = 0.0f;	// wieder anschalten
			particle[i].active = true;
			
			zuf1 = rand();
			float f =  50.0f - 0.02f*(zuf1%5000);
			float s = -50.0f + 0.02f*(rand()%5000);
			q = (camX*f) + (camUp*s) + camPos;			// q zufälliger Punkt auf der Ebene durch camPos
			q = q + camRealOri * (10.0f+zuf1%160);	// Gerade entlang des Normalenvektors: d.h. q liegt weiter vor dem Schiff!
			
			particle[i].lo = o1 + u1 + q;	// Partikelecken setzen
			particle[i].lu = o1 + u2 + q;
			particle[i].ro = o2 + u1 + q;
			particle[i].ru = o2 + u2 + q;
			
			particle[i].pos = q;	// neue Partikelposition
			
		};	// if else
		
	};	// for iClouds
	
};	// resetDeadCloudParticles



void PaintWidget::renderParticles(particles particle[])
{
	glEnable(GL_FOG);
	GLfloat color[4]={0.0f,0.0f,0.0f,1.0f};
	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_START, 40.0f);
	glFogf(GL_FOG_END, 60.0f);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	
	
	for (int i=0; i<200;i++)
	{
		if (particle[i].active)
		{
			float x = particle[i].pos.x;
			float y = particle[i].pos.y;
			float z = particle[i].pos.z;
			glPointSize(1.5f);
			glEnable(GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE);//_MINUS_SRC_ALPHA);
			glBegin(GL_POINTS);
			glColor4f(0.6f,0.6f,0.6f,particle[i].life);
			glVertex3f(x,y,z); 
			glEnd(); 
			glDisable(GL_BLEND);
			glPointSize(1.0f);
		}
	}
	glDisable(GL_FOG);
	
};




void PaintWidget::resetDeadParticles(particles particle[])
{
	
	Vektor3f tmpV;
	
	// alle Partikel durchgehen
	for (int i=0; i<200; i++)
	{
		int zuf1;
		
		if (particle[i].active)		// falls Partikel aktiv:
		{
			particle[i].life = particle[i].life - particle[i].fade;	// Partikel faden
			
			if (particle[i].life <= 0.1f)	// falls schwach genug, deaktivieren
				particle[i].active = false;
		}
		
		else if (!particle[i].active && ((zuf1=rand())%3))	// sonst, falls zufall ok:
		{
			int zuf2 = rand();
			particle[i].life = 1.0f;	// Partikel neu aktivieren
			particle[i].active = true;
			particle[i].fade = 0.0002f*(zuf1%100) + 0.002f;
			
			float f = 30.0f - 0.01f*(zuf1%6000);
			float s = -30.0f + 0.01f*(zuf2%6000);
			tmpV = (camX*f) + (camUp*s) + camPos;		// q zufälliger Punkt auf der Ebene durch camPos
			tmpV = tmpV + camRealOri * (50.0f+zuf2%10);	// Gerade entlang des Normalenvektors: q liegt weiter vor dem Schiff!
			
			particle[i].pos = tmpV;		// neue Partikelposition
		};	// if else
		
	};	// for
	
};	// resetDeadParticles



void PaintWidget::initializeGL() //Rendermode, Licht, Persp....
{
	
	
	BuildFont();
	
	
	for (int i=0;i<200;i++)
	{
		particle[i].active=false;
		particle[i].life=1.0f;
		
	}
	for (i=0;i<1000;i++)
	{
		qParticle[i].active=false;
		qParticle[i].life=1.0f;
		
	}
	for (i=0;i<100; i++)
		exhaustParticle[i].active=false;
	
	//	for (i=0;i<30;i++)
	//		debris[i].active=false;
	
	showCab(); //Zeige Hardware-Daten
	
	
	
	//glEnable(GL_NORMALIZE); //durch glScale werden die Normalen ungültig -> Korrektur hier
	
	
	//	gTextures[t_Cockpit]=loadAlphaTexture("texturen/cockpit3.bmp");
	gTextures[t_Stars]=loadTexture("texturen/stars.bmp");
	gTextures[t_Numbers]=loadAlphaTexture("texturen/numbers.bmp");
	gTextures[t_Metal]=loadTexture("texturen/gate.bmp", true);
	gTextures[t_Earth]=loadAlphaTexture("texturen/earth.bmp");
	gTextures[t_Nebula]=loadAlphaTexture("texturen/nebula.bmp");
	gTextures[t_Sun]=loadAlphaTexture("texturen/sun.bmp");
	gTextures[t_Rock1]=loadTexture("texturen/asteroid1.bmp", true);
	gTextures[t_Rock2]=loadTexture("texturen/asteroid2.bmp", true);
	gTextures[t_Rock3]=loadTexture("texturen/asteroid3.bmp", true);
	gTextures[t_Flare]=loadAlphaTexture("texturen/flare1.bmp");
	gTextures[t_Halo]=loadAlphaTexture("texturen/halo2.bmp");
	gTextures[t_Ship]=loadTexture("texturen/racertop.bmp", true);
	gTextures[t_Missile]=loadTexture("texturen/missile.bmp", true);
	gTextures[t_MenuBase]=loadAlphaTexture("texturen/mainmenu.bmp");
	gTextures[t_fog]=loadAlphaTexture("texturen/fog.bmp", true);
	gTextures[t_PolyCockpit]=loadTexture("texturen/polycockpit.bmp", true);
	gTextures[t_Pane]=loadAlphaTexture("texturen/env.bmp");
	gTextures[t_Title]=loadAlphaTexture("texturen/spacerace.bmp",true);
	
	gTextures[t_YouWin]=loadAlphaTexture("texturen/youwin.bmp",true);
	gTextures[t_YouLose]=loadAlphaTexture("texturen/youlose.bmp",true);
	
	
	
    glEnable( GL_DEPTH_TEST ); // Z-Buffer einschalten
	glShadeModel( GL_SMOOTH ); //Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Hintergrundfarbe
	
	glCullFace (GL_BACK); //Backface-Culling ein
    glEnable (GL_CULL_FACE);
	
    base = glGenLists (listRange);
	
	glLoadIdentity();
	
	
	// globale Helligkeit
	GLfloat globalAmb[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
	
	
	// Standart Material-Einstellungen
	GLfloat spec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diff[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat amb[] =  { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat shin[] = { 70.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_SHININESS, shin);
	
	
	
	// das Schiffsmodell
	glNewList(base+o_Ship, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Ship]);
	
	loadMesh("Models/racerfinal.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// das Other-Schiffsmodell
	glNewList(base+o_OtherShip, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Ship]);
	
	loadMesh("Models/racerfinal.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Tor 1
	glNewList(base+o_Gate1, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Metal]);
	
	loadMesh("Models/tracktex2.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Tor 2
	glNewList(base+o_Gate2, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Rock1]);
	
	loadMesh("Models/gate2-big.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Asteroid 1
	glNewList(base+o_Asteroid1, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Rock1]);
	
	loadMesh("Models/asteroid1.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Asteroid 2
	glNewList(base+o_Asteroid2, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Rock2]);
	
	loadMesh("Models/asteroid2.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Asteroid 3
	glNewList(base+o_Asteroid3, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Rock3]);
	
	loadMesh("Models/asteroid3.obj");
	
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Missile
	glNewList(base+o_Missile,GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,gTextures[t_Missile]);
	glPushMatrix();
	glScalef(0.5f,0.5f,0.5f);
	loadMesh("Models/rocket2.obj");
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	
	
	// Kugel
	{
		glNewList(base+o_Kugel, GL_COMPILE);
		//	glPushMatrix();
		//	glPushAttrib(GL_CURRENT_BIT);	
		glBegin(GL_TRIANGLE_STRIP);
		
		float grid=15.0f;
		
		double pi = 3.1415926535897932384626433832795028841971693993751058209;
		double schritt = pi/grid;
		
		double lambda=0;
		double my=0;
		
		float x = sin(lambda)*sin(my);
		float y = sin(lambda)*cos(my);
		float z = cos(lambda);
		
		glNormal3f( x, y, z );
		glVertex3f( x, y, z );
		
		for (int j=0; j<2*grid; j++)
		{
			
			for (int i=0; i<(grid); i++)
			{
				lambda=lambda+schritt;
				
				x = sin(lambda)*sin(my);
				y = sin(lambda)*cos(my);
				z = cos(lambda);
				//glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // rot
				glNormal3f( x, y, z );
				glVertex3f( x, y, z );
				
				my=my+schritt;
				x = sin(lambda)*sin(my);
				y = sin(lambda)*cos(my);
				z = cos(lambda);
				//glColor4f(0.0f, 1.0f, 0.0f, 1.0f); // grün
				glNormal3f( x, y, z );
				glVertex3f( x, y, z );
				
				my=my-schritt;
			}
			
			my=my+schritt;
		}
		
		glEnd();
		//	glPopAttrib();
		//	glPopMatrix();
		glEndList();	// Kugel
	}
	
	
	
	// Cube
	glNewList(base+o_Cube, GL_COMPILE);
	
	loadMesh("Models/box.smf");
	
	glEndList();
	
	
	
	glLoadIdentity();
	
	
	
	//die SkyBox
	glNewList(base+o_Skybox, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Stars]);	// rundrum: Stars
	// Center the Skybox around the given x,y,z position
	float width=4000.0,height=4000.0,length=4000.0;
	float x = camPos.x - width  / 2;
	float y = camPos.y - height / 2;
	float z = camPos.z - length / 2;
	float x2 = x + width;
	float y2 = y + height;
	float z2 = z + length;
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
	
	//glBindTexture(GL_TEXTURE_2D, SkyboxTexture[SKYFRONT]);	// evtl besser als planeten drüberblenden?
	glBegin(GL_QUADS);	// Front side
	glTexCoord2f(4.5f, 0.0f); glVertex3f(x,		y,		z2);
	glTexCoord2f(4.5f, 4.5f); glVertex3f(x,		y2,		z2);
	glTexCoord2f(0.0f, 4.5f); glVertex3f(x2,	y2,		z2); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,	y,		z2);
	glEnd();
	glBegin(GL_QUADS);	// Back side
	glTexCoord2f(4.5f, 0.0f); glVertex3f(x2,	 y,		z);
	glTexCoord2f(4.5f, 4.5f); glVertex3f(x2,	 y2,	z); 
	glTexCoord2f(0.0f, 4.5f); glVertex3f(x,		 y2,	z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		 y,		z);
	glEnd();
	glBegin(GL_QUADS);	// Left side
	glTexCoord2f(4.5f, 4.5f); glVertex3f(x,		  y2,	z);	
	glTexCoord2f(0.0f, 4.5f); glVertex3f(x,		  y2,	z2); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,	z2);
	glTexCoord2f(4.5f, 0.0f); glVertex3f(x,		  y,	z);		
	glEnd();
	glBegin(GL_QUADS);	// Right side
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,	y,		z);
	glTexCoord2f(4.5f, 0.0f); glVertex3f(x2,	y,		z2);
	glTexCoord2f(4.5f, 4.5f); glVertex3f(x2,	y2,		z2); 
	glTexCoord2f(0.0f, 4.5f); glVertex3f(x2,	y2,		z);
	glEnd();
	glBegin(GL_QUADS);	// Up side
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x2,	y2,		z);
	glTexCoord2f(4.5f, 0.0f); glVertex3f(x2,	y2,		z2); 
	glTexCoord2f(4.5f, 4.5f); glVertex3f(x,		y2,		z2);
	glTexCoord2f(0.0f, 4.5f); glVertex3f(x,		y2,		z);
	glEnd();
	glBegin(GL_QUADS);	// Down side
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		y,		z);
	glTexCoord2f(4.5f, 0.0f); glVertex3f(x,		y,		z2);
	glTexCoord2f(4.5f, 4.5f); glVertex3f(x2,	y,		z2); 
	glTexCoord2f(0.0f, 4.5f); glVertex3f(x2,	y,		z);
	glEnd();
	
	
	
	// Etwas Overloaded, aber is ja nur zu Demo-Zwecken... ;)
	// Und viel zu aufwändig, besser vermutlich sechs einzelne SkyBox Texturen für 
	// die jeweilige komplette Seite. Wenn man überhaupt so viel Hintergrund will... ;)
	
	
	// SkyBox Objekte, z.B. Planeten etc*/
	glEnable(GL_ALPHA_TEST);	// Alphawerte nur bis zu einem bestimmten Wert
	glAlphaFunc(GL_GREATER,0.3f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Earth]);	// down: earth
 			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width*0.3f,		y+2.0f,		z+length*0.3f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width*0.3f,		y+2.0f,		z+length*0.7f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width*0.7f,		y+2.0f,		z+length*0.7f); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width*0.7f,		y+2.0f,		z+length*0.3f);
			glEnd();
			glDisable(GL_ALPHA_TEST);
			
			glEnable(GL_BLEND);		// Alpha-Blending anschalten
			
			glBindTexture(GL_TEXTURE_2D, gTextures[t_Nebula]);	// right: nebula
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x+2.0f,		y+height*0.7f,		z+length*0.3f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+2.0f,		y+height*0.7f,		z+length*0.7f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+2.0f,		y+height*0.3f,		z+length*0.7f); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x+2.0f,		y+height*0.3f,		z+length*0.3f);
			glEnd();
			
			glBindTexture(GL_TEXTURE_2D, gTextures[t_Sun]);	// back: sun
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width*0.54f,		y+height*0.46f,		z+2.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width*0.54f,		y+height*0.54f,		z+2.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width*0.46f,		y+height*0.54f,		z+2.0f); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width*0.46f,		y+height*0.46f,		z+2.0f);
			glEnd();
			
			
			glDisable(GL_BLEND);
			
			glDisable(GL_TEXTURE_2D);
			glEndList();	// SkyBox
			
			
			glLoadIdentity();
			
			
			
			// Flare
			glNewList(base+o_Flare,GL_COMPILE);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);glVertex2f(-128,-128);
			glTexCoord2f(0.0f,1.0f);glVertex2f(-128,128);
			glTexCoord2f(1.0f,1.0f);glVertex2f(128,128);
			glTexCoord2f(1.0f,0.0f);glVertex2f(128,-128);
			glEnd();
			glEndList();
			
			
			// Laser
			/*	glNewList(base+o_Laser,GL_COMPILE);
			glPushMatrix();
			glTranslatef(-0.2f,-0.3f,0.0f);
			glCallList(base+d_LaserBox);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(-0.2f,0.3f,0.0f);
			glCallList(base+d_LaserBox);
			glPopMatrix();
			glEndList();
			
			  
				// Laser-Box
				glNewList(base+d_LaserBox,GL_COMPILE);
				glDisable(GL_LIGHTING);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE,GL_ONE);
				glColor4f(1.0f,0.45f,0.45f,0.3f);
				glPushMatrix();
				glScalef(0.02f,0.02f,1.5f);
				glTranslatef(-0.5f,-0.5f,-0.5f);
				glBegin(GL_QUADS);
				glVertex3f( 0.0f,  0.0f,  0.0f);
				glVertex3f( 1.0f,  0.0f,  0.0f);
				glVertex3f( 1.0f,  0.0f,  1.0f);
				glVertex3f( 0.0f,  0.0f,  1.0f);
				glVertex3f( 0.0f,  0.0f,  1.0f);
				glVertex3f( 1.0f,  0.0f,  1.0f);
				glVertex3f( 1.0f,  1.0f,  1.0f);
				glVertex3f( 0.0f,  1.0f,  1.0f);
				glVertex3f( 0.0f,  1.0f,  1.0f);
				glVertex3f( 1.0f,  1.0f,  1.0f);
				glVertex3f( 1.0f,  1.0f,  0.0f);
				glVertex3f( 0.0f,  1.0f,  0.0f);
				glVertex3f( 0.0f,  1.0f,  0.0f);
				glVertex3f( 1.0f,  1.0f,  0.0f);
				glVertex3f( 1.0f,  0.0f,  0.0f);
				glVertex3f( 0.0f,  0.0f,  0.0f);
				glVertex3f( 1.0f,  0.0f,  1.0f);
				glVertex3f( 1.0f,  0.0f,  0.0f);
				glVertex3f( 1.0f,  1.0f,  0.0f);
				glVertex3f( 1.0f,  1.0f,  1.0f);
				glVertex3f( 0.0f,  0.0f,  0.0f);
				glVertex3f( 0.0f,  0.0f,  1.0f);
				glVertex3f( 0.0f,  1.0f,  1.0f);
				glVertex3f( 0.0f,  1.0f,  0.0f);
				glEnd();
				glPopMatrix();
				glDisable(GL_BLEND);
				glEnable(GL_LIGHTING);
				glEndList();
			*/
			
			
			// 3D-Cockpit
			glNewList(base+o_Cockpit,GL_COMPILE);
			glPushMatrix();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_TEXTURE_2D);
			
			// Scheibe
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			//		glEnable(GL_COLOR_MATERIAL);
			//		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// braucht man??
			//		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			//		glEnable(GL_TEXTURE_GEN_S);
			//		glEnable(GL_TEXTURE_GEN_T);
			glBindTexture(GL_TEXTURE_2D,gTextures[t_Pane]);
			glColor4f(1.0f,1.0f,1.0f,1.0f);	
			glTranslatef(0.0f,-1.2f,-4.4f);
			glScalef(1.3f,1.05f,1.2f);
			loadMesh("Models/pane.obj",true);
			//		glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_BLEND);
			
			// Streben
			glBindTexture(GL_TEXTURE_2D,gTextures[t_PolyCockpit]);
			loadMesh("Models/cockpit3.obj",true);
			//		glDisable(GL_TEXTURE_GEN_S);
			//		glDisable(GL_TEXTURE_GEN_T);
			
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
			
			// ** gleich GL_DEPTH_TEST??
			
			//fürs Lensflare in den tiefenbuffer
			glColorMask(0,0,0,0);
			loadMesh("Models/cockpit3.obj",true);
			glColorMask(1,1,1,1);
			
			glPopAttrib();
			glPopMatrix();
			glEndList();
			
			glLoadIdentity();
			
}		




void PaintWidget::resizeGL (int width, int height )   //Fenstergrösse, Perspektive...
{
	//--------------------------------------------------------------
	glViewport(0,0,width,height);						// Reset The Current Viewport
	
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,4000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
	//--------------------------------------------------------------
	
	
}



void PaintWidget::keyPressEvent( QKeyEvent *e )
{
	
}



void PaintWidget::checkError()
{
	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) 
	{
		errString = gluErrorString(errCode);
		printf("OpenGL Error: %s\n", errString);
	}
}



void PaintWidget::drawArrow(const State& target)
{
	//TO-DO: schönerer Pfeil
	
	float alpha=0.0f,r;
	Vektor3f n=camRealOri;
	n.normalize();
	Vektor3f pos=target.pos;
	Vektor3f cam=camPos;
	r=((-pos.x+cam.x)*n.x+(-pos.y+cam.y)*n.y+(-pos.z+cam.z)*n.z)/(n.x*n.x+n.y*n.y+n.z*n.z);
	Vektor3f result=(n*r)+pos-cam;
	result.normalize();
	Vektor3f up=camUp;
	up.normalize();
	r=((-pos.x+cam.x)*up.x+(-pos.y+cam.y)*up.y+(-pos.z+cam.z)*up.z)/(up.x*up.x+up.y*up.y+up.z*up.z);
    alpha=acos(up.dot(result))*(57.295779513082320876798154814105);
	result=(up*r)+pos-cam;
	result.normalize();
	n=n.kreuz(up);
	float beta=acos(n.dot(result))*(57.295779513082320876798154814105);
	//printf("cam.x>res.x:%d cam.y>res.y: %d cam.z>res.z: %d \n" ,(int) (cam.x>result.x), (int)(cam.y>result.y),(int) (cam.z>result.z) );
	//printf("alpha: %4.4f beta: %4.4f \n",alpha, beta);
	if (beta>90.0) alpha=-alpha;
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(w*0.5f,h*0.5f,0.0);
	glRotatef(alpha,0.0f,0.0f,1.0f);
	glTranslatef(-w*0.5f,-h*0.5f,0.0);
	glBegin(GL_TRIANGLES);
	glColor4f(0.0f,1.0f,0.0f,0.6f);
	glVertex2f(w*0.4975f,h*0.48f);
	glVertex2f(w*0.5025f,h*0.48f);
	glVertex2f(w*0.5f,h*0.46f);
	glEnd();
	glPopMatrix();
	
};


/*
void PaintWidget::drawDebris(particles debris[])
{
Vektor3f pos=camPos;
Vektor3f randVec;

  for (int i=0;i<30; i++)
  {
  
	if ( rand()%3 && !debris[i].active)
	{
	randVec.x=-30.0f+rand()%300*0.2f;
	randVec.y=-30.0f+rand()%300*0.2f;
	randVec.z=-30.0f+rand()%300*0.2f;
	randVec.normalize();
	debris[i].moveVec=randVec*-0.01f*(rand()%5);
	debris[i].moveVec.x+=0.05f*(rand()%3)+0.01f;
	debris[i].moveVec.y+=0.05f*(rand()%3)+0.01f;
	debris[i].moveVec.y+=0.05f*(rand()%3)+0.01f;
	randVec=randVec*22.0f;
	debris[i].active=true;
	debris[i].pos=pos+randVec;
	debris[i].life=0.05f;
	debris[i].fade=0.0f;
	
	  }
	  else
	  {
	  if (debris[i].life<=0.0f)
	  {
	  debris[i].active=false;
	  continue;
	  
		}
		
		  float sqrdist=(debris[i].pos-pos).sqrLength() ;
		  if (sqrdist>3000.0f)
		  debris[i].life-=0.01f;
		  else if (debris[i].life<=0.99f && sqrdist>4.0f)
		  debris[i].life+=0.01f;
		  
			debris[i].fade+=5.0f+rand()%5;
			if (sqrdist<4.0f)
			debris[i].moveVec=debris[i].pos-camPos;
			debris[i].moveVec.normalize();
			debris[i].moveVec=debris[i].moveVec*0.03f;
			
			  debris[i].pos=debris[i].pos+debris[i].moveVec;
			  glPushMatrix();
			  glEnable(GL_LIGHTING);
			  glEnable(GL_BLEND);
			  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			  glColor4f(0.2f,0.2f,0.2f,debris[i].life);
			  glTranslatef(debris[i].pos.x,debris[i].pos.y,debris[i].pos.z);
			  glScalef(0.02f,0.02f,0.02f);
			  glRotatef(debris[i].fade,1.0f,1.0f,0.0f);
			  glCallList(base+o_Asteroid1);
			  glDisable(GL_LIGHTING);
			  glDisable(GL_BLEND);
			  glPopMatrix();
			  
				}
				}
				
				  }
				  */
				  
				  void PaintWidget::drawCockpit()
				  {
					  glPushAttrib(GL_ALL_ATTRIB_BITS);
					  OrthoMode(0,0,w,h);//andere Projektionsmatrix->2D
					  
					  tempSpObj = sList->getSpObj(0);	// Schiff zwischenspeichern
					  
					  glEnable (GL_BLEND);
					  glDisable(GL_DEPTH_TEST);	
					  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
					  
					  /*
					  // altes flaches Cockpit
					  glEnable(GL_TEXTURE_2D);
					  glEnable (GL_BLEND);
					  glDisable(GL_DEPTH_TEST);	
					  
						glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
						glBindTexture(GL_TEXTURE_2D, gTextures[t_Cockpit]);	//cockpit
						glBegin(GL_QUADS);
						glColor4f(1.0f, 1.0f, 0.9f, 0.8f);
						glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, 0);
						glTexCoord2f(0.0f, 1.0f);	glVertex2f(0,h);
						glTexCoord2f(1.0f, 1.0f);	glVertex2f(w,h);
						glTexCoord2f(1.0f, 0.0f);	glVertex2f(w, 0);
						glEnd();
						glDisable(GL_BLEND);
					  */
					  
					  
					  // Fadenkreuz
					  glLineWidth(2.0f*hScal);
					  glBegin(GL_LINES);
					  glColor4f(0.0f, 1.0f, 0.0f, 0.7f);
					  glVertex2f(0.485f*w, 0.5f*h);
					  glVertex2f(0.515f*w, 0.5f*h);
					  glVertex2f(0.5f*w, 0.483f*h);
					  glVertex2f(0.5f*w, 0.517f*h);
					  glEnd();
					  
					  
					  // speed:
					  int speed_set = tempSpObj->getSpeedSet();
					  int speed_real = tempSpObj->getSpeedReal();
					  if (speed_real<0) speed_real=0;
					  
					  glColor4f(0.9f,0.9f,0.9f,0.8f);//farbe
					  glRasterPos2f(0.028f*w,0.93f*h);  //position
					  glPrint(" set:   %04d", speed_set);
					  glRasterPos2f(0.028f*w,0.953f*h);  //position
					  glPrint("real:   %04d", speed_real);
					  
					  
					  // speed
					  drawGauge(speed_set, speed_real, 1500, 0.0195f*w, 0.85f*h, 35, true);
					  //TO-DO: getSpeedMax für Ship (virtual für spaceobject)
					  
					  
					  // time:
					  if (time<0) time=0;
					  minutes = time/60;
					  seconds = time%60;
					  
					  glColor4f(0.9f,0.9f,0.9f,0.8f);//farbe
					  glRasterPos2f(0.93f*w,0.94f*h);  //position
					  glPrint("%02d : %02d", minutes, seconds);
					  
					  
					  // fuel
					  aftFuel = tempSpObj->getFuel();
					  drawGauge(1000, aftFuel, 1000, 0.975f*w, 0.85f*h, 35);//afterburner
					  
					  
					  tempSpObj = NULL;	// Schiff wird nichtmehr gebraucht
					  
					  PerspectiveMode();	//Projektionsmatrix wieder 3D
					  glPopAttrib();	// alle Blend, Z-Buffer etc zurücksetzen
				  }
				  
				  
				  
				  void PaintWidget::drawGauge(const int& setValue, const int& value, const int& maximum, const int& xPos, int yPos, const int& steps, const bool& arrow)
				  {
					  
					  //muss im Orthomode aufgerufen werden!	
					  
					  
					  int delta = maximum/steps;	// Achtung: steps sollten in maximum aufgehen??
					  float invDelta = 1.0f/delta;
					  
					  
					  // an Position von set gegebenenfalls das Dreieck zeichnen
					  if (arrow)
					  {
						  float d = -setValue*5.0f*invDelta;	// Position des Pfeils auf die Länge des Gauge gerechnet
						  glBegin(GL_TRIANGLES);
						  if(afterburner)		// bei Nachbrenner den Pfeil einfärben
							  glColor4f(0.9f,0.35f,0.3f,0.8f);
						  else
							  glColor4f(0.5f,0.5f,0.7f,0.8f);
						  glVertex2f(xPos-10, d+yPos-2);
						  glVertex2f(xPos-10, d+yPos+4);
						  glVertex2f(xPos-2,  d+yPos+1);
						  glEnd();
					  };
					  
					  
					  float a=1.0f;	// Farbwert des obersten Quads
					  int vi;		// "Abstand" Wert-i
					  
					  
					  // alle Step-Positionen durchgehen
					  for (int i=0; i<=maximum; i+=delta)
					  {
						  vi = value - i;
						  
						  if (0 < vi)	// falls unterhalb des aktuellen Wertes:
						  {
							  if (vi<delta)	// oberstes helles Quad schattieren
								  a = 0.5f + (vi*0.5f*invDelta);
							  
							  glBegin(GL_QUADS);	// helles Quad zeichnen
							  if (arrow)		// bei Nachbrenner-Gauge andere Quad-Farbe
								  glColor4f(a,a,0.0f,0.8f);
							  else
								  glColor4f(a,0.2f,0.1f,0.8f);
							  glVertex2f(xPos,   yPos-2);
							  glVertex2f(xPos,   yPos);
							  glVertex2f(xPos+8, yPos);
							  glVertex2f(xPos+8, yPos-2);
							  glEnd();
						  }
						  else	// sonst:
						  {
							  glBegin(GL_QUADS);	// dunkles Quad zeichnen
							  if (arrow)		// bei Nachbrenner-Gauge andere Quad-Farbe
								  glColor4f(0.5f,0.5f,0.1f,0.7f);
							  else
								  glColor4f(0.5f,0.2f,0.2f,0.7f);
							  glVertex2f(xPos,   yPos-2);
							  glVertex2f(xPos,   yPos);
							  glVertex2f(xPos+8, yPos);
							  glVertex2f(xPos+8, yPos-2);
							  glEnd();
						  };
						  
						  yPos-=5;	// yPos des nächsten Quads
						  
					  };	// for
					  
					  
				  }	//drawGauge
				  
				  
				  
				  void PaintWidget::drawRadar()
				  {
					  //Großes TO-DO: das Ding irgendwie passend skalieren
					  
					  glEnable (GL_BLEND);
					  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					  glPushMatrix();
					  glLoadIdentity();
					  glEnable(GL_STENCIL_TEST);
					  OrthoMode(0,0,w,h);
					  if(!menuMode)
					  {
						  glClear(GL_STENCIL_BUFFER_BIT);
						  glStencilFunc(GL_ALWAYS,1,0xFF);
						  glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
						  glEnable(GL_BLEND);
						  glDisable(GL_DEPTH_TEST);
						  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
						  glBegin(GL_POLYGON);
						  glColor4f(0.1f,0.1f,0.5f,0.3f);
						  glVertex2f( wScal*624,hScal*625);
						  glVertex2f( wScal*564,hScal*550);
						  glVertex2f( wScal*460,hScal*550);
						  glVertex2f( wScal*400,hScal*625);
						  glVertex2f( wScal*460,hScal*700);
						  glVertex2f( wScal*564,hScal*700);
						  glEnd();
						  
						  glEnable(GL_DEPTH_TEST);
						  glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
						  glStencilFunc(GL_EQUAL,1,0xFF);
					  }
					  PerspectiveMode();
					  if (menuMode)
						  glTranslatef(0.335f,0.02f, 0.0f);
					  
					  glTranslatef(0.0,-0.25f,-0.8f);
					  gluLookAt(camPos.x, camPos.y , camPos.z,
						  camOri.x, camOri.y , camOri.z,  
						  camUp.x , camUp.y , camUp.z);
					  glTranslatef(camPos.x, camPos.y, camPos.z);
					  if (menuMode)
					  {
						  glRotatef(alpha*0.5f,0.0f,1.0f,0.0f);
						  
					  }
					  float scal=0.0005f;
					  
					  
					  
					  Gate* iGate = firstGate;
					  
					  while (!(iGate==NULL))
					  {
						  
						  State st = iGate->getState();
						  
						  if ( iGate->CheckPointAllowed )	{ targetGate=iGate;};
						  
						  Vektor3f p=st.pos;
						  
						  Vektor3f q;
						  p=Vektor3f(-(camPos.x-p.x),-(camPos.y-p.y),-(camPos.z-p.z));
						  if (p.length()>maxDist) maxDist=p.length();
						  scal=zoomFactor*0.0225f/maxDist;
						  if (menuMode)
							  scal=0.1f/maxDist;
						  q=Vektor3f(-(camPos.x-p.x)*scal,-(camPos.y-p.y)*scal,-(camPos.z-p.z)*scal);
						  if(q.length()<0.085f || !menuMode)
						  {
							  
							  if ( iGate->CheckPointPassed )
							  {
								  glPointSize(2.5f);
								  glColor4f(0.0f,0.0f,1.0f,0.6f);
							  }
							  else if ( iGate==targetGate )
							  {
								  if (!menuMode)
								  {
									  glPointSize(3.5f);
									  glColor4f(1.0f,0.0f,0.0f,0.7f);
								  }
							  }
							  else 
							  {
								  glPointSize(2.0f);
								  glColor4f(0.0f,1.0f,1.0f,0.6f);
							  }
							  
							  glBegin(GL_POINTS);
							  glVertex3f(q.x,q.y,q.z);
							  glEnd();
						  }
						  
						  //}
						  iGate = iGate->getSuccGate();
					  };	// while
					  
					  
					  
					  
					  if (!menuMode)
					  {
						  
						  OrthoMode(0,0,w,h);
						  
						  //TO-DO:der controller sollte noch das momentan aktive Gate mitgeteilt bekommen, damit man es sich hier holen kann ohne
						  //die liste durchzuwandern(oder geht das schon und ich bin nur zu blöd ;)?
						  
						  // Theoretisch muß man doch oben die ganzen Gates durchgehen, um jedem die richtige Farbe zuzuweisen??
						  // Eigentlich sogar die ganze viewList??
						  
						  
						  State s = target->getState();
						  glDisable(GL_STENCIL_TEST);
						  //drawArrow(s);
						  PerspectiveMode();
						  
						  
						  
						  glBegin(GL_POINTS);
						  glColor4f(0.0f,1.0f,0.0f,1.0f);
						  glVertex3f(0.0f,0.0f,0.0f);
						  glEnd();
						  glPointSize(1.0f);
					  }
					  glPopMatrix();
					  glDisable (GL_BLEND);
					  glEnable(GL_DEPTH_TEST);
					  
					  
					  //---------------------------------radar
					  
}



void PaintWidget::drawLensFlare(const Vektor3f& sun, bool bOcc)
{
	
	GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    GLdouble winX, winY, winZ;  /*  returned world x, y, z coords  */
	glGetIntegerv (GL_VIEWPORT, viewport);
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	gluProject(camPos.x+sun.x,camPos.y+sun.y,camPos.z+sun.z, 
		mvmatrix, 
		projmatrix, 
		viewport, 
		&winX, 
		&winY, 
		&winZ);
	int y=winY; int x=winX; 
	y= viewport[3] - y - 1;
	//printf("%d %d %4.4f\n" ,x, y ,winZ);
	float occScal=1.0f;
	
	/*if (NV_OCC && bOcc)
	{	
	printf("pc: %d\n",sunPixels);
	//if (sunPixels<5000*wScal) return;
	
	  
		occScal=0.5f+(float)(sunPixels)/(5000*wScal*hScal);
		
		  
			
}*/
	
	
	GLfloat pixelZ;
	if (x<1 || y<1 || x>w-1 ||y>h-1 ) return;
	glReadPixels(winX,winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&pixelZ);
	//printf("bufferz: %4.4f z: %4.4f  \n" , pixelZ, winZ); 
	if (winZ<0.0f || winZ>1.0f ||pixelZ<winZ && (!NV_OCC || !bOcc)) return;
	else
		if(winZ<0.0f || winZ>1.0f ||pixelZ<winZ && NV_OCC)
			/*if (sunPixels<1200*hScal*wScal)*/ return;
			
			
			
			glDisable(GL_DEPTH_TEST);
		
		float wScal=w*0.0009765625f,hScal=h*0.0013888888888888888888888888888889f;
		//Anpassung an die Fenstergröße, hier lässt sich auch die Flare skalieren
		
		Vektor3f c=Vektor3f(w*0.5,h*0.5,0);
		OrthoMode(0,0,w,h);
		glEnable(GL_BLEND);
		glBlendFunc (GL_ONE, GL_ONE);
		glColor4f(occScal,occScal,occScal,0.4f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,gTextures[t_Flare]);
		glPushMatrix();
		glTranslatef(x,y,0);
		glScalef(wScal,hScal,1.0f);
		glCallList(base+o_Flare);
		glPopMatrix();
		x=x-c.x, y=y-c.y;
		glBindTexture(GL_TEXTURE_2D,gTextures[t_Halo]);
		//halo1
		glColor4f(occScal,occScal,0.7f*occScal,0.00001f);
		glPushMatrix();
		glTranslatef(x*0.8+c.x,y*0.8+c.y,0);
		glScalef(wScal*0.1f,hScal*0.1f,1.0f);
		glCallList(base+o_Flare);
		glPopMatrix();
		//halo2
		glColor4f(0.7f*occScal,0.7f*occScal,occScal,0.00001f);
		glPushMatrix();
		glTranslatef(-x+c.x,-y+c.y,0);
		glScalef(wScal*0.4f,hScal*0.4f,1.0f);
		glCallList(base+o_Flare);
		glPopMatrix();
		//halo3
		glColor4f(0.7f*occScal,occScal,occScal,0.00001f);
		glPushMatrix();
		glTranslatef(-x*0.5+c.x,-y*0.5+c.y,0);
		glScalef(wScal*0.15f,hScal*0.15f,1.0f);
		glCallList(base+o_Flare);
		glPopMatrix();
		//halo4
		glColor4f(occScal,0.7f*occScal,occScal,0.00001f);
		glPushMatrix();
		glTranslatef(-x*0.833+c.x,-y*0.833+c.y,0);
		glScalef(wScal*0.3f,hScal*0.3f,1.0f);
		glCallList(base+o_Flare);
		glPopMatrix();
		//halo5
		glColor4f(occScal,occScal,0.7f *occScal,0.00001f);
		glPushMatrix();
		glTranslatef(-x*0.3125+c.x,-y*0.3215+c.y,0);
		glScalef(wScal*0.2f,hScal*0.2f,1.0f);
		glCallList(base+o_Flare);
		glPopMatrix();
		
		glDisable(GL_BLEND);
		PerspectiveMode();
		//glEnable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		
}




void PaintWidget::drawMainMenu(Menu* menu)
{
	
	//menuPoint:
	//0:Singleplayer/Multiplayer
	//1:Race//Deathmatch
	//2:Select Track 
	//3:Exit
	//4:Start Game
	//eingestellte Werte updaten
	bMulti=menu->getSetting(0);
	iMode=menu->getSetting(1);
	iTrack=menu->getSetting(2);
	
	
	//textzeile einstellen
	int zeile;
	int point=menu->getMenuPoint();
	
	if (point==0)
	{
		if(bMulti)zeile=2;
		else zeile=1;
	}
	else if (point==1)
		zeile=!iMode+4;
	else if (point==2)
		zeile=3;
	else if (point==3)
		zeile=7;
	else zeile=6;
	
	
	
	OrthoMode(0,0,w,h);
	glLoadIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	alpha+=0.5;
	
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(false);
	glEnable(GL_STENCIL_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS,0,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	
	//background
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,h);
	glVertex2f(w,h);
	glVertex2f(w,0);
	glEnd();
	
	//rahmen
	glColor4f(0.5f,0.5f,0.5f,1.0f);
	glDepthMask(false);
	glBegin(GL_QUADS);
	glVertex2f(145*wScal,120*hScal);
	glVertex2f(145*wScal,630*hScal);
	glVertex2f(905*wScal,630*hScal);
	glVertex2f(905*wScal,120*hScal);
	glEnd();
	
	
	
	glBegin(GL_QUADS); //oben links
	glVertex2f(70*wScal,70*hScal);
	glVertex2f(70*wScal,230*hScal);
	glVertex2f(280*wScal,230*hScal);
	glVertex2f(280*wScal,70*hScal);
	glEnd();
	
	glBegin(GL_QUADS);//oben rechts
	glVertex2f(770*wScal,70*hScal);
	glVertex2f(770*wScal,230*hScal);
	glVertex2f(980*wScal,230*hScal);
	glVertex2f(980*wScal,70*hScal);
	glEnd();
	
	glBegin(GL_QUADS);//unten rechts
	glVertex2f(770*wScal,520*hScal);
	glVertex2f(770*wScal,705*hScal);
	glVertex2f(980*wScal,705*hScal);
	glVertex2f(980*wScal,520*hScal);
	glEnd();
	
	glBegin(GL_QUADS);//unten links 
	glVertex2f(70*wScal,520*hScal);
	glVertex2f(70*wScal,705*hScal);
	glVertex2f(280*wScal,705*hScal);
	glVertex2f(280*wScal,520*hScal);
	glEnd();
	glDepthMask(false);
	//_________--rahmen
	
	//Hauptframe4___________________________
	glColorMask(0,0,0,0);
	glStencilFunc(GL_ALWAYS,0,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	
	
	glBegin(GL_QUADS);
	glVertex2f(150*wScal,125*hScal);
	glVertex2f(150*wScal,625*hScal);
	glVertex2f(900*wScal,625*hScal);
	glVertex2f(900*wScal,125*hScal);
	glEnd();
	
	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	
	
	glBegin(GL_QUADS); //oben links
	glVertex2f(70*wScal,70*hScal);
	glVertex2f(70*wScal,230*hScal);
	glVertex2f(280*wScal,230*hScal);
	glVertex2f(280*wScal,70*hScal);
	glEnd();
	
	glBegin(GL_QUADS);//oben rechts
	glVertex2f(770*wScal,70*hScal);
	glVertex2f(770*wScal,230*hScal);
	glVertex2f(980*wScal,230*hScal);
	glVertex2f(980*wScal,70*hScal);
	glEnd();
	
	glBegin(GL_QUADS);//unten rechts
	glVertex2f(770*wScal,520*hScal);
	glVertex2f(770*wScal,705*hScal);
	glVertex2f(980*wScal,705*hScal);
	glVertex2f(980*wScal,520*hScal);
	glEnd();
	
	glBegin(GL_QUADS);//unten links 
	glVertex2f(70*wScal,520*hScal);
	glVertex2f(70*wScal,705*hScal);
	glVertex2f(280*wScal,705*hScal);
	glVertex2f(280*wScal,520*hScal);
	glEnd();
	
	glDepthMask(true);
	glStencilFunc(GL_EQUAL,1,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glColorMask(1,1,1,1);
	PerspectiveMode();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glLoadIdentity();
	
	static GLfloat pos[4] = {0.0f, 0.0f, -800.0f, 0.0f };
	GLfloat ambColor[] = {0.15f, 0.25f, 0.25f, 1.0f};
	GLfloat diffColor[] = {0.9f, 0.9f, 0.9f, 1.0f};
	
	GLfloat ambColor2[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat diffColor2[] = {0.9f, 0.2f, 0.2f, 1.0f};
	glEnable(GL_LIGHT0);
	glTranslatef (0.0,0.0,-30);
	//glRotatef (alpha, 1.0, 0.0, 0.0);
	glRotatef (alpha*0.5f, 0.0, 1.0, 0.0);
	glLightfv( GL_LIGHT0, GL_POSITION, pos );
	/*gluLookAt(camState.pos.x, camState.pos.y, camState.pos.z,
	camState.ori.x, camState.ori.y , camState.ori.z,  
	camState.up.x , camState.up.y , camState.up.z);*/
	
	glDisable(GL_LIGHTING);
	GLfloat color[4]={FogColor[0],FogColor[1],FogColor[2],1.0f};
	glFogfv(GL_FOG_COLOR, color);
	glFogf(GL_FOG_START, 30.0f);
	glFogf(GL_FOG_END, 200.0f);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	
	if (bFog)
		glEnable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glCallList(base+o_Skybox);	// zeichnen
	glEnable(GL_DEPTH_TEST);	
	glEnable(GL_LIGHTING);
	
	glCallList(base+o_Szene);	// Szene darstellen
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	drawLensFlare(Vektor3f(0.0f,0.0f,-800.0f));	
	
	
	
	glPopMatrix();
	glPopAttrib();
	OrthoMode(0,0,w,h);
	
	glStencilFunc(GL_EQUAL,1,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glDisable(GL_DEPTH_TEST);
	glColor4f(0.0f,0.0f,0.5f,0.0f);
	
	if (menu->getMenuPoint() ==4)
		glColor4f(0.0f,0.5f,0.5f,0.5f);
	
	
	glBegin(GL_QUADS);
	glVertex2f(150*wScal,125*hScal);
	glVertex2f(150*wScal,625*hScal);
	glVertex2f(900*wScal,625*hScal);
	glVertex2f(900*wScal,125*hScal);
	glEnd();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	
	
	//______________HauptFrame	4
	
	//Text_____
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glStencilFunc(GL_ALWAYS,0,0xFF);
	
	float row=(zeile-1)*0.1375+0.04;
	glColor4f(1.0f,1.0f,1.0f,0.5f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_MenuBase]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,row);glVertex2f(400*wScal,50*hScal);
	glTexCoord2f(0.0f,row+0.1375);glVertex2f(400*wScal,125*hScal);
	glTexCoord2f(1.0f,row+0.1375);glVertex2f(700*wScal,125*hScal);
	glTexCoord2f(1.0f,row);glVertex2f(700*wScal,50*hScal);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	//_____Text
	
	//frame links oben 0_________
	glColor4f(0.0f,0.0f,0.5f,1.0f);
	glColorMask(0,0,0,0);
	glDepthMask(false);
	glStencilFunc(GL_ALWAYS,0,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	glBegin(GL_QUADS); //oben links
	glVertex2f(75*wScal,75*hScal);
	glVertex2f(75*wScal,225*hScal);
	glVertex2f(275*wScal,225*hScal);
	glVertex2f(275*wScal,75*hScal);
	glEnd();
	glStencilFunc(GL_NOTEQUAL,0,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
	glColorMask(1,1,1,1);
	glDepthMask(1);
	PerspectiveMode();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glLoadIdentity();
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, diffColor);
	glTranslatef(-11.5,7.5f,0.0f);
	if (bMulti)
		glTranslatef (-7,5, -50);
	else
		glTranslatef (0.0,0.0,-30);
	glRotatef (alpha, 1.0, 0.0, 0.0);
	glRotatef (alpha*0.5f, 0.0, 1.0, 0.0);
	glLightfv( GL_LIGHT0, GL_POSITION, pos );
	glDisable(GL_DEPTH_TEST);
	glCallList(base+o_Skybox);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
				updateThruster(exhaustParticle);
				if (bMulti)
					glTranslatef(0.0f,-3.25f,0.0f);
				
				glCallList(base+o_Ship);
				/*drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-2.7f),0.5f);
				drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-2.7f),0.5f);*/
				if (bMulti)
				{
					glTranslatef(0.0f,6.5f,0.0f);
					glCallList(base+o_Ship);
					
					/*drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-2.7f),0.5f);
					drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-2.7f),0.5f);*/
				}
				glDisable(GL_LIGHTING);
				drawLensFlare(Vektor3f(0.0f,0.0f,-800.0f));	
				
				
				glPopMatrix();
				glPopAttrib();
				OrthoMode(0,0,w,h);
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				glDepthMask(1);
				glDisable(GL_DEPTH_TEST);
				glColor4f(0.0f,0.0f,0.5f,0.0f);
				
				if (menu->getMenuPoint() ==0)
					glColor4f(0.0f,0.5f,0.5f,0.5f);
				
				glBegin(GL_QUADS); //oben links
				glVertex2f(75*wScal,75*hScal);
				glVertex2f(75*wScal,225*hScal);
				glVertex2f(275*wScal,225*hScal);
				glVertex2f(275*wScal,75*hScal);
				glEnd();
				
				glColor4f(0.0f,0.0f,0.5f,1.0f);
				
				//___________________frame links oben 0
				
				//frame unten rechts____________-
				glColorMask(0,0,0,0);
				glDepthMask(false);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
				
				
				glBegin(GL_QUADS);//unten rechts
				glVertex2f(775*wScal,525*hScal);
				glVertex2f(775*wScal,700*hScal);
				glVertex2f(975*wScal,700*hScal);
				glVertex2f(975*wScal,525*hScal);
				glEnd();
				glDepthMask(true);
				glStencilFunc(GL_NOTEQUAL,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				glColorMask(1,1,1,1);
				PerspectiveMode();
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				glLoadIdentity();
				
				glEnable(GL_LIGHT0);
				glTranslatef (0.0,0.0,-30);
				glRotatef(alpha*0.5f,0.0f,1.0f,0.0f);
				glLightfv( GL_LIGHT0, GL_POSITION, pos );
				glDisable(GL_DEPTH_TEST);
				glCallList(base+o_Skybox);
				glEnable(GL_DEPTH_TEST);
				drawRadar();
				glPopMatrix();
				glPopAttrib();
				OrthoMode(0,0,w,h);
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				
				//glColorMask(1,1,1,1);
				glDisable(GL_DEPTH_TEST);
				glColor4f(0.0f,0.0f,0.5f,0.0f);
				
				if (menu->getMenuPoint() ==2)
					glColor4f(0.0f,0.5f,0.5f,0.5f);
				
				
				glBegin(GL_QUADS);//unten rechts
				glVertex2f(775*wScal,525*hScal);
				glVertex2f(775*wScal,700*hScal);
				glVertex2f(975*wScal,700*hScal);
				glVertex2f(975*wScal,525*hScal);
				glEnd();
				glColor4f(0.0f,0.0f,0.5f,1.0f);
				
				
				
				
				
				//____________-frame unten rechts
				//frame unten links__________________--
				glColorMask(0,0,0,0);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
				
				
				glBegin(GL_QUADS);//unten links stenciln
				glVertex2f(75*wScal,525*hScal);
				glVertex2f(75*wScal,700*hScal);
				glVertex2f(275*wScal,700*hScal);
				glVertex2f(275*wScal,525*hScal);
				glEnd();
				glDepthMask(true);
				glStencilFunc(GL_NOTEQUAL,0,0xFF); //ins gestencilte rendern
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				glColorMask(1,1,1,1);
				PerspectiveMode();
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glPushMatrix();
				glLoadIdentity();
				
				glEnable(GL_LIGHT0);
				glTranslatef (0.0,0.0,-30);
				glRotatef(alpha*0.5f,0.0f,1.0f,0.0f);
				glLightfv( GL_LIGHT0, GL_POSITION, pos );
				glDisable(GL_DEPTH_TEST);
				glCallList(base+o_Skybox);
				glEnable(GL_DEPTH_TEST);
				drawRadar();
				
				glPopMatrix();
				glPopAttrib();
				OrthoMode(0,0,w,h);
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				
				//glColorMask(1,1,1,1);
				glDisable(GL_DEPTH_TEST);
				glColor4f(0.0f,0.0f,0.5f,0.0f);
				if (menu->getMenuPoint() ==3)
					glColor4f(0.0f,0.5f,0.5f,0.4f);
				
				glBegin(GL_QUADS);//unten links
				glVertex2f(75*wScal,525*hScal);
				glVertex2f(75*wScal,700*hScal);
				glVertex2f(275*wScal,700*hScal);
				glVertex2f(275*wScal,525*hScal);
				glEnd();
				
				
				glColor4f(0.0f,0.0f,0.5f,1.0f);
				//______________-unten links
				//oben rechts______________
				glColorMask(0,0,0,0);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
				
				
				glBegin(GL_QUADS);//oben rechts
				glVertex2f(775*wScal,75*hScal);
				glVertex2f(775*wScal,225*hScal);
				glVertex2f(975*wScal,225*hScal);
				glVertex2f(975*wScal,75*hScal);
				glEnd();
				glDepthMask(true);
				glStencilFunc(GL_NOTEQUAL,0,0xFF); //ins gestencilte rendern
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				glColorMask(1,1,1,1);
				PerspectiveMode();
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				{glPushMatrix();
				glLoadIdentity();
				
				
				glTranslatef(11.5,7.5f,0.0f);
				if(!iMode)
					glTranslatef (10,5,-50);
				else
					glTranslatef(0.0f,0.0f,-30);
				glRotatef(alpha*0.5f,0.0f,1.0f,0.0f);
				
				//glLightfv( GL_LIGHT0, GL_POSITION, pos );
				glDisable(GL_DEPTH_TEST);
				glCallList(base+o_Skybox);
				glEnable(GL_DEPTH_TEST);
				static GLfloat pos[4] = {0.0f, 0.0f, -800.0f, 0.0f };
				GLfloat ambColor[] = {0.075f, 0.1f, 0.1f, 1.0f};
				GLfloat diffColor[] = {0.3f, 0.3f, 0.45f, 1.0f};
				glLightfv(GL_LIGHT0, GL_AMBIENT, ambColor);
				glLightfv(GL_LIGHT0, GL_DIFFUSE, diffColor);
				glLightfv(GL_LIGHT0, GL_SPECULAR, diffColor);
				glLightfv( GL_LIGHT0, GL_POSITION, pos );
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				if(!iMode)
				{
					glScalef(0.15f,0.15f,0.15f);
					glRotatef(90.0f,0.0f,0.0f,1.0f);
					
					glCallList(base+o_Gate1);
				}
				else
				{
					glScalef(2.0f,2.0f,2.0f);
					
					glCallList(base+o_Missile);
					
					drawThruster(exhaustParticle,Vektor3f(0.0f,0.0f,0.75f),Vektor3f(0.0f,0.0f,2.0f),0.5f);
				}
				glDisable(GL_LIGHTING);
				
				glPopMatrix();
				}
				glPopAttrib();
				OrthoMode(0,0,w,h);
				glClear(GL_STENCIL_BUFFER_BIT);
				glStencilFunc(GL_ALWAYS,0,0xFF);
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
				
				//glColorMask(1,1,1,1);
				glDisable(GL_DEPTH_TEST);
				glColor4f(0.0f,0.0f,0.5f,0.0f);
				glDepthMask(1);
				
				
				
				if (menu->getMenuPoint() ==1)
					glColor4f(0.0f,0.5f,0.5f,0.5f);
				
				glBegin(GL_QUADS);//oben rechts
				glVertex2f(775*wScal,75*hScal);
				glVertex2f(775*wScal,225*hScal);
				glVertex2f(975*wScal,225*hScal);
				glVertex2f(975*wScal,75*hScal);
				glEnd();
				
				glColor4f(0.0f,0.0f,0.5f,1.0f);
				
				glDisable(GL_STENCIL_TEST);
				
				
				//MP Chages=======================================
				
				
				glColor4f(0.0f,0.8f,0.3f,0.7f);//farbe
				glRasterPos2f(160*wScal,210*hScal);  //position
				if(bMulti==1) glPrint("Server"); 
				if(bMulti==2) glPrint("Client"); 
				
				
				//	if (menu->getMenuPoint()==5)
				{
					if(menu->getMenuPoint()==5)  //SERVER
					{
						glRasterPos2f(420*wScal,160*hScal);  //position
						glPrint("Server started");
						glRasterPos2f(350*wScal,190*hScal);
						glPrint("WAITING FOR OTHER PLAYERS....");
						
						if(menu->client2)
						{
							glRasterPos2f(400*wScal,300*hScal);  //position
							glPrint("Client connected...");
						}
						if(menu->client3)
						{
							glRasterPos2f(400*wScal,320*hScal);  //position
							glPrint("Client connected...");
						}
						if(menu->client4)
						{
							glRasterPos2f(400*wScal,340*hScal);  //position
							glPrint("Client connected...");
						}
						if((menu->client4)||(menu->client3) ||(menu->client2))
						{
							glRasterPos2f(400*wScal,380*hScal);  //position
							glPrint("- PRESS RETURN TO START -");
						}
						
					}
					
					if(menu->getMenuPoint()==6)  //CLIENT
					{
						if (!menu->connected)
						{
							glRasterPos2f(400*wScal,310*hScal);  
							glPrint("Client Menu");
							glRasterPos2f(400*wScal,340*hScal);
							glPrint("PLEASE ENTER SERVER IP:");
							glRasterPos2f(400*wScal,380*hScal);
							glPrint("%s",(const char*)menu->host);
							
							glRasterPos2f(400*wScal,420*hScal);
							if(!(menu->nameState))  glPrint("Player Nr: %s",(const char*)menu->player);
							
							if((menu->player).length())
							{
								glRasterPos2f(400*wScal,440*hScal);
								glPrint("PRESS RETURN TO CONNECT");
							}
						}
						else
						{
							glRasterPos2f(400*wScal,310*hScal);  
							glPrint("CONNECTED !");
							glRasterPos2f(200*wScal,340*hScal);
							glPrint("WAITING FOR OTHER PLAYERS READY SIGNAL");
						}
					}
					
				}
				
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, gTextures[t_Title]);
				
				glColor4f(1.0f,1.0f,1.0f,1.0f);//farbe
				
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.0f);glVertex2f(312*wScal,408*hScal);
				glTexCoord2f(0.0f,1.0f);glVertex2f(312*wScal,808*hScal);
				glTexCoord2f(1.0f,1.0f);glVertex2f(712*wScal,808*hScal);
				glTexCoord2f(1.0f,0.0f);glVertex2f(712*wScal,408*hScal);
				glEnd();
				
				glDisable(GL_TEXTURE_2D);
				
				
				glDisable(GL_BLEND);
				//======================================
				
				
				PerspectiveMode();
				
};



void PaintWidget::drawMessage(int texName)
{
	message=texName;
}



void PaintWidget::paintGL()
{
	
	if (menuMode)
	{	glClearColor(0,0,0,0);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);	// Clear Screen And Depth Buffer
	
	glLoadIdentity();
	drawMainMenu(mainMenu);
	
	}
	
	
	else	// if not Menu
	{
		
		
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();	// Reset The Current Modelview Matrix
		
		
		// Nachbrenner-Schütteln
		if ((!afterburner || aftFuel<=0) && (shake_strength>0))	// tab nicht gedrückt und shake noch vorhanden
		{
			shake_strength=shake_strength-25;
			if (shake_strength<0)
			{
				shake_strength=0;
			};
		}
		else if ((afterburner && aftFuel>0) && (shake_strength<1000))	// tab gedrückt und shake noch nicht voll da
		{
			shake_strength=shake_strength+30;
		};
		
		if (shake_strength)		// falls schütteln da, d.h. (sh_st>0)
		{
			int zuf1 = (rand()%100) - 50;	// -50 => bereich von -50 bis +50
			int zuf2 = (rand()%100) - 50;
			int zuf3 = (rand()%100) - 50;
			float tmpShake = ((float)shake_strength)*0.00000006f;
			shakePos.x = ((float)zuf1)*tmpShake;	// EyePunkt der Kamera in gluLookAt() etwas
			shakePos.y = ((float)zuf2)*tmpShake;	// verschiebe-verschütteln
			shakePos.z = ((float)zuf3)*tmpShake;
		}
		else
			shakePos = Vektor3f(0.0f,0.0f,0.0f);
		
		
		// Cockpit-Ansicht
		gluLookAt(camPos.x+shakePos.x, camPos.y+shakePos.y, camPos.z+shakePos.z,
			camOri.x, camOri.y , camOri.z,  
			camUp.x , camUp.y , camUp.z);
		
		
		// alte Partikel löschen
		resetDeadParticles(particle);
		resetDeadCloudParticles(qParticle);
		
		
		
		//GLfloat color[4]={0.9f,0.5f,0.25f,1.0f};//0,0,3,1
		/*
		if (flashLight)
		{
		GLfloat FogColor2[4]={FogColor[0]+0.1f,FogColor[1]+0.2f,FogColor[3]};
		glFogfv(GL_FOG_COLOR, FogColor2);
		glFogf(GL_FOG_START, 30.0f);
		glFogf(GL_FOG_END, fFogFar);
		glFogi(GL_FOG_MODE,GL_LINEAR);
		}
		else
		{*/
		
		
		// Nebel
		if (bFog)
		{
			glFogfv(GL_FOG_COLOR, FogColor);
			glFogf(GL_FOG_START, 30.0f);
			glFogf(GL_FOG_END, fFogFar);
			glFogi(GL_FOG_MODE,GL_LINEAR);
			glEnable(GL_FOG);
		};
		
		
		
		glDisable( GL_LIGHTING );
		
		
		// SkyBox
		glPushMatrix();
		glDisable(GL_DEPTH_TEST);	// da als allererstes und immer hinten, kein Z-Buffer
		glTranslatef(camPos.x, camPos.y, camPos.z);	// mitbewegen
		glCallList(base+o_Skybox);	// zeichnen
		glEnable(GL_DEPTH_TEST);	
		glPopMatrix();
		
		
		
		//	glDisable( GL_LIGHT2 );
		/*if((rand()%500==1) && bFlash ||flashLight)
		{
		if (!flashLight)
		{
		flashLight=10+rand()%10;
		
		  }
		  static GLfloat pos2[4] = {0.1f*(rand()%1000), 0.1f*(rand()%1000), 0.1f*(rand()%1000), 1.0f };
		  glLightfv( GL_LIGHT2, GL_POSITION, pos2 );
		  glLightfv(GL_LIGHT2, GL_AMBIENT, ambColor2);
		  glLightfv(GL_LIGHT2, GL_DIFFUSE, diffColor2);
		  glLightfv(GL_LIGHT2, GL_SPECULAR, diffColor2);
		  
			//	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
			//	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
			//	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
			glEnable( GL_LIGHT2 );
			flashLight--;
	}*/
		//	 glDisable( GL_LIGHT1 );
		
		
		static GLfloat pos[4] = {0.0f, 0.0f, -800.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, AmbColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffColor);
		glLightfv(GL_LIGHT0, GL_SPECULAR, DiffColor);
		glLightfv( GL_LIGHT0, GL_POSITION, pos );
		
		
		glEnable( GL_LIGHTING );
		glEnable( GL_LIGHT0 );
		
		// Szene darstellen
		glCallList(base+o_Szene);
		
		//	drawDebris(debris);
		
		
		glDisable(GL_FOG);
		glDisable( GL_LIGHTING );
		
		updateThruster(exhaustParticle);
		
		renderParticles(particle);
		renderCloudParticles(qParticle);
		
		
		glEnable(GL_LIGHTING);
		
		if (cockpitVisible && !hinterherflieg)
		{
			cpRoll=-(Schiff->cwccw)*0.2f ;
			cpUD= (Schiff->updown)*0.18f -3.5f+shakePos.y*20;
			cpLR= -(Schiff->leftright)*0.2f+shakePos.z*20;
			glPushMatrix();
			glDisable(GL_DEPTH_TEST);
			glLoadIdentity();
			glRotatef(cpRoll,0.0f,0.0f,1.0f);
			glRotatef(cpUD,1.0f,0.0f,0.0f);
			glRotatef(cpLR,0.0f,1.0f,0.0f);
			glCallList(base+o_Cockpit);
			glEnable(GL_DEPTH_TEST);
			glPopMatrix();
		}
		
		glDisable(GL_LIGHTING);
		//printf("cw: %d, ud: %d, lr: %d\n", Schiff->cwccw,Schiff->updown,Schiff->leftright);
		
		
		targeting(&target->getState());
		
		drawLensFlare(Vektor3f(0.0f,0.0f,-800.0f));	
		
		drawCockpit();
		
		drawRadar();
		
		
		// Messages anzeigen
		if (message)
		{
			
			OrthoMode(0,0,w,h);//andere Projektionsmatrix->2D
			glEnable(GL_TEXTURE_2D);		
			glEnable (GL_BLEND);
			glDisable(GL_DEPTH_TEST);	
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);		
			
			glColor4f(1.0f,1.0f,1.0f,0.8f);
			
			glBindTexture(GL_TEXTURE_2D, gTextures[message]);
			
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);glVertex2f(256*wScal,118*hScal);
			glTexCoord2f(0.0f,1.0f);glVertex2f(256*wScal,630*hScal);
			glTexCoord2f(1.0f,1.0f);glVertex2f(768*wScal,630*hScal);
			glTexCoord2f(1.0f,0.0f);glVertex2f(768*wScal,118*hScal);
			glEnd();
			
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
			PerspectiveMode();	//Projektionsmatrix wieder 3D
		}
		
		
}	// else menu

checkError();

}



void PaintWidget::OrthoMode(const int& left, const int& top, const int& right, const int& bottom)
{
	glMatrixMode(GL_PROJECTION);						
	glPushMatrix();									
	glLoadIdentity();								
	glOrtho( left, right, bottom, top, 0, 1 );	
	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();										
}



void PaintWidget::PerspectiveMode()	
{
	glMatrixMode( GL_PROJECTION );							
	glPopMatrix();											
	glMatrixMode( GL_MODELVIEW );							
}



void PaintWidget::paintEvent(QPaintEvent* event)
{
	//	updateGL();
};



void PaintWidget::resizeEvent(QResizeEvent* event)
{
	resize(event->size());
	resizeGL(width(),height());
	w=width();h=height();
	wScal=w*0.0009765625f;
	hScal=h*0.0013888888888888888888888888888889f;
	mScalX=18/(width()*0.5f) ;
	mScalY=18/(height()*0.5f) ;
	
};



void PaintWidget::sltResize(QResizeEvent* event)
{
	resizeEvent(event);
}


void PaintWidget::setTarget(SpaceObject* targ)
{
	target=targ;
}


void PaintWidget::targeting(State* targ)
{
	
	
	Vektor3f tar;
	tar=targ->pos;
	
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLdouble winX1, winY1, winZ1;  /*  returned world x, y, z coords  */
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
	gluProject(tar.x,tar.y,tar.z, 
		mvmatrix, 
		projmatrix, 
		viewport, 
		&winX1, 
		&winY1, 
		&winZ1);
	int y=winY1; int x=winX1; 
	y= viewport[3] - y - 1;
	
	
	OrthoMode(0,0,w,h);
	
	
	if ( (camRealOri.dot(tar-camPos)>0) && (x>1 && x<w-1 && y>1 && y<h-1 &&winZ1>=0 && winZ1<=1) )
	{
		//float scale=200*(targ->getBoundRad())/(tar-camState.pos).length();
		//printf("%4.4f \n",scale);
		glEnable(GL_BLEND);
		
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glLineWidth(3.0f*hScal);
		glColor4f(0.0f,1.0f,0.0f,0.5f);
		//		glBegin(GL_LINES);
		
		glBegin(GL_LINE_STRIP);
		glVertex2f(x+50*wScal,y+30*hScal);
		glVertex2f(x+60*wScal,y);
		glVertex2f(x+60*wScal,y);
		glVertex2f(x+50*wScal,y-30*hScal);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex2f(x-50*wScal,y+30*hScal);
		glVertex2f(x-60*wScal,y);
		glVertex2f(x-60*wScal,y);
		glVertex2f(x-50*wScal,y-30*hScal);
		glEnd();
		
		//		glLineWidth(1.0f);
		
		glRasterPos2f(x-45*wScal,y+30*hScal);  //position
		glPushMatrix();
		glColor4f(0.0f,1.0f,0.0f,0.6f);//farbe
		glPrint("Target-%4.1f", (tar-camPos).length()); 
		glPopMatrix();
	}
	else
	{
		drawArrow(*targ);
	};
	
	PerspectiveMode();
	
	glPopAttrib();
	glPopMatrix();
	
}



//Welche Hardware rendert?
void PaintWidget::showCab(void)
{
	const unsigned char* ven;
	ven=glGetString(GL_VENDOR);
	printf("GL Accelerator: %s\n",ven);
	
	ven=glGetString(GL_RENDERER);
	printf("GL Renderer: %s\n",ven);
	
	ven=glGetString(GL_VERSION);
	printf("GL Version: %s\n",ven);
	
	//	ven=glGetString(GL_EXTENSIONS);
	//	printf("%s",ven);
}



void PaintWidget::sltLoad()
{
}



void PaintWidget::sltInput(SpaceObjectList* sol)
{
	sList=sol;
	
	float FogFarPlane = 0;
	
	if (bFog) { FogFarPlane = fFogFar*fFogFar*2+2500; }
	
	SpaceObject* cam = sol->getSpObj(0);
	setCameraState(cam->getState(), cam->myX);
	//camState.show();
	
	time=(sol->getTime());
	// viele Objekte, Koordinaten aus StateList
	
	glNewList(base+o_Szene, GL_COMPILE);
	float*	matrix = new float[16];
	
	int i=1;
	if (hinterherflieg) { i = 0; };		// bei Außenkamera Schiff auch zeichnen
	
	
	
	for (i; i < (sol->getCount()); i++)		// alle Objekte durchgehen
	{
		
		// State holen
		State st = (sol->getSpObj(i))->getState();
		Vektor3f p = st.pos;
		int objT = st.objType;
		
		
		if (objT==o_BoundingField)	// wenn Obj=BoundingField
		{
			
			BoundingField* bf = (BoundingField*)(sol->getSpObj(i));		// Zeiger holen
			
			
			if (show_BoundingSpheres)	// Bounding-Field Anzeigen
			{
				glPushMatrix();
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				
				glTranslatef(p.x, p.y, p.z);
				
				glDisable( GL_LIGHTING );	// Beleuchtung aus
				glPolygonMode(GL_FRONT,GL_LINE);	// nur Linien
				glColor4f( 0.0f, 0.6f, 0.0f, 1.0f );	// dunkel-grün
				
				if (bf->isbox)	// falls BoxField
				{
					glPushMatrix();
					Vektor3f box = bf->getBox();
					glScalef((2.0f*box.x),(2.0f*box.y),(2.0f*box.z));	// scale: BoundingBox
					glCallList(base+o_Cube);	// Zeichne Box
					glPopMatrix();
				};
				
				//					else	// falls SphereField
				//					{
				float rad = bf->getBoundRad();
				glScalef(rad,rad,rad);	// scale: BoundingSphere
				glCallList(base+o_Kugel);	// Zeichne Sphere
				//					};
				
				glPopAttrib();
				glPopMatrix();
			};
			
			
			for (int j=0; j<(bf->getCount()); j++)	// alle SubObjekte des Fields durchgehen
			{
				
				// State holen, ACHTUNG: State des BoundingField wird überschrieben!
				st = (bf->getSpObj(j))->getState();
				p=st.pos;
				
				if (!bFog || fFogFar==0.0f || (p-camPos).sqrLength()<=FogFarPlane)
				{
					Vektor3f ori=st.ori;
					Vektor3f up=st.up;
					Vektor3f xa = (bf->getSpObj(j))->myX;
					int objT = st.objType;
					
					// Matrix aufstellen
					matrix[0] =  up.x; matrix[4] =  xa.x; matrix[8]  = ori.x; matrix[12] =    p.x; 
					matrix[1] =  up.y; matrix[5] =  xa.y; matrix[9]  = ori.y; matrix[13] =    p.y; 
					matrix[2] =  up.z; matrix[6] =  xa.z; matrix[10] = ori.z; matrix[14] =    p.z; 
					matrix[3] =  0.0f; matrix[7] =  0.0f; matrix[11] =  0.0f; matrix[15] = 1.0f;
					
					glPushMatrix();	
					glPushAttrib(GL_CURRENT_BIT);
					
					glMultMatrixf(matrix);	// Matrix anwenden
					
					
					if ( objT==o_Nebula )
					{
						drawNebula(bf->getSpObj(i));
					}
					else
					{
						glCallList(base+objT);	// Platziere Objekt von passendem Typ
					};
					
					
					if (objT==o_Ship )	// Schiff-Düsen
					{
						cpRoll=-Schiff->cwccw*0.7f ;
						cpUD= -Schiff->updown*0.8f +shakePos.y*20;
						cpLR= -Schiff->leftright*0.7f+shakePos.z*20;
						
						//glLoadIdentity();
						glRotatef(cpRoll,0.0f,0.0f,1.0f);
						glRotatef(cpUD,0.0f,1.0f,0.0f);
						glRotatef(cpLR,1.0f,0.0f,0.0f);
						
						glCallList(base+objT);
						
						
						
						
						float scale=hScal*1.5f;
						float len=0.001f*(sol->getSpObj(i)->getSpeedSet());
						bool afterburn=(afterburner && (aftFuel>0));
						if (afterburn) len=1.5f;
						if (len>0 && !menuMode)
						{
							drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale,afterburn);
							drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale,afterburn);
						}
						
					}	// Schiff-Düsen
					
					
					if ( objT==o_Missile )		// Missile-Düsen
					{
						float scale=(p-camPos).length();
						if (scale!=0)scale=hScal*20.0f/scale;
						//float len=0.0015f*(bf->getSpObj(j)->getSpeedSet());
						float len=1.5f;
						if (len>0)
						{
							drawThruster(exhaustParticle,Vektor3f(0.0f,0.0f,-0.5f),Vektor3f(0.0f,0.0f,-0.5f-len),scale);
						};
					}	// Missile-Düsen
					
					
					if ( objT==o_OtherShip )	// OtherShip-Düsen
					{
						float scale=(p-camPos).length();
						if (scale!=0)scale=hScal*15.0f/scale;
						//float len=0.0015f*(bf->getSpObj(j)->getSpeedSet());
						float len=1.5f;
						if (len>0 &&!menuMode)
						{
							drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale);
							drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale);
						}
					}	// OtherShip-Düsen
					
					
					// CheckSpheres anzeigen
					if ((show_CheckSpheres) && ( (objT==o_Gate1) || (objT==o_Gate2) ) )
					{
						glPushMatrix();
						glPushAttrib(GL_ALL_ATTRIB_BITS);
						glDisable( GL_LIGHTING );	// Beleuchtung aus
						glPolygonMode(GL_FRONT,GL_LINE);	// nur Linien
						glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );		// gelb
						glScalef(7.5f,7.5f,7.5f);	// scale: CheckPoints, Achtung feste Größe
						glCallList(base+o_Kugel);	// Zeichne Sphere
						glPopAttrib();
						glPopMatrix();
					}
					
					// Bounding Spheres anzeigen
					if (show_BoundingSpheres)
					{
						glPushMatrix();
						glPushAttrib(GL_ALL_ATTRIB_BITS);
						glDisable( GL_LIGHTING );	// Beleuchtung aus
						glPolygonMode(GL_FRONT,GL_LINE);	// nur Linien
						glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );		// rot
						float rad = (bf->getSpObj(j))->getBoundRad();
						glScalef(rad,rad,rad);	// scale: BoundingSphere
						glCallList(base+o_Kugel);	// Zeichne Sphere
						glPopAttrib();
						glPopMatrix();
					};
					
					glPopAttrib();
					glPopMatrix();
				} //fogfarplane test
				
			};	// for
		}
		
		
		else	// falls kein BoundingField
		{
			if (!bFog || fFogFar==0.0f || (p-camPos).sqrLength()<=FogFarPlane)
			{
				
				// Rest des State, der für die Bounding-Box irrelevant war
				Vektor3f ori=st.ori;
				Vektor3f up=st.up;
				Vektor3f xa = (sol->getSpObj(i))->myX;
				
				
				// Matrix aufstellen
				matrix[0] =  up.x; matrix[4] =  xa.x; matrix[8]  = ori.x; matrix[12] =    p.x; 
				matrix[1] =  up.y; matrix[5] =  xa.y; matrix[9]  = ori.y; matrix[13] =    p.y; 
				matrix[2] =  up.z; matrix[6] =  xa.z; matrix[10] = ori.z; matrix[14] =    p.z; 
				matrix[3] =  0.0f; matrix[7] =  0.0f; matrix[11] =  0.0f; matrix[15] = 1.0f;
				
				glPushMatrix();	
				glPushAttrib(GL_CURRENT_BIT);
				
				glMultMatrixf(matrix);	// Matrix anwenden
				
				
				if ( objT==o_Nebula )
				{
					drawNebula(sol->getSpObj(i));
				}
				else if (!objT==o_Ship)
				{
					glCallList(base+objT);	// Platziere Objekt von passendem Typ
				};
				
				
				
				
				if (objT==o_Ship )	// Schiff-Düsen
				{
					cpRoll=-Schiff->cwccw*0.7f ;
					cpUD= -Schiff->updown*0.8f +shakePos.y*20;
					cpLR= -Schiff->leftright*0.7f+shakePos.z*20;
					
					//glLoadIdentity();
					glRotatef(cpRoll,0.0f,0.0f,1.0f);
					glRotatef(cpUD,0.0f,1.0f,0.0f);
					glRotatef(cpLR,1.0f,0.0f,0.0f);
					
					glCallList(base+objT);
					
					
					
					
					float scale=hScal*1.5f;
					float len=0.001f*(sol->getSpObj(i)->getSpeedSet());
					bool afterburn=(afterburner && (aftFuel>0));
					if (afterburn) len=1.5f;
					if (len>0 && !menuMode)
					{
						drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale,afterburn);
						drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale,afterburn);
					}
					
				}	// Schiff-Düsen
				
				
				if (objT==o_Missile )		// Missile-Düsen
				{
					float scale=(p-camPos).length();
					if (scale!=0)scale=hScal*20.0f/scale;
					//float len=0.0015f*(sol->getSpObj(i)->getSpeedSet());
					float len=1.5f;
					if (len>0)
					{
						drawThruster(exhaustParticle,Vektor3f(0.0f,0.0f,-0.5f),Vektor3f(0.0f,0.0f,-0.5f-len),scale);
					};
				}	// Missile-Düsen
				
				
				if (objT==o_OtherShip )	// OtherShip-Düsen
				{
					float scale=(p-camPos).length();
					if (scale!=0)scale=hScal*15.0f/scale;
					//float len=0.0015f*(sol->getSpObj(i)->getSpeedSet());
					float len=1.5f;
					if (len>0 &&!menuMode)
					{
						drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale);
						drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale);
					}
				}	// OtherShip-Düsen
				
				
				// CheckSpheres anzeigen
				if ((show_CheckSpheres) && ( (objT==o_Gate1) || (objT==o_Gate2) ) )
				{
					glPushMatrix();
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					glDisable( GL_LIGHTING );	// Beleuchtung aus
					glPolygonMode(GL_FRONT,GL_LINE);	// nur Linien
					glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );		// gelb
					glScalef(7.5f,7.5f,7.5f);	// scale: CheckPoints, ** Achtung feste Größe
					glCallList(base+o_Kugel);	// Zeichne Sphere
					glPopAttrib();
					glPopMatrix();
				}	// CheckSpheres
				
				
				// Bounding Spheres anzeigen
				if (show_BoundingSpheres)
				{
					glPushMatrix();
					glPushAttrib(GL_ALL_ATTRIB_BITS);
					glDisable( GL_LIGHTING );	// Beleuchtung aus
					glPolygonMode(GL_FRONT,GL_LINE);	// nur Linien
					glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );		// rot
					float rad = sol->getSpObj(i)->getBoundRad();
					glScalef(rad,rad,rad);	// scale: BoundingSphere
					glCallList(base+o_Kugel);	// Zeichne Sphere
					glPopAttrib();
					glPopMatrix();
				};	// BoundingSpheres
				
				
				glPopAttrib();
				glPopMatrix();
			}//fogfarplane
			
		};	// else
		
	};	// for
	
	
	delete matrix;
	
	glEndList();
	
}



void PaintWidget::sltMenu(Menu* m)
{
	
	mainMenu=m;
	//printf("*boing*\n");
	menuMode=true;
}



void PaintWidget::setCameraState(const State& s, const Vektor3f& sx)
{
	// Cockpit-View  vs.  Hinterher-View
	if (!hinterherflieg)
	{
		camPos=s.pos;
	}
	else
	{
		camPos=s.pos-s.ori*15.0f;
	};
	camOri=camPos+s.ori;
	camUp=s.up;
	camRealOri=s.ori;
	camX=sx;
}



typedef unsigned int BGRA;

GLuint PaintWidget::loadTexture(char *filename, bool mipMap)
{
	QImage *image = new QImage(filename);
	
	int width = image->width();
	int height = image->height();
	BGRA *texData = new BGRA[width*height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			BGRA pix = image->pixel(x,y);
			texData[x+y*width] = pix;
		}
	}
	GLuint result;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	if (mipMap) glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );	// Linear mipmap Filtering
	else glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texData);
	
	if (mipMap) gluBuild2DMipmaps( GL_TEXTURE_2D, GL_COMPRESSED_RGB, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE,texData );
	
	
	delete image;
	
	delete texData;
	return result;
}



GLuint PaintWidget::loadAlphaTexture(char *filename, bool mipMap)
{
	QImage *image = new QImage(filename);
	
	QString name(filename);
	QString maskName=name.left(name.length()-4)+"mask.bmp"; 
	printf( "Loading mask: %s ....\n",  (const char *)maskName );
	
	QImage *image2 = new QImage(maskName);		
	
	BGRA ff=255;
	BGRA mask=(ff<<16)|(ff<<8)|ff;
	
	int width = image->width();
	int height = image->height();
	BGRA *texData = new BGRA[width*height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			//Alpha				RGB
			BGRA pix =  (image2->pixel(x,y)<<24)|(image->pixel(x,y)&mask);
			texData[x+y*width] = pix;
		}
	}
	GLuint result;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);
	if (mipMap) glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );	// Linear mipmap Filtering
	else glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texData);
	
	if (mipMap) gluBuild2DMipmaps( GL_TEXTURE_2D, GL_COMPRESSED_RGBA, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE,texData );
	
	delete image;
	delete image2;
	delete texData;
	return result;
}



bool PaintWidget::loadMesh(const QString& name, bool bObjNormals)
{
	if ( name.isEmpty() )
		return false;
	
	std::vector<Vektor3f> node;
	node.clear();
	
	std::vector<Vektor3f> normals;
	normals.clear();
	
	std::vector<objFace> triObj;
	triObj.clear();
	
	std::vector<Tex> coords;
	coords.clear();
	
	std::vector<Face> tri;
	tri.clear();
	
	
	QFile data(name);
	QStringList line;
    QString s;
	bool goOn=true;
	
	Vektor3f myV;
	Tex myTex;
	
	node.push_back(myV);
	
	
	int fileType;
	
	if (name.right(3).upper()=="SMF")
		fileType=0;
	if (name.right(3).upper()=="ASC")
		fileType=1;
	
	if (name.right(3).upper()=="OBJ")
		fileType=2;
	
	if (fileType==2)
	{
		normals.push_back(myV);
		coords.push_back(myTex);
	};
	
	if (data.open(IO_ReadOnly) )
	{
		printf( "Loading Mesh: %s ... ",  (const char *)name );
		QTextStream t( &data );        // use a text stream
		
		switch(fileType)
		{
			
		case 0:
			
			
			while ( goOn ) // until end of file...
			{			
				s = t.readLine();       // line of text excluding '\n'
				line=QStringList::split(' ', s);
				
				if (line[0]=="v")
				{
					Vektor3f cV;
					cV.x=line[1].toFloat();
					cV.y=line[2].toFloat();
					cV.z=line[3].toFloat();
					node.push_back(cV);
				};
				
				if (line[0]=="f")
				{
					
					Face cF;
					cF.w[0]=line[1].toUInt();
					cF.w[1]=line[2].toUInt();
					cF.w[2]=line[3].toUInt();
					tri.push_back(cF);
				};
				
				goOn=!t.eof();
				if (line[0]=="bind") 
				{
					goOn=false;
				};
				
			}; //while
			
			   /*		if (bunt)
			   {
			   faceCol.push_back(QColor(0,0,0));
			   while(!t.eof())
			   {
			   s = t.readLine();       // line of text excluding '\n'
			   line=QStringList::split(' ', s, false);
			   if (line[0]=="c")
			   {
			   
				 faceCol.push_back(QColor((int)(line[1].toFloat()*255),
				 (int)(line[2].toFloat()*255),(int)(line[3].toFloat()*255) ));
				 
				   };
		};*/
			break;
			
		case 1:
			
			while (!t.eof())
			{
				s=t.readLine();
				line=QStringList::split(' ', s);
				if (line.count()==8) 
				{
					if (line[0]=="Face") 
					{
						Face cF;
						cF.w[0]=line[2].mid(2).toUInt()+1;
						cF.w[1]=line[3].mid(2).toUInt()+1;
						cF.w[2]=line[4].mid(2).toUInt()+1;
						tri.push_back(cF);
					}
					
				};
				if (line.count()>=5) 
				{
					Vektor3f cV;
					cV.x=line[2].mid(2).toFloat()/100.0f;
					cV.y=line[3].mid(2).toFloat()/100.0f;
					cV.z=line[4].mid(2).toFloat()/100.0f ;
					node.push_back(cV);
				};
			}; //while
			break;
			
		case 2:
			
			while(!t.eof())
			{
				s=t.readLine();
				line=QStringList::split(' ', s);
				
				if (line[0]=="v")
				{
					Vektor3f cV;
					cV.x=line[1].toFloat();
					cV.y=line[2].toFloat();
					cV.z=line[3].toFloat();
					node.push_back(cV);
				};
				
				if (line[0]=="f")
				{
					QStringList facelist;
					facelist=QStringList::split('/',line[1]);
					
					objFace cF;
					cF.w[0]=facelist[0].toUInt();//punkt 0
					cF.w[1]=facelist[1].toUInt();//normale 0
					cF.w[2]=facelist[2].toUInt();//tex 0
					facelist=QStringList::split('/',line[2]);
					
					cF.w[3]=facelist[0].toUInt();//p 1
					cF.w[4]=facelist[1].toUInt();//n 1
					cF.w[5]=facelist[2].toUInt();//t 1
					
					facelist=QStringList::split('/',line[3]);
					
					cF.w[6]=facelist[0].toUInt();//p 2
					cF.w[7]=facelist[1].toUInt();//n 2
					cF.w[8]=facelist[2].toUInt();//t 2
					triObj.push_back(cF);
				};
				
				if (line[0]=="vn")
				{
					Vektor3f cN;
					cN.x=line[1].toFloat();
					cN.y=line[2].toFloat();
					cN.z=line[3].toFloat();
					normals.push_back(cN);
				};
				
				
				if (line[0]=="vt")
				{
					Tex cT;
					cT.w[0]=line[1].toFloat();
					cT.w[1]=line[2].toFloat();
					coords.push_back(cT);
				};
				
				/*if (line[0]=="mtllib") //für materials später
				
				if (line[0]=="usemtl")*/
				
				goOn=!t.eof();
				
				if (line[0]=="bind") 
				{
					goOn=false;
				};
			};//while
			break;
			
		}; // switch
		
        data.close();
		
	}
	
	else
	{
		printf("* !! Konnte Datei %s nicht oeffnen!\n", (const char *)name);
	};	// if
	
	
	printf("Nodes: %d  ",node.size());
	printf("Faces: %d   ",tri.size());
	printf( "Load complete!\n");
	
	
	glBegin(GL_TRIANGLES); 
	//	glPushMatrix();	
	
	if (fileType<2) //obj hat normalen schon dabei
	{
		
		Vektor3f n;
		normals.clear();
		
		std::vector<Vektor3f> faceNormals;
		faceNormals.clear();
		
		for (int i=0;i<tri.size();i++)
		{
			n=(node[tri[i].w[1]]-node[tri[i].w[0]]).kreuz(node[tri[i].w[2]]-node[tri[i].w[0]]) ;
			n.normalize();
			//	n.show();
			faceNormals.push_back(n);
		}
		
		
		for ( i=0;i<node.size();i++)
		{
			normals.push_back(Vektor3f(0.0f,0.0f,0.0f));
		}
		
		
		for (i=0;i<tri.size();i++)
		{
			n=faceNormals[i];
			
			normals[tri[i].w[0]]=n + normals[tri[i].w[0]];
			normals[tri[i].w[1]]=n + normals[tri[i].w[1]];
			normals[tri[i].w[2]]=n + normals[tri[i].w[2]];
		}
		
		
		for (i=0;i<node.size();i++)
		{
			normals[i].normalize();
		}
		
		
		//=====================================================
		
		for (int poly=0;poly<tri.size();poly++) 
		{
			int p0=tri[poly].w[0];
			int p1=tri[poly].w[1];
			int p2=tri[poly].w[2];
			
			glColor3f(1.0f,1.0f,1.0f);	
			//	glColor3f(0.3f,0.3f,0.3f);	
			
			glTexCoord2f(0.0f, 0.0f);
			glNormal3f( normals[p0].x ,normals[p0].y ,normals[p0].z);
			glVertex3f(node[ p0].x,node[ p0].y,node[ p0].z);
			
			//	glColor3f(0.6f,0.6f,0.6f);	
			
			glTexCoord2f(0.0f, 1.0f);
			glNormal3f( normals[p1].x ,normals[p1].y ,normals[p1].z);
			glVertex3f(node[ p1].x,node[ p1].y,node[ p1].z);
			
			
			glTexCoord2f(1.0f, 1.0f);
			glNormal3f( normals[p2].x ,normals[p2].y ,normals[p2].z);
			glVertex3f(node[ p2].x,node[ p2].y,node[ p2].z);
			
		};	// for
		
		
		//	glPopMatrix();	
	}//endif filetype<2
	//------OBJ
	
	else if (!bObjNormals)//filetype2
	{
		Vektor3f n;
		normals.clear();
		
		std::vector<Vektor3f> faceNormals;
		faceNormals.clear();
		
		for (int i=0;i<triObj.size();i++)
		{
			n=(node[triObj[i].w[3]]-node[triObj[i].w[0]]).kreuz(node[triObj[i].w[6]]-node[triObj[i].w[0]]) ;
			n.normalize();
			//	n.show();
			faceNormals.push_back(n);
		}
		
		
		for ( i=0;i<node.size();i++)
		{
			normals.push_back(Vektor3f(0.0f,0.0f,0.0f));
		}
		
		
		for (i=0;i<triObj.size();i++)
		{
			n=faceNormals[i];
			
			normals[triObj[i].w[0]]=n + normals[triObj[i].w[0]];
			normals[triObj[i].w[3]]=n + normals[triObj[i].w[3]];
			normals[triObj[i].w[6]]=n + normals[triObj[i].w[6]];
		}
		
		
		for (i=0;i<node.size();i++)
		{
			normals[i].normalize();
		}
		
		
		//=====================================================
		
		for (int poly=0;poly<triObj.size();poly++) 
		{
			int p0=triObj[poly].w[0];
			int p1=triObj[poly].w[3];
			int p2=triObj[poly].w[6];
			int t0=triObj[poly].w[2];
			int t1=triObj[poly].w[5];
			int t2=triObj[poly].w[8];
			
			glColor3f(1.0f,1.0f,1.0f);	
			//	glColor3f(0.3f,0.3f,0.3f);	
			
			
			glTexCoord2f(coords[t0].w[0], coords[t0].w[1]);
			glNormal3f( normals[p0].x ,normals[p0].y ,normals[p0].z);
			glVertex3f(node[ p0].x,node[ p0].y,node[ p0].z);
			
			//	glColor3f(0.6f,0.6f,0.6f);	
			
			glTexCoord2f(coords[t1].w[0], coords[t1].w[1]);
			glNormal3f( normals[p1].x ,normals[p1].y ,normals[p1].z);
			glVertex3f(node[ p1].x,node[ p1].y,node[ p1].z);
			
			
			glTexCoord2f(coords[t2].w[0], coords[t2].w[1]);
			glNormal3f( normals[p2].x ,normals[p2].y ,normals[p2].z);
			glVertex3f(node[ p2].x,node[ p2].y,node[ p2].z);
			
		};	// for
		
		
	}//else filetype 2
	else //filetype 2 && bObjNormals
	{
		
		for (int poly=0;poly<triObj.size();poly++) 
		{
			
			int p0=triObj[poly].w[0];
			int p1=triObj[poly].w[3];
			int p2=triObj[poly].w[6];
			int n0=triObj[poly].w[1];
			int n1=triObj[poly].w[4];
			int n2=triObj[poly].w[7];
			int t0=triObj[poly].w[2];
			int t1=triObj[poly].w[5];
			int t2=triObj[poly].w[8];
			if (poly==1)
				
				//printf("normale 1.x: %4.4f \n",normals[n0].x);
				
				
				glColor3f(1.0f,1.0f,1.0f);	
			//	glColor3f(0.3f,0.3f,0.3f);	
			
			glTexCoord2f(coords[t0].w[0], coords[t0].w[1]);
			glNormal3f( normals[n0].x ,normals[n0].y ,normals[n0].z);
			glVertex3f(node[ p0].x,node[ p0].y,node[ p0].z);
			
			//	glColor3f(0.6f,0.6f,0.6f);	
			glTexCoord2f(coords[t1].w[0], coords[t1].w[1]);
			
			glNormal3f( normals[n1].x ,normals[n1].y ,normals[n1].z);
			glVertex3f(node[ p1].x,node[ p1].y,node[ p1].z);
			
			glTexCoord2f(coords[t2].w[0], coords[t2].w[1]);
			glNormal3f( normals[n2].x ,normals[n2].y ,normals[n2].z);
			glVertex3f(node[ p2].x,node[ p2].y,node[ p2].z);
			
		};	// for
		
	} //endelse filetype 2 (obj)*/
	
	glEnd();
	
	return true;
	
};	// loadMesh


void PaintWidget::mousePressEvent(QMouseEvent* event)
{
	
	if (event->button()==LeftButton)
	{
		Schiff->sltKey(55);
	}
	else if (event->button()==RightButton)
		Schiff->sltKey(57);
	else if (event->button()==MidButton)
		Schiff->sltKey(4097);
	//	updateGL();					//neu zeichnen
	
};

void PaintWidget::mouseReleaseEvent(QMouseEvent* event)
{
	
	if (event->button()==LeftButton)
	{
		Schiff->sltKeyRelease(55);
	}
	else if (event->button()==RightButton)
		Schiff->sltKeyRelease(57);
	else if (event->button()==MidButton)
		Schiff->sltKeyRelease(4097);
	//	updateGL();					//neu zeichnen
	
};



void PaintWidget::mouseMoveEvent(QMouseEvent* event)
{
	//	updateGL();					//neu zeichnen
	
	Schiff->setLR(int((event->x()-width()*0.5f)*mScalX));
	Schiff->setUD(int((event->y()-height()*0.5f)*mScalY));
};



void PaintWidget::updateJoystick()
{
	if (jStick)
	{
		joyGetPosEx(JOYSTICKID1,&stick);
		Schiff->setLR(int(stick.dwXpos*0.00061f-19));
		Schiff->setUD(int(stick.dwYpos*-0.00061f+19));
		//0.00061 ist 20/32676.  327676 ist der MittelPunkt meines Gamepads,
		//ich hoffe das ist bei jedem Joystick so, kann's aber nicht testen,
		//weil mein anderer streikt
		
		bool b1=false, b2=false,b3=false,ccw=false,cw=false;
		
		if (stick.dwButtons&JOY_BUTTON1)
			b1=true;
		if (stick.dwButtons&JOY_BUTTON2)
			b2=true;
		if (stick.dwButtons&JOY_BUTTON3)
			b3=true;
		if(stick.dwButtons&JOY_BUTTON9) ccw=true;
		
		if(stick.dwButtons&JOY_BUTTON10) cw=true;
		
		
		Schiff->kpl=b2;
		Schiff->kmi=b1;
		Schiff->ktab=b3;
		afterburner=b3;
		Schiff->k7=ccw;
		Schiff->k9=cw;
		
	}
}

#include"paint_widget_qt_moc.cpp"
