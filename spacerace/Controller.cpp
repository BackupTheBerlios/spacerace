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




#include "Controller.h"



Controller::Controller(const QString &host,const int &player ,int st,QWidget * parent, const char * name, WFlags f)
:QWidget(parent,name,f)
{

	//MPG
	stick=st;
	myHost=host;
	playerNr=player;
	c2=NULL;
	c3=NULL;
	c4=NULL;



	gameOver = 0;
	sndAllGates=new MySound("sounds/progcomp.wav");
	sndTimeOver=new MySound("sounds/Timeover.wav");
	sndProc = new MySound("sounds/proceed.wav");
	sndMusic = new MySound("sounds/music.mp3",1);
	playingmusic=false;

	iTrack=0;
	
	isServer=false;
	isClient=false;

	QGLFormat form;
	form.setStencil(true);
	form.setDirectRendering(true);
	QGLFormat::setDefaultFormat(form);

	Viewer = new PaintWidget(this, "viewer", 0);
	connect (this,SIGNAL(sigResize(QResizeEvent*)),Viewer,SLOT(sltResize(QResizeEvent*)));

	Schiff1 = new Ship();
	Viewer->Schiff=Schiff1;

	mesh_Other = new Mesh("Models/racercollilow.smf");
	Schiff2 = new OtherShip(mesh_Other);
	Schiff3 = new OtherShip(mesh_Other);
	Schiff4 = new OtherShip(mesh_Other);


	Schiff2->setPos(Vektor3f(0.0f, 0.0f, 6.0f));
	Schiff3->setPos(Vektor3f(0.0f, 0.0f, -6.0f));
	Schiff4->setPos(Vektor3f(0.0f, 0.0f, 12.0f));

	viewList = new SpaceObjectList();
	viewList->addItem(Schiff1);
	viewList->addItem(Schiff2);
	viewList->addItem(Schiff3);
	viewList->addItem(Schiff4);


	editList = new SpaceObjectList();
	editList->addItem(Schiff1);
	editList->addItem(Schiff2);
	editList->addItem(Schiff3);
	editList->addItem(Schiff4);
	
	time=new QTime;
    time->start();    
	gateTime=20;
	roundTime=0;
	timeOver=false;

	activeGate=NULL;
	firstGate=NULL;
	target=NULL;

	mesh_Asteroid1 = new Mesh("Models/asteroid1.smf");
	mesh_Asteroid2 = new Mesh("Models/asteroid2.smf");
	mesh_Asteroid3 = new Mesh("Models/asteroid3.smf");

	mesh_Gate1 = new Mesh("Models/tracktex2coll.smf");
	mesh_Gate2 = new Mesh("Models/gate2collibig.smf");

	loadScene("Courses/Course_2.szn");


	timer = new QTimer( this );
    QObject::connect( timer, SIGNAL(timeout()), SLOT(tick()) );


	connect (this,SIGNAL(sigPaint()),Viewer,SLOT(updateGL()));
	connect (this,SIGNAL(sigUpdateViewer(SpaceObjectList*)),Viewer,SLOT(sltInput(SpaceObjectList*)));

	// Tasten-Signale connecten
	connect(this, SIGNAL( sigKey(int) ), Schiff1, SLOT( sltKey(int) ) );
	connect(this, SIGNAL( sigKeyR(int) ), Schiff1, SLOT( sltKeyRelease(int) ) );

	setFocus();

	
	mainMenu=new Menu();
	menuMode=true;
	bMulti=false;

	mouseMode=false;


	connect(mainMenu, SIGNAL( startServer() ), this, SLOT( startServer() ) );
	connect(mainMenu, SIGNAL( startClient(QString,int) ), this, SLOT( startClient(QString,int) ) );
	connect(mainMenu, SIGNAL( stopServer() ), this, SLOT( stopServer() ) );
	connect(mainMenu, SIGNAL( stopClient() ), this, SLOT( stopClient() ) );
	connect(mainMenu, SIGNAL( startMPGame() ), this, SLOT( startMPGame() ) );

	connect(this, SIGNAL( sigMessage(int) ), Viewer, SLOT( drawMessage(int) ));


	timer->start( TICKRATE, FALSE );
	editMode=0;

	printf("Mode: %s\n",(const char*)myHost);
	if(myHost!="single")
	{
		if(myHost=="server")
		{
			startServer();
			mainMenu->menuPoint=5;
		}
		else
		{
			startClient(myHost,player);
				mainMenu->menuPoint=6;
			mainMenu->connected=true;

		}

	}

	if (stick)
	{
		mouseMode=!mouseMode;
		if (mouseMode)
			Viewer->setMouseTracking(true);
		else Viewer->setMouseTracking(false);
			Schiff1->mouseMode=mouseMode;	
		Viewer->jStick=!Viewer->jStick;
	}

}



Controller::~Controller()
{
	sndAllGates->stop();
	sndTimeOver->stop();
	sndProc->stop();
	sndMusic->stop();

	delete  Schiff1;
	delete  Schiff2;
	delete  Schiff3;
	delete  Schiff4;
	if(isServer)	delete server1;
	if(isServer)	delete server2;
	if(isServer)	delete server3;

	if(isClient)	delete cli;

	delete	viewList;
	delete  editList;

	delete sndAllGates;
	delete sndTimeOver;
	delete sndProc;

	delete sndMusic;


	delete mainMenu;
}



void Controller::startServer()
{
	if(!(isServer||isClient))
	{
		players=1;
		server1 = new TCPServer( 4242, this );
		connect (server1,SIGNAL(newConnect( TCPClSocket* )),this,SLOT(sltSock1(TCPClSocket* )));
		server2 = new TCPServer( 4243, this );
		connect (server2,SIGNAL(newConnect( TCPClSocket* )),this,SLOT(sltSock2(TCPClSocket* )));
		server3 = new TCPServer( 4244, this );
		connect (server3,SIGNAL(newConnect( TCPClSocket* )),this,SLOT(sltSock3(TCPClSocket* )));
		isServer=true;
		myHost="server";

	}



}


