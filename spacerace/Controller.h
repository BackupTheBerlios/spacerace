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




#ifndef _CONTROLLER_H
#define _CONTROLLER_H 1

#include <qwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qfile.h>
#include <qvector.h>
#include <qlabel.h>
#include <qdatetime.h>

#include "Vektor3f.h"

#include"paint_widget_qt.h"
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


class Controller:public QWidget
{
	Q_OBJECT
		
public:
	
	Controller(const QString &host,const int &player,int st, QWidget * parent=0, const char * name=0, WFlags f=0);
	virtual ~Controller();
	
	
private:
	
	//	protected:
	
	bool menuMode;
	PaintWidget* Viewer;
	Ship* Schiff1;
	
	
	MySound * sndAllGates;
	MySound * sndTimeOver;
	MySound * sndProc;
	MySound * sndMusic;
	
	
	
	//MP==========
	
	TCPServer *server1;
	TCPServer *server2;
	TCPServer *server3;
	
	
	TCPClSocket *c2;
	TCPClSocket *c3;
	TCPClSocket *c4;
	
	
	int players;
	bool isServer,isClient;
	
	OtherShip* Schiff2;
	OtherShip* Schiff3;
	OtherShip* Schiff4;
	
	TCPClient* cli;
	
	
	//MP==========
	//MPG
	QString myHost;
	int playerNr;
	int stick;
	
	Menu *mainMenu;
	
	
	SpaceObjectList* viewList;
	SpaceObjectList* editList;
	
	SpaceObject* target; int cycleTarg;
	
	
	QTimer *timer;
	QTime *time;
	int gateTime;
	int roundTime;
	bool timeOver;
	
	bool mouseMode;
	
	int gameOver;
	
	Gate* activeGate;
	Gate* firstGate;
	
	Mesh* mesh_Other;
	Mesh* mesh_Asteroid1;
	Mesh* mesh_Asteroid2;
	Mesh* mesh_Asteroid3;
	
	Mesh* mesh_Gate1;
	Mesh* mesh_Gate2;
	
	bool bMulti;
	int iMode;
	int iTrack;
	bool editMode;
	
	bool playingmusic;
	
	//		void resetSzene();
	
	
	
protected:
	
	void mousePressEvent(QMouseEvent*);	
	void resizeEvent(QResizeEvent*);
	void keyPressEvent( QKeyEvent* );
	void keyReleaseEvent( QKeyEvent* );
	
	void loadScene(const QString& name);
	void saveScene(const QString& name,SpaceObjectList*);
	
	int readLine(const QStringList&,SpaceObjectList*);
	
	void connectGates(SpaceObjectList*);
	void cycleTargets();
	
	
	
public slots:
	void tick(void);
	void sltResize(QResizeEvent*);
	void sltGatePassed(Gate*);
	void sltSock1(TCPClSocket* s);
	void sltSock2(TCPClSocket* s);
	void sltSock3(TCPClSocket* s);
	
	void startServer();
	void startClient( QString host, int player);
	
	void stopServer();
	void stopClient();
	
	void startMPGame();
	void serverStartsMP();
	
				//MPG
	void looseMP();
	void serverWon(int);
	void clientWon();
	void clientError();
	
signals:
	void sigResize(QResizeEvent* event);
	void sigPaint ();
	void sigUpdateViewer(SpaceObjectList*);
	void sigKey(int);
	void sigKeyR(int);
	void quit();
	void sigMessage(int);
	
	void sigGameOver(QString,int,int);
};

#endif
