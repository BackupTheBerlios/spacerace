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





#ifndef _INTROCONTROLLER_H
#define _INTROCONTROLLER_H 1


#include <qwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfile.h>
#include <qvector.h>
#include <qlabel.h>
#include <qdatetime.h>

#include "Vektor3f.h"

#include"IntroPaintWidget.h"
#include "SpaceObject.h"
#include "SpaceObjectList.h"

#include "Ship.h"
#include "Gate.h"
#include "Asteroid.h"
#include "Mesh.h"
#include "Missile.h"
#include "BoundingField.h"
#include "Nebula.h"

#include "TCPServer.h"
#include "TCPClient.h"

#include "OtherShip.h"

#include "enum_names.h"
#include "Menu.h"

#include "MySound.h"



class IntroController:public QWidget
{
	Q_OBJECT
		
		
public:
	
	IntroController(const QString &host,const QString &portString, QWidget * parent=0, const char * name=0, WFlags f=0);
	virtual ~IntroController();
	
	
	
private:
	
	int tickCount;
	bool alreadyDone;
	
	Ship* Schiff1;
	
	SpaceObject* Schiff2;
	SpaceObject* Schiff3;
	SpaceObject* Schiff4;
	SpaceObject* OurNames;
	SpaceObject* Flare;
	
	IntroPaintWidget* Viewer;
	
	SpaceObjectList* viewList;
	
	QTimer *timer;
	
	bool playingmusic;
	
	void resizeEvent(QResizeEvent*);
	void keyPressEvent( QKeyEvent* );
	
	MySound * sndMusic;
	
	
	
protected:
	
	
	
public slots:
	
	void tick(void);
	void sltResize(QResizeEvent*);
	
	
signals:
	
	void sigResize(QResizeEvent* event);
	void sigPaint ();
	void sigUpdateViewer(SpaceObjectList*);
	void quit();
	
};

#endif