void Controller::startClient(QString host, int player)
{
		

	if(!(isServer||isClient)&&((player==2)||(player==3)||(player==4)))
	{
		if(player==2)
		{
			Schiff1->setPos(Vektor3f(0.0f, 0.0f, 6.0f));
			cli = new TCPClient( host, 4242 );
			isClient=true;
			playerNr=player;
		
		}
		if(player==3)
		{
			Schiff1->setPos(Vektor3f(0.0f, 0.0f, -6.0f));
			cli = new TCPClient( host, 4243 );
			isClient=true;
			playerNr=player;
		}
		if(player==4)
		{
			Schiff1->setPos(Vektor3f(0.0f, 0.0f, 12.0f));
			cli = new TCPClient( host, 4244 );
			isClient=true;
			playerNr=player;
		}
		myHost=host;
		
		QObject::connect( cli, SIGNAL(criticalError()), this,SIGNAL(quit()) );
		cli->setShips(Schiff1,Schiff2,Schiff3,Schiff4);
		QObject::connect( cli, SIGNAL(serverStartsMP()), this,SLOT(serverStartsMP()));
	
		//MPG
		QObject::connect( cli, SIGNAL(serverWonMP()), this,SLOT(looseMP()));
		
	}

}

void Controller::stopServer()
{
	if(isServer)	delete server1;
	if(isServer)	delete server2;
	if(isServer)	delete server3;
	if(isServer) isServer=false;
	//MPG
	if(isServer) myHost="single";
}

void Controller::stopClient()
{

	if(isClient)	delete cli;
	if(isClient)	isClient=false;
	if(isClient)	myHost="single";
	//MPG
}




void Controller::tick(void)
{


	if (gameOver)
	{
		gameOver--;
		if (gameOver < 2)
		{
			printf("sigGAMEOVER\n");
			timer->stop();
			emit sigGameOver(myHost,playerNr,stick);
			return;
		}
	}


		if(isServer)
		{
		}
		if(isClient)
		{
			cli->updateServer();
		}

		
		
	if (menuMode)
	{
		if(mainMenu->getSetting(3)) { emit quit(); };
			//exit(1);
		
		if(mainMenu->bStart)
		{
			menuMode=false;
			Viewer->menuMode=false;
			mainMenu->bStart=false;
		}
		else
		{
		bMulti=mainMenu->getSetting(0);
		iMode=mainMenu->getSetting(1);
		if (iTrack!=mainMenu->getSetting(2))
		{
			iTrack=mainMenu->getSetting(2);
			switch (iTrack)
			{
				case 3:loadScene("courses/Course_2.szn");break;
				case 2:loadScene("courses/Course_1.szn");break;
				case 1:loadScene("courses/Course_Asteroid.szn");break;
				case 0:loadScene("courses/track1fog.szn");break;
			}
		}

		Viewer->sltMenu(mainMenu);
		emit sigUpdateViewer(viewList);
		emit sigPaint();
		}
	}
	else
	{
		SpaceObject* so = NULL;

		for (int i=1; i < (viewList->getCount()); i++)	// alle anderen Objekte updaten
		{
			
			so = viewList->getSpObj(i);
			if (so->getLife()==0) //"abgelaufene" dynamische Objekte zerstören
			{
				viewList->delItem(i);
				delete so;
			}
			else
			so->reCalc();
		};	// for

		Schiff1->reCalc(viewList);


		if(time->elapsed()>1000)
		{
			if (!editMode && !bMulti)
				gateTime--;
			roundTime++;
			time->start();
		}


		if ((gateTime<=0))
		{
			gateTime=0;
			if(!timeOver)	sndTimeOver->play();
		
			if (!gameOver) 
			{
				if(! (isServer||isClient) ) gameOver = 100;//MPG
			};
		
			if(!timeOver)		emit sigMessage(t_YouLose);
			timeOver=true;
		}

		viewList->setTime(gateTime);
		emit sigUpdateViewer(viewList);
		emit sigPaint();


		if (Viewer->jStick)
		Viewer->updateJoystick();

		   
		//printf("Time : %d\n",time->elapsed());               
	   // printf("Times: %d\n",time->second());   
	}
}




void Controller::mousePressEvent(QMouseEvent* event)
{
		printf("Mouse \n");
};


