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





#include"IntroPaintWidget.h"
 /*#define GLH_EXT_SINGLE_FILE 1
#include "glh_extensions.h"*/

//#include <gl\aux.h>



// Konstruktor.
IntroPaintWidget::IntroPaintWidget(QWidget * parent, const char * name, WFlags f)
:QGLWidget(parent,name,0,f)
{
	rtri=0.0f;
	rquad=0.0f;
	
	listRange=40;	// Anzahl der Display Listen
	
	lensPos = Vektor3f(-800.0f,0.0f,0.0f);
	
	afterburner=false;
	shake_strength=0;
	shakePos = Vektor3f(0.0f,0.0f,0.0f);
	
	maxDist=100.0f;
	
	screen_fade = 0.93f;
	
	hinterherflieg = false;
	
	wScal=w*0.0009765625f;
	hScal=h*0.0013888888888888888888888888888889f;
	
	bRainbow=false;
	iClouds=300;
	bFog=true;
	bFlash=false;
	fFogFar=725.0f;
	FogColor[0]=0.25f;
	FogColor[1]=0.35f;
	FogColor[2]=0.4f;
	FogColor[3]=1.0f;
	FogAlpha=0.15f;
	
	AmbColor[0]=0.10f; AmbColor[1]=0.11f; AmbColor[2]=0.12f; AmbColor[3]=1.0f;
	DiffColor[0]=1.0f; DiffColor[1]=1.0f; DiffColor[2]=1.0f; DiffColor[3]=1.0f;
	
	DiffScale=0.6f;
	AmbScale=0.2f;
	
	cpRoll=0.0f,cpLR=0.0f,cpUD=0.0f;
	
	mScalX=18/(width()*0.5f) ;
	mScalY=18/(height()*0.5f) ;
	
};



// Destruktor.
IntroPaintWidget::~IntroPaintWidget()
{
};



GLvoid IntroPaintWidget::BuildFont(GLvoid)        // Build Our Bitmap Font
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



GLvoid IntroPaintWidget::glPrint(const char *fmt, ...)     
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
}



void IntroPaintWidget::renderCloudParticles(quadparticles particle[])
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



void IntroPaintWidget::drawNebula(SpaceObject* nebula)
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



void IntroPaintWidget::drawThruster(particles exhaustParticle[], const Vektor3f& center, Vektor3f& target,const float& scale, bool burner)
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



void IntroPaintWidget::updateThruster(particles exhaustParticle[])
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



void IntroPaintWidget::resetDeadCloudParticles(quadparticles particle[])
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




void IntroPaintWidget::renderParticles(particles particle[])
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




void IntroPaintWidget::resetDeadParticles(particles particle[])
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