void Controller::resizeEvent(QResizeEvent* event)
{
	emit sigResize(event);
};

 
void Controller::keyPressEvent( QKeyEvent *e )
{
	//printf("Key: %d\n",(int)e->key());
	// nur erster Tastendruck, nicht bei gedrückt gehalten, wird registriert und an Canvas übermittelt
	

	if (menuMode)
	{
		if (!(e->isAutoRepeat()))
		{
			mainMenu->ikey(e->key());
		};
	
	}
	else
	{
	
		if (e->key() == Key_F7)
		{editMode=!editMode;}
		
		
		if (e->key() == Key_Escape)
		{
			timer->stop();
			emit sigGameOver(myHost,playerNr,stick);
			return;
//			menuMode=true;
//			Viewer->menuMode=true;
		};

		
		if ( !(e->isAutoRepeat()) ) { emit sigKey(e->key()); };
	
	
	
	// P = Pause
	if (e->key() == Key_P)
	{
		timer->stop();
		QDialog* mb = new QDialog(this,"pausedialog",TRUE,WStyle_Customize | WStyle_DialogBorder | WStyle_Dialog);
		mb->setCaption("[Qt]: Pause");
		QPushButton* pb =  new QPushButton("Pause.  Zum weiterspielen [Esc] oder hier drücken!",mb,"pb");
		pb->setFixedSize(400,30);
		pb->setFont(QFont( "arial",10));
		QLayout* layout = new QVBoxLayout(mb);
		layout->add(pb);
		layout->activate();
		layout->freeze();
		connect(pb, SIGNAL(released()), mb, SLOT(close()));
		mb->exec();
		delete mb;
	    timer->start( TICKRATE, FALSE );
	};
	
	if (editMode)
	{
		//Mit G wird ein neues Gate 1 an der aktuellen Schiffsposition gesetzt
		if (e->key() == Key_G)
		{
			State cState;
			cState=Schiff1->getState();
			// falls Rückwärtsflug, Orientierung des Tores umdrehen, wegen der CheckPoint-Richtung
			if (Schiff1->getSpeed()<0.0f) { cState.ori = (cState.ori*-1.0f); }
			cState.objType=o_Gate1;
			printf( "New Gate 1 set\n");
	//		cState.show();

			SpaceObject* so = new Gate(mesh_Gate1);
			so->setState(cState);

			editList->addItem( so );
		};


		//Mit H wird ein neues Gate 2 an der aktuellen Schiffsposition gesetzt
		if (e->key() == Key_H)
		{
			State cState;
			cState=Schiff1->getState();
			// falls Rückwärtsflug, Orientierung des Tores umdrehen, wegen der CheckPoint-Richtung
			if (Schiff1->getSpeed()<0.0f) { cState.ori = (cState.ori*-1.0f); }
			cState.objType=o_Gate2;
			printf( "New Gate 2 set\n");
	//		cState.show();

			SpaceObject* so = new Gate(mesh_Gate2);
			so->setState(cState);

			editList->addItem( so );
		};

		
		//Mit A wird ein neuer Asteroid#1 an der aktuellen Schiffsposition gesetzt
		if (e->key() == Key_A)
		{
			State cState;
			cState=Schiff1->getState();
			cState.objType=o_Asteroid1;
			printf( "New Asteroid1 set\n");
	//		cState.show();

			SpaceObject* so = new Asteroid(mesh_Asteroid1);
			so->setState(cState);

			editList->addItem( so );
		};


		//Mit B wird ein neuer Asteroid#2 an der aktuellen Schiffsposition gesetzt
		if (e->key() == Key_B)
		{
			State cState;
			cState=Schiff1->getState();
			cState.objType=o_Asteroid2;
			printf( "New Asteroid2 set\n");
	//		cState.show();

			SpaceObject* so = new Asteroid(mesh_Asteroid2);
			so->setState(cState);

			editList->addItem( so );
		};


		//Mit D wird ein neuer Asteroid#3 an der aktuellen Schiffsposition gesetzt
		if (e->key() == Key_D)
		{
			State cState;
			cState=Schiff1->getState();
			cState.objType=o_Asteroid3;
			printf( "New Asteroid3 set\n");
	//		cState.show();

			SpaceObject* so = new Asteroid(mesh_Asteroid3);
			so->setState(cState);

			editList->addItem( so );
		};

		
		// Mit R werden die Gates zurückgesetzt
		if (e->key() == Key_R)
		{
			viewList->resetGates();
			(firstGate->CheckPointAllowed) = true;
		};


		//  Mit T Szene speichern und testen
		if (e->key() == Key_T)
		{
			saveScene("Courses/test.szn",editList);
			loadScene("Courses/test.szn");
		};


		//  Mit L Szene laden
		if (e->key() == Key_L)
		{
			timer->stop();
			QFileDialog* dialog = new QFileDialog();
			QString name(dialog->getOpenFileName( "Courses", "Szene (*.szn)", this ) );
			if ( !(name.isEmpty()) ) { loadScene( name ); };
			delete dialog;
			timer->start( TICKRATE, FALSE );
		}

				// S = show Bounding Spheres
		if (!(e->isAutoRepeat()) && (e->key() == Key_S))
		{
			Viewer->show_BoundingSpheres = !(Viewer->show_BoundingSpheres);
		};

		// C = show Check Spheres
		if (!(e->isAutoRepeat()) && (e->key() == Key_C))
		{
			Viewer->show_CheckSpheres = !(Viewer->show_CheckSpheres);
		};


			if (!(e->isAutoRepeat()) && (e->key() == Key_3))
			{
				if (Viewer->iClouds==0)
					Viewer->iClouds=30;
				else if (Viewer->iClouds<=1900)
					Viewer->iClouds+=100;
			};

			if (!(e->isAutoRepeat()) && (e->key() == Key_5))
			{
					if (Viewer->iClouds>100)
					Viewer->iClouds-=100;
					else
					Viewer->iClouds=0;
			
			};

			if (!(e->isAutoRepeat()) && (e->key() == Key_Q))
			{
					
					Viewer->bRainbow=!Viewer->bRainbow;
			
			};

			if (!(e->isAutoRepeat()) && (e->key() == Key_Y))
			{
					
					Viewer->bFog=!Viewer->bFog;
			
			};

			if (!(e->isAutoRepeat()) && (e->key() == Key_N))
			{
					
				State cState;
				cState=Schiff1->getState();
				cState.objType=o_Nebula;
				printf( "New Nebula set\n");
		//		cState.show();

				SpaceObject* so = new Nebula();
				so->setState(cState);

				editList->addItem( so );
			}

			if (!(e->isAutoRepeat()) && (e->key() == Key_U))
			{
				if (Viewer->FogColor[0]<=1.0f)
					Viewer->FogColor[0]+=0.05f;
				else
					Viewer->FogColor[0]=0.0f;
				printf("R: %4.4f,G %4.4f, B %4.4f, A %4.4f AmbScale %4.4f DiffScale: %4.4f \n",Viewer->FogColor[0],
					Viewer->FogColor[1],Viewer->FogColor[2],Viewer->FogAlpha,Viewer->AmbScale,Viewer->DiffScale);
				
			};
			if (!(e->isAutoRepeat()) && (e->key() == Key_I))
			{
				if (Viewer->FogColor[1]<1.0f)
					Viewer->FogColor[1]+=0.05f;
				else
					Viewer->FogColor[1]=0.0f;
				printf("R: %4.4f,G %4.4f, B %4.4f, A %4.4f AmbScale %4.4f DiffScale: %4.4f \n",Viewer->FogColor[0],
					Viewer->FogColor[1],Viewer->FogColor[2],Viewer->FogAlpha,Viewer->AmbScale,Viewer->DiffScale);
				
			};
			if (!(e->isAutoRepeat()) && (e->key() == Key_O))
			{
				if (Viewer->FogColor[2]<1.0f)
					Viewer->FogColor[2]+=0.05f;
				else
					Viewer->FogColor[2]=0.0f;
					printf("R: %4.4f,G %4.4f, B %4.4f, A %4.4f AmbScale %4.4f DiffScale: %4.4f \n",Viewer->FogColor[0],
					Viewer->FogColor[1],Viewer->FogColor[2],Viewer->FogAlpha,Viewer->AmbScale,Viewer->DiffScale);
				
			};
			if (!(e->isAutoRepeat()) && (e->key() == Key_0))
			{
				if (Viewer->FogAlpha<1.0f)
					Viewer->FogAlpha+=0.05f;
				else
					Viewer->FogAlpha=0.0f;
				printf("R: %4.4f,G %4.4f, B %4.4f, A %4.4f AmbScale %4.4f DiffScale: %4.4f \n",Viewer->FogColor[0],
					Viewer->FogColor[1],Viewer->FogColor[2],Viewer->FogAlpha,Viewer->AmbScale,Viewer->DiffScale);
				
			};
				if (!(e->isAutoRepeat()) && (e->key() == Key_J))
			{
				if (Viewer->AmbScale<1.0f)
					Viewer->AmbScale+=0.05f;
				else
					Viewer->AmbScale=0.0f;
					printf("R: %4.4f,G %4.4f, B %4.4f, A %4.4f AmbScale %4.4f DiffScale: %4.4f \n",Viewer->FogColor[0],
					Viewer->FogColor[1],Viewer->FogColor[2],Viewer->FogAlpha,Viewer->AmbScale,Viewer->DiffScale);
				Viewer->AmbColor[0]=Viewer->AmbScale*Viewer->FogColor[0];
				Viewer->AmbColor[1]=Viewer->AmbScale*Viewer->FogColor[1];
				Viewer->AmbColor[2]=Viewer->AmbScale*Viewer->FogColor[2];
			};

			if (!(e->isAutoRepeat()) && (e->key() == Key_K))
			{
				if (Viewer->DiffScale<1.0f)
					Viewer->DiffScale+=0.05f;
				else
					Viewer->DiffScale=0.0f;
				printf("R: %4.4f,G %4.4f, B %4.4f, A %4.4f AmbScale: %4.4f \n",
					Viewer->FogColor[1],Viewer->FogColor[2],Viewer->FogAlpha,Viewer->AmbScale);
				Viewer->DiffColor[0]=Viewer->DiffScale*Viewer->FogColor[0];
				Viewer->DiffColor[1]=Viewer->DiffScale*Viewer->FogColor[1];
				Viewer->DiffColor[2]=Viewer->DiffScale*Viewer->FogColor[2];
			};
	} //if editmode


	// F1 = Hilfe
	if (e->key() == Key_F1)
	{
		timer->stop();
		QDialog* mb = new QDialog(this,"hilfedialog",TRUE,WStyle_Customize | WStyle_DialogBorder | WStyle_Dialog);
		mb->setCaption("[Qt]: Hilfe");
		QLabel* lab1 = new QLabel("Tastenbelegung:\n",mb,"lab1");
		QLabel* lab2 = new QLabel("Keypad 4,6, 2,8, 7,9 zur Steuerung\n +,- Geschwindigkeit einstellen\nTab = Nachbrenner\n",mb,"lab2");
		QLabel* lab3 = new QLabel("A = neuen Asteroid#1 erstellen\nB = neuen Asteroid#2 erstellen\nD = neuen Asteroid#3 erstellen\nG = neues Gate 1 erstellen\nH = neues Gate 2 erstellen\n",mb,"lab3");
		QLabel* lab4 = new QLabel("T = TestSzene speichern und laden\n\nL = Szene laden\n\nR = Gates reseten\n\nV = View umschalten\n\nS = show Bounding-Spheres\n\nC = show Check-Spheres\n\nP = Pause\n\n",mb,"lab4");
		lab1->setFont(QFont( "arial",12,QFont::Bold));
		lab2->setFont(QFont( "arial",10));
		lab3->setFont(QFont( "arial",10));
		lab4->setFont(QFont( "arial",10));
		QPushButton* pb =  new QPushButton("weiter mit [Esc]",mb,"pb");
		pb->setFont(QFont( "arial",10));
		QLayout* layout = new QVBoxLayout(mb);
		layout->add(lab1);
		layout->add(lab2);
		layout->add(lab3);
		layout->add(lab4);
		layout->add(pb);
		layout->activate();
		layout->freeze();
		connect(pb, SIGNAL(released()), mb, SLOT(close()));
		mb->exec();
		delete mb;
	    timer->start( TICKRATE, FALSE );
	};

	
	// Tab = Nachbrenner
	if (!(e->isAutoRepeat()) && (e->key() == Key_Tab))
	{
		Viewer->afterburner = true;
	};

	// V = View umschalten
	if (!(e->isAutoRepeat()) && (e->key() == Key_V))
	{
		if (Viewer->hinterherflieg) 
		{
			Viewer->hinterherflieg = false;
			Viewer->cockpitVisible = false;
		}
		else
		{
			if (Viewer->cockpitVisible)
			{
				Viewer->hinterherflieg = true;
				Viewer->cockpitVisible = false;
			}
			else
			{
				Viewer->hinterherflieg = false;
				Viewer->cockpitVisible = true;
			};
		};
	};



	if (e->key() == Key_M)
	{
		if (playingmusic) { sndMusic->stop(); }
			else { sndMusic->play(); };
		playingmusic=!playingmusic;
	};


	//m= missile
/*	if (e->key() == Key_M)
	{
		//if(Viewer->targAquired==150)
		viewList->addItem(new Missile(target,Schiff1->getPos(),Schiff1->getOri(), 0));
	};


	if (e->key() == Key_Return)
	{
		viewList->addItem(new Missile(target,Schiff1->getPos(),Schiff1->getOri(),1));
	};
 */

	if (!(e->isAutoRepeat()) && (e->key() == Key_1))
	{
		cycleTargets();
	};



	
	

	if (!(e->isAutoRepeat()) && (e->key() == Key_Z))
	{
		if (Viewer->zoomFactor<5)
			Viewer->zoomFactor++;

		else Viewer->zoomFactor=1;
	};

	


	if (e->key() == Key_F8)
	{
		mouseMode=!mouseMode;
		if (mouseMode)
			Viewer->setMouseTracking(true);
		else Viewer->setMouseTracking(false);
		Schiff1->mouseMode=mouseMode;
	};

	if (e->key() == Key_F9)
	{
		Viewer->jStick=!Viewer->jStick;
		if (stick==1) stick=0;
		else stick=1;
		

	};



	

	} //not menumode

}


void Controller::keyReleaseEvent( QKeyEvent *e )
{

	// nur letztes Taste loslassen, nicht bei gedrückt gehalten, wird registriert und an Canvas übermittelt
	if ( !(e->isAutoRepeat()) ) { emit sigKeyR(e->key()); };

	if (!(e->isAutoRepeat()) && (e->key() == Key_Tab))
	{
		Viewer->afterburner=false;
	};

}



void Controller::loadScene(const QString& name)
{
	QFile f(name);

    if ( f.open( IO_ReadOnly ) ) 
	{
		QTextStream ts( &f );
		QString s;
		QStringList line;
		printf( "Loading Szene File: %s ... \n",  (const char *)name );
		viewList->reset();

		while (!ts.eof() )	// alle Zeilen durchgehen
		{
			s = ts.readLine(); 
			line = QStringList::split(' ', s);

			// nächste Zeile einlesen, return value ist Typ des erzeugten Objekts
			// dabei Objekte erzeugen und zur ViewList, außer es ist ein Bounding-Field, dann:

			if ( (readLine(line, viewList) == o_BoundingField) && (line.count()>2) )
			{
				//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ (radius || width height length)

				// neues BF erstellen
				BoundingField* boundF;

				if (line.count()<12)	// falls weniger spalten, kugel-field
				{
					float radius = line[10].toFloat();
					boundF = new BoundingField(radius);
				}
				else	// sonst box-field
				{
					float width = line[10].toFloat();
					float height = line[11].toFloat();
					float length = line[12].toFloat();
					boundF = new BoundingField(width,height,length);
				};

				State stat;	// Status setzen
				stat.objType = line[0].toInt();
				stat.pos = Vektor3f(line[1].toFloat(), line[2].toFloat(), line[3].toFloat());
				stat.ori = Vektor3f(line[4].toFloat(),line[5].toFloat(),line[6].toFloat());
				stat.up =  Vektor3f(line[7].toFloat(), line[8].toFloat(), line[9].toFloat());
				boundF->setState(stat);


				SpaceObjectList* tmpList = new SpaceObjectList();

				int t=0;	// alle folgenden Zeilen einlesen und zum BF hinzufügen
				do
				{
					s = ts.readLine(); 
					line = QStringList::split(' ', s);
					t = readLine(line, tmpList);
				}
				while ( (t!=o_BoundingField) && (!ts.eof()) );	// solange bis wieder boundingfield-ID kommt

				for (t=0; t<tmpList->getCount(); t++)	// danach alle Objekte...
				{
					boundF->addItem( tmpList->getSpObj(t) );	// aus der TmpList dem BF hinzufügen...
					tmpList->updateItem(NULL,t);	// und alle Zeiger in der TmpList überschreiben...
				};

				delete tmpList; // ...da sonst beim delete der List unsere Objekte zerstört werden und _nix_ mehr in dem boundF steht

				viewList->addItem(boundF);

			};


		};	// while


		f.close();

		printf( "Load Szene complete!\n");	

		connectGates(viewList);
//		resetSzene();
	}
	else printf("* !! Konnte Datei %s nicht oeffnen!\n", (const char *)name);

}