void IntroPaintWidget::initializeGL() //Rendermode, Licht, Persp....
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
	
	showCab(); //Zeige Hardware-Daten
	
	
	gTextures[t_Cockpit]=loadAlphaTexture("texturen/cockpit3.bmp");
	gTextures[t_Metal]=loadTexture("texturen/gate.bmp", true);
	gTextures[t_Rock1]=loadTexture("texturen/asteroid1.bmp", true);
	gTextures[t_Rock2]=loadTexture("texturen/asteroid2.bmp", true);
	gTextures[t_Rock3]=loadTexture("texturen/asteroid3.bmp", true);
	gTextures[t_Stars]=loadTexture("texturen/stars.bmp");
	gTextures[t_Flare]=loadAlphaTexture("texturen/flare1.bmp");
	gTextures[t_Halo]=loadAlphaTexture("texturen/halo2.bmp");
	gTextures[t_Ship]=loadTexture("texturen/racertop.bmp", true);
	gTextures[t_fog]=loadAlphaTexture("texturen/fog.bmp", true);
	gTextures[t_Alex]=loadAlphaTexture("texturen/alex.bmp", true);
	gTextures[t_Benni]=loadAlphaTexture("texturen/benni.bmp", true);
	gTextures[t_Mark]=loadAlphaTexture("texturen/mark.bmp", true);
	gTextures[t_Present]=loadAlphaTexture("texturen/present.bmp", true);
	gTextures[t_Title]=loadAlphaTexture("texturen/spacerace.bmp", false);
	
	
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
	
	glLoadIdentity();
	
	
	
	
	//die SkyBox
	glNewList(base+o_Skybox, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Stars]);	// rundrum: Stars
	// Center the Skybox around the given x,y,z position
    float width=1200.0,height=1200.0,length=1200.0;
	
	float x = camPos.x - width  / 2;
	float y = camPos.y - height / 2;
	float z = camPos.z - length / 2;
	
	float x2 = x + width;
	float y2 = y + height;
	float z2 = z + length;
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 
	
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
	
	glLoadIdentity();
	
	
	
	// Wolke-Alex
	glNewList(base+o_Alex,GL_COMPILE);
	//glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Alex]);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.003f);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);glVertex3f(-3.0f, -3.0f, 0.0f);
	glTexCoord2f(1.0f,1.0f);glVertex3f(-3.0f, 3.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f(3.0f, 3.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f(3.0f, -3.0f, 0.0f);
	glEnd();
	
	glPopAttrib();
	//glPopMatrix();
	glEndList();
	
	
	// Wolke-Benni
	glNewList(base+o_Benni,GL_COMPILE);
	//glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Benni]);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.003f);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);glVertex3f(-3.0f, -3.0f, 0.0f);
	glTexCoord2f(1.0f,1.0f);glVertex3f(-3.0f, 3.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f(3.0f, 3.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f(3.0f, -3.0f, 0.0f);
	glEnd();
	
	glPopAttrib();
	//glPopMatrix();
	glEndList();
	
	
	// Wolke-Mark
	glNewList(base+o_Mark,GL_COMPILE);
	//glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Mark]);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.003f);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);glVertex3f(-3.0f, -3.0f, 0.0f);
	glTexCoord2f(1.0f,1.0f);glVertex3f(-3.0f, 3.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f(3.0f, 3.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f(3.0f, -3.0f, 0.0f);
	glEnd();
	
	glPopAttrib();
	//glPopMatrix();
	glEndList();
	
	
	
	// Wolke-Present
	glNewList(base+o_Present,GL_COMPILE);
	//	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Present]);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.003f);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);glVertex3f(-3.0f, -3.0f, 0.0f);
	glTexCoord2f(1.0f,1.0f);glVertex3f(-3.0f, 3.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f(3.0f, 3.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f(3.0f, -3.0f, 0.0f);
	glEnd();
	
	glPopAttrib();
	//	glPopMatrix();
	glEndList();
	
	
	// Space-Race
	glNewList(base+o_Title,GL_COMPILE);
	
	//	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextures[t_Title]);
	glDisable(GL_LIGHTING);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.003f);
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);glVertex3f(-3.0f, -3.0f, 0.0f);
	glTexCoord2f(1.0f,1.0f);glVertex3f(-3.0f, 3.0f, 0.0f);
	glTexCoord2f(1.0f,0.0f);glVertex3f(3.0f, 3.0f, 0.0f);
	glTexCoord2f(0.0f,0.0f);glVertex3f(3.0f, -3.0f, 0.0f);
	glEnd();
	
	glPopAttrib();
	//glPopMatrix();
	glEndList();
	
}		




void IntroPaintWidget::resizeGL (int width, int height )   //Fenstergrösse, Perspektive...
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



void IntroPaintWidget::checkError()
{
	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) 
	{
		errString = gluErrorString(errCode);
		printf("OpenGL Error: %s\n", errString);
	}
}


void IntroPaintWidget::OrthoMode(const int& left, const int& top, const int& right, const int& bottom)
{
	glMatrixMode(GL_PROJECTION);						
	glPushMatrix();									
	glLoadIdentity();								
	glOrtho( left, right, bottom, top, 0, 1 );	
	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();										
}



void IntroPaintWidget::drawLensFlare(const Vektor3f& sun, bool bOcc)
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



void IntroPaintWidget::paintGL()
{
	
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();	// Reset The Current Modelview Matrix
	
	
	// Nachbrenner-Schütteln
	if (!afterburner && (shake_strength>0))	// tab nicht gedrückt und shake noch vorhanden
	{
		shake_strength=shake_strength-25;
		if (shake_strength<0)
		{
			shake_strength=0;
		};
	}
	else if (afterburner && (shake_strength<1000))	// tab gedrückt und shake noch nicht voll da
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
	
	
	glFogfv(GL_FOG_COLOR, FogColor);
	glFogf(GL_FOG_START, 30.0f);
	glFogf(GL_FOG_END, fFogFar);
	glFogi(GL_FOG_MODE,GL_LINEAR);
	glEnable(GL_FOG);
	
	
	glDisable( GL_LIGHTING );
	
	
	// SkyBox
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);	// da als allererstes und immer hinten, kein Z-Buffer
	glTranslatef(camPos.x, camPos.y, camPos.z);	// mitbewegen
	glCallList(base+o_Skybox);	// zeichnen
	glEnable(GL_DEPTH_TEST);	
    glPopMatrix();
	
	
    static GLfloat pos[4] = {50.0f, 80.0f, -100.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, DiffColor);
    glLightfv( GL_LIGHT0, GL_POSITION, pos );
	
	
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	
	// Szene darstellen
	glCallList(base+o_Szene);
	
	glDisable(GL_FOG);
	glDisable( GL_LIGHTING );
	
	updateThruster(exhaustParticle);
	
	renderParticles(particle);
	renderCloudParticles(qParticle);
	
	if (!hinterherflieg)
	{
		cpRoll=-Schiff->cwccw*0.2f ;
		cpUD= (Schiff->updown)*0.18f-3.5f+shakePos.y*20;
		cpLR= -(Schiff->leftright)*0.2f+shakePos.z*20;
	}
	
	drawLensFlare(lensPos);
	
	
	if (screen_fade>0.0f)	// Bild am Anfang einblenden
	{
		OrthoMode(0,0,w,h);	//andere Projektionsmatrix->2D
		glEnable (GL_BLEND);
		glDisable(GL_DEPTH_TEST);	
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);		
		
		glColor4f(0.0f,0.0f,0.0f,screen_fade);
		
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);glVertex2f(0*wScal,0*hScal);
		glTexCoord2f(0.0f,1.0f);glVertex2f(0*wScal,768*hScal);
		glTexCoord2f(1.0f,1.0f);glVertex2f(1024*wScal,768*hScal);
		glTexCoord2f(1.0f,0.0f);glVertex2f(1024*wScal,0*hScal);
		glEnd();
		
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		PerspectiveMode();	//Projektionsmatrix wieder 3D
		screen_fade-=0.015f;
	}
	else
	{
		screen_fade=-1.0f;
	};
	
	
	checkError();
	
}







void IntroPaintWidget::PerspectiveMode()									
{
	glMatrixMode( GL_PROJECTION );							
	glPopMatrix();											
	glMatrixMode( GL_MODELVIEW );							
}



void IntroPaintWidget::paintEvent(QPaintEvent* event)
{
	//	updateGL();
};



void IntroPaintWidget::resizeEvent(QResizeEvent* event)
{
	resize(event->size());
	resizeGL(width(),height());
	w=width();h=height();
	wScal=w*0.0009765625f;
	hScal=h*0.0013888888888888888888888888888889f;
	mScalX=18/(width()*0.5f) ;
	mScalY=18/(height()*0.5f) ;
	
};



void IntroPaintWidget::sltResize(QResizeEvent* event)
{
	resizeEvent(event);
}




//Welche Hardware rendert?
void IntroPaintWidget::showCab(void)
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



void IntroPaintWidget::sltLoad()
{
}



void IntroPaintWidget::sltInput(SpaceObjectList* sol)
{
	
	float FogFarPlane=0;
	
	if (bFog) { FogFarPlane = fFogFar*fFogFar*2+2500; }
	
	SpaceObject* cam = sol->getSpObj(0);
	setCameraState(cam->getState(), cam->myX);
	//camState.show();
	
	
	glNewList(base+o_Szene, GL_COMPILE);
	float*	matrix = new float[16];
	
	for (int i=1; i < (sol->getCount()); i++)		// alle Objekte durchgehen
	{
		
		// State holen
		State st = (sol->getSpObj(i))->getState();
		Vektor3f p = st.pos;
		int objT = st.objType;
		
		
		if (objT==15) { lensPos = p; };
		
		if (objT==o_BoundingField)	// wenn Obj=BoundingField
		{
			
			BoundingField* bf = (BoundingField*)(sol->getSpObj(i));		// Zeiger holen
			
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
						if (len>0)
						{
							drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale,false);
							drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale,false);
						}
					}	// Schiff-Düsen
					
					
					
					if ( objT==o_OtherShip )	// OtherShip-Düsen
					{
						float scale=(p-camPos).length();
						if (scale!=0)scale=hScal*15.0f/scale;
						//float len=0.0015f*(bf->getSpObj(j)->getSpeedSet());
						float len=1.5f;
						if (len>0)
						{
							drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale);
							drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale);
						}
					}	// OtherShip-Düsen
					
					
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
					if (len>0)
					{
						drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale,false);
						drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale,false);
					}
					
				}	// Schiff-Düsen
				
				
				if (objT==o_OtherShip )	// OtherShip-Düsen
				{
					float scale=(p-camPos).length();
					if (scale!=0)scale=hScal*15.0f/scale;
					//float len=0.0015f*(sol->getSpObj(i)->getSpeedSet());
					float len=1.5f;
					if (len>0)
					{
						drawThruster(exhaustParticle,Vektor3f(0.45f,1.25f,-1.7f),Vektor3f(0.45f,1.25f,-1.7f-len),scale);
						drawThruster(exhaustParticle,Vektor3f(0.45f,-1.25f,-1.7f),Vektor3f(0.45f,-1.25f,-1.7f-len),scale);
					}
				}	// OtherShip-Düsen
				
				
				glPopAttrib();
				glPopMatrix();
			}//fogfarplane
			
		};	// else
		
	};	// for
	
	
	delete matrix;
	
	glEndList();
	
}




void IntroPaintWidget::setCameraState(const State& s, const Vektor3f& sx)
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

GLuint IntroPaintWidget::loadTexture(char *filename, bool mipMap)
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



GLuint IntroPaintWidget::loadAlphaTexture(char *filename, bool mipMap)
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
	// if (mipMap) glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );	// Linear mipmap Filtering
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



bool IntroPaintWidget::loadMesh(const QString& name, bool bObjNormals)
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



#include"IntroPaintWidget_moc.cpp"