void Controller::saveScene(const QString& name,SpaceObjectList* spObjLi)
{
	QFile f(name);


    if ( f.open( IO_WriteOnly ) ) 
	{

		printf( "Saving File: %s ... ",  (const char *)name );	
		QTextStream ds( &f );

		ds <<"iClouds"<<" "<< Viewer->iClouds<<" " <<"bFog" << " "<<Viewer->bFog<<" "<<"bFlash"<<" "<<Viewer->bFlash<<" "<<"fFogFar"<<" "<<Viewer->fFogFar<<"\n";
		ds <<"Colors:"<<" "<<"FogR" <<" "<<Viewer->FogColor[0]<<" "
							 "FogG" <<" "<<Viewer->FogColor[1]<<" "
							 "FogB" <<" "<<Viewer->FogColor[2]<<" "
							 "Amb"  <<" "<<Viewer->AmbScale<<" "
							 "Diff" <<" "<<Viewer->DiffScale<<"\n";
						    
		for (int i=4; i < (spObjLi->getCount()); i++)
		{
			State st = (spObjLi->getSpObj(i))->getState();
			
			ds  << st.objType << " " ;
			ds  << st.pos.x << " " <<st.pos.y<< " " <<st.pos.z<< " ";
			ds  << st.ori.x << " " <<st.ori.y<< " " <<st.ori.z<< " ";
			ds  << st.up.x << " " <<st.up.y<< " " <<st.up.z<< "\n";

		}

		f.close();
		printf( "Save complete!\n");
	}
	else printf("* !! Konnte Datei %s nicht oeffnen!\n", (const char *)name);

}



int Controller::readLine(const QStringList& line, SpaceObjectList* soList)
{

			if(line[0]=="iClouds")
			{
				Viewer->iClouds=line[1].toInt();
				Viewer->bFog=line[3].toInt();
				Viewer->bFlash=line[5].toInt();
				Viewer->fFogFar=line[7].toFloat();
				return -1;
			}
			else if (line[0]=="Colors:")
			{
				Viewer->FogColor[0]=line[2].toFloat();
				Viewer->FogColor[1]=line[4].toFloat();
				Viewer->FogColor[2]=line[6].toFloat();
				Viewer->AmbScale=line[8].toFloat();
				Viewer->AmbColor[0]=Viewer->AmbScale*Viewer->FogColor[0];
				Viewer->AmbColor[1]=Viewer->AmbScale*Viewer->FogColor[1];
				Viewer->AmbColor[2]=Viewer->AmbScale*Viewer->FogColor[2];
				Viewer->DiffScale=line[10].toFloat();
				Viewer->DiffColor[0]=Viewer->DiffScale*Viewer->FogColor[0];
				Viewer->DiffColor[1]=Viewer->DiffScale*Viewer->FogColor[1];
				Viewer->DiffColor[2]=Viewer->DiffScale*Viewer->FogColor[2];
				
				return -1;
			}
		

				State st;
				st.objType = line[0].toInt();	// (Typ 0+=Ship, 10+=Tor, 20+=Asteroid, 5=Kugel)
				int objT = st.objType;

				if ((line.count()<2) && (objT==o_BoundingField)) { return objT; };	// falls EndOf-BoundingField


				st.pos = Vektor3f(line[1].toFloat(), line[2].toFloat(), line[3].toFloat());
				st.ori = Vektor3f(line[4].toFloat(),line[5].toFloat(),line[6].toFloat());
				st.ori.normalize();
				st.up =  Vektor3f(line[7].toFloat(), line[8].toFloat(), line[9].toFloat());
				st.up.normalize();


				SpaceObject* spobj=NULL;

	/*			if (objT==o_Ship)
				{
					//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ
					spobj = new Ship();
					spobj->setState(st);
					soList->addItem(spobj);
				};	// Ship

	*/

				if ( (objT==o_Gate1) )
				{
					//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ
					spobj = new Gate(mesh_Gate1);
					spobj->setState(st);
					if ((line.count()>10) && ((line[10].left(1).upper()=="B") || (line[17].left(1).upper()=="B")))
					{
						((Gate*)spobj)->setBothSides(true);
					};
					soList->addItem(spobj);
				};	// Gate


				if ( (objT==o_Gate2) )
				{
					//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ
					spobj = new Gate(mesh_Gate2);
					spobj->setState(st);
					if ((line.count()>10) && ((line[10].left(1).upper()=="B") || (line[17].left(1).upper()=="B")))
					{
						((Gate*)spobj)->setBothSides(true);
					};
					soList->addItem(spobj);
				};	// Gate



				if ( (objT==o_Asteroid1) || (objT==o_Asteroid2) || (objT==o_Asteroid3) )
				{
					//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ
					// Type bestimmen und State setzen
					if (objT==o_Asteroid1) { spobj = new Asteroid(mesh_Asteroid1); }
					else if (objT==o_Asteroid2) { spobj = new Asteroid(mesh_Asteroid2); }
					else { spobj = new Asteroid(mesh_Asteroid3); };
					spobj->setState(st);
					soList->addItem(spobj);
				};	// Asteroids


				if (objT==o_Nebula)
				{
					spobj=new Nebula();
					spobj->setState(st);
					soList->addItem(spobj);
				}; // Nebula


				if ( objT==o_AsteroidField )
				{
					//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ density (radius || width height length) movement

					int density = line[10].toInt();	// Anzahl der Asteroiden im Feld

					float radius;	// Begrenzungen des Bounding-Fields
					float width;
					float height;
					float length;

					float zufX;		// Zufallsposition innerhalb des Fields
					float zufY;
					float zufZ;
					int zufAst;		// zufälliger Asteroiden-Typ
					int zufmov;		// Movement-Speed
					bool spherefield = (line.count()<14);	// falls weniger spalten, kugel-field


					// Unterscheidung zwischen Sphere und Box:
					if (spherefield)	// falls kugel-field
					{
						radius = line[11].toFloat(); zufmov = line[12].toInt();
					}
					else	// sonst box-field
					{
						width = line[11].toFloat(); height = line[12].toFloat(); length = line[13].toFloat(); zufmov = line[14].toInt();
					}


					// Anzahl der Sub-Objekte erzeugen
					for (int i=0; i<density; i++)
					{

						// zufälligen Typ wählen
						zufAst = rand()%3;
						if (zufAst==0) { spobj = new Asteroid(mesh_Asteroid1); }
						else if (zufAst==1) { spobj = new Asteroid(mesh_Asteroid2); }
						else { spobj = new Asteroid(mesh_Asteroid3); };
						
						float tmpBR = spobj->getBoundRad();

						// zufällige Position innerhalb des Fields finden (nur 1."Oktant")
						if (spherefield)	// falls kugel-field
						{
							float tmpR = radius-tmpBR;
							do
							{
								zufX = (((float)(rand()%1000))/1000.0f)*tmpR;
								zufY = (((float)(rand()%1000))/1000.0f)*tmpR;
								zufZ = (((float)(rand()%1000))/1000.0f)*tmpR;
							}
							while ( ((zufX*zufX)+(zufY*zufY)+(zufZ*zufZ)) > (tmpR*tmpR) );
						}
						else	// sonst box-field
						{
							zufX = ((float)(rand()%1000))/1000.0f*(width-tmpBR);
							zufY = ((float)(rand()%1000))/1000.0f*(height-tmpBR);
							zufZ = ((float)(rand()%1000))/1000.0f*(length-tmpBR);
						};

						// andere "Oktanten" der Box auch abdecken
						if ( (rand()%2)==0 ) { zufX = -zufX; };
						if ( (rand()%2)==0 ) { zufY = -zufY; };
						if ( (rand()%2)==0 ) { zufZ = -zufZ; };

						// Position innerhalb umrechnen in Weltkoordinaten
						Vektor3f zufPos(zufX,zufY,zufZ);
						zufPos = zufPos + st.pos;

						// State des neuen Asteroiden setzen
						State stA;
						stA.objType = o_Asteroid1+zufAst; // zufälliger Type
						stA.pos = zufPos;	// Position
						zufPos.normalize();
						stA.ori = zufPos;	// Orientierung zufällig nach Position (zeigt also bisher immer vom Mittelpukt weg)
						Vektor3f up(zufPos.kreuz(st.pos));	// Up steht senkrecht auf Positions- und Fieldpositions-Vektor, beliebig
						up.normalize();
						stA.up = up;
						spobj->setState(stA);

						soList->addItem(spobj);
					};	// for

				};	// AsteroidFields



				if (spobj==NULL) { return objT; };	// unbekannter objT



				if ( (line.count()>11) && (objT!=o_AsteroidField) && (objT!=o_BoundingField))	// falls feste Rot/Mov-Parameter
				{
					float tmp[3];	// %1000/60000

					for (int l=0; l<3; l++)
					{
						if ( line[l+10].left(1).upper()=="F")	// fester Wert
						{
							tmp[l] = (float)((line[l+10].right(line[l+10].length()-1)).toInt());
							tmp[l] = tmp[l]/60000;
						}
						else	// zufälliger Wert
						{
							int maxRot = line[l+10].right(line[l+10].length()-1).toInt();
							tmp[l] = (float)(rand()%maxRot);
							if ( (rand()%2)==0 ) { tmp[l] = -tmp[l]; };
							tmp[l] = tmp[l]/60000;
						};
					};
					spobj->setEigenRot(tmp[0], tmp[1], tmp[2]);


					for (l=0; l<3; l++)
					{
						if ( line[l+13].left(1).upper()=="F")	// fester Wert
						{
							tmp[l] = (line[l+13].right(line[l+13].length()-1)).toFloat();
							tmp[l] = tmp[l]/60000;
						}
						else	// zufälliger Wert
						{
							int maxMov = line[l+13].right(line[l+13].length()-1).toInt();
							tmp[l] = (float)(rand()%maxMov);
							if ( (rand()%2)==0 ) { tmp[l] = -tmp[l]; };
							tmp[l] = tmp[l]/60000;
						};
					};
					Vektor3f tmpMov(tmp[0], tmp[1], tmp[2]);
					tmpMov.normalize();

					float tmpSpeed;
					if ( line[16].left(1).upper()=="F")	// fester Wert
					{
						tmpSpeed = (float)((line[16].right(line[16].length()-1)).toInt());
						tmpSpeed = tmpSpeed/10000;
					}
					else	// zufälliger Wert
					{
						int maxSpeed = line[16].right(line[16].length()-1).toInt();
						tmpSpeed = (float)(rand()%maxSpeed);
						tmpSpeed = tmpSpeed/10000;
					};

					spobj->setMovement(tmpMov, tmpSpeed);

					
			
				};	// if line.count>10
			return objT;
		

			//SzeneFile: objNr posX posY posZ oriX oriY oriZ upX upY upZ ((F_rotX F_rotY F_rotZ) || (Z_maxRotX Z_maxRotY Z_maxRotZ)) ((F_moxX F_movY F_movZ movSpeed) || (Z_maxMovX Z_maxMovY Z_maxMovZ Z_maxMovSpeed)) 
	// Typ des gerade erzeugten Objekts zurückgeben


};	// readLine



void Controller::connectGates(SpaceObjectList* spObjLi)
{

		Gate* g = NULL;
		Gate* g2 = NULL;
		SpaceObject* so = NULL;

		for (int i=0; i < (spObjLi->getCount()); i++)
		{
			so = spObjLi->getSpObj(i);

			if ( ((so->getObjType())==o_Gate1) || ((so->getObjType())==o_Gate2) )
			{

				if (g2==NULL)	// erstes Gate in der Liste
				{
					g2=(Gate*)so;
					activeGate=g2;
					firstGate=g2;
					target=g2;
					Viewer->setFirstGate(g2);
					Viewer->setTarget(target);
				}
				else	// alle anderen
				{
					g = (Gate*)so;
					g2->setSuccGate(g);
					connect(g2,SIGNAL(sigCheckPointPassed(Gate*)),this,SLOT(sltGatePassed(Gate*)));
					g2=g;
				};
			};	// if gates


			if ((so->getObjType())==o_BoundingField)
			{

				BoundingField* bf = (BoundingField*)so;
				for (int j=0; j<bf->getCount(); j++)
				{
					so = bf->getSpObj(j);
					if ( ((so->getObjType())==o_Gate1) || ((so->getObjType())==o_Gate2) )
					{

						if (g2==NULL)	// erstes Gate in der Liste
						{
							g2=(Gate*)so;
							activeGate=g2;
							firstGate=g2;
							target=g2;
							Viewer->setFirstGate(g2);
							Viewer->setTarget(target);
						}
						else	// alle anderen
						{
							g = (Gate*)so;
							g2->setSuccGate(g);
							connect(g2,SIGNAL(sigCheckPointPassed(Gate*)),this,SLOT(sltGatePassed(Gate*)));
							g2=g;
						};
					};	// if gates

				};	// for

			};	// if boundfield


		};	// for


		
		if (g2==NULL)	// falls kein gate
		{
			printf("No Gates in Szene!!!!!");
		}
		else
		{
			// letztes Gate auch connecten
			connect(g2,SIGNAL(sigCheckPointPassed(Gate*)),this,SLOT(sltGatePassed(Gate*)));
			// gates reseten
			spObjLi->resetGates();
			// und erstes gate aktivieren
			firstGate->CheckPointAllowed = true;
		};


}



void Controller::cycleTargets()
{
	if(cycleTarg<3) cycleTarg++;
	else cycleTarg=0;

	switch (cycleTarg)
	{
		case 0:target=activeGate;break;
		case 1:target=Schiff2;break;
		case 2:target=Schiff3;break;
		case 3:target=Schiff4;break;
	}
	Viewer->setTarget(target);
}



void Controller::sltGatePassed(Gate* succGate)
{

	if(!timeOver)
	{
		printf(" Time Bonus\n");
		gateTime=20;
	}


	if ((succGate!=NULL))	// falls nicht letztes Gate
	{
		Gate* g = succGate;
		g->enableCheckPoint();
		if (target==activeGate) { target=g; Viewer->setTarget(target); };
		activeGate=g;
		sndProc->play();
	}
	else	// falls letztes Gate
	{
		printf(" \n *** Round Complete *** \n");
		printf(" Round Time: %d",roundTime);


		// falls mehrere Runden
		viewList->resetGates();
		firstGate->CheckPointAllowed = true;
		if (target==activeGate) { target=firstGate; Viewer->setTarget(target); };
		activeGate=firstGate;

			if(!timeOver)
		{
			sndAllGates->play();
			emit sigMessage(t_YouWin);

	//MPG
			if (isClient) 
			{
				cli->opCodeOut=2;
				cli->iWin=true;
				cli->updateServer();
				gameOver = 133;
				isClient=false;
				delete cli;
			}
			if (isServer)
			{	
				gameOver = 66;
				serverWon(1);
			}

					else
					if (!isClient)
						 gameOver = 133;//MPG
		}

	}


};


void Controller::sltResize(QResizeEvent* event)
{
	resize(event->size());
}

//MPG
void Controller::sltSock1(TCPClSocket* s)
{
	s->setShips(Schiff1,Schiff2,Schiff3,Schiff4);
	c2=s;
	mainMenu->client2=true;
QObject::connect( c2, SIGNAL(sigWin()), this,SLOT(clientWon()));
	

}

void Controller::sltSock2(TCPClSocket* s)
{	
	s->setShips(Schiff1,Schiff3,Schiff4,Schiff2);
	mainMenu->client3=true;
	c3=s;
	QObject::connect( c3, SIGNAL(sigWin()), this,SLOT(clientWon()));
}

void Controller::sltSock3(TCPClSocket* s)
{	
	s->setShips(Schiff1,Schiff4,Schiff2,Schiff3);
	mainMenu->client4=true;
	c4=s;
QObject::connect( c4, SIGNAL(sigWin()), this,SLOT(clientWon()));	
}

void Controller::startMPGame()
{
	if(mainMenu->client2)
	{
		c2->opCodeOut=1;
	}
	if(mainMenu->client3)
	{
		c3->opCodeOut=1;
	}
	if(mainMenu->client4)
	{
		c4->opCodeOut=1;
	}

}


void Controller::serverStartsMP()
{
	if(menuMode&&cli->go)menuMode=false;
	Viewer->menuMode=menuMode;
}

void Controller::serverWon(int m)
{
	//MPG
  
  
	if(mainMenu->client2)
	{
		c2->opCodeOut=2;
	}
	if(mainMenu->client3)
	{
		c3->opCodeOut=2;
	}
	if(mainMenu->client4)
	{
		c4->opCodeOut=2;
	}
  
}


void Controller::clientWon()
{
	//MPG
  printf("A CLIENT WON\n");
  {
	if(mainMenu->client2)
	{
		c2->opCodeOut=2;
	}
	if(mainMenu->client3)
	{
		c3->opCodeOut=2;
	}
	if(mainMenu->client4)
	{
		c4->opCodeOut=2;
	}
  }

  	emit sigMessage(t_YouLose);
	timeOver=true;
	 gameOver = 66;
	printf("reset in 2 sec\n");

}

void Controller::clientError()
{
	printf("sigGAMEOVER\n");
	isClient=false;
	delete cli;
	timer->stop();
	emit sigGameOver(myHost,playerNr,stick);
}

void Controller::looseMP()
{
	//MPG

	gameOver = 133;
	timeOver=true;
	printf("reset in 4 sec\n");
	
	emit sigMessage(t_YouLose);
		isClient=false;
		delete cli;

}


#include"controller_moc.cpp"
