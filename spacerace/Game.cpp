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




#include "Game.h"



Game::Game(QMainWindow * parent, const char * name, WFlags f)
:QWidget(parent,name,f)
{
	
	setBackgroundColor(QColor(0,0,0));	// paint it black
	GController = NULL;
	IController = NULL;
	loading = NULL;
	
	IController = new IntroController( "","",this, "IC" );
	QObject::connect( IController, SIGNAL(quit()), this, SLOT(endIntro()) );
	QObject::connect (this,SIGNAL(sigResize(QResizeEvent*)),IController, SLOT(sltResize(QResizeEvent*)));
	
	//	endIntro();
	
	myHost="single";
	myPl=0;
	stick=0;
}



Game::~Game()
{
	delete IController;
	delete GController;
	delete loading;
}



void Game::endIntro()
{
	printf("------------- INTRO END ----------------\n");
	
	if (GController == NULL)
	{
		
		QImage img("texturen/loading.bmp");
		img = img.smoothScale(width(),height());
		QPixmap pm;
		pm.convertFromImage(img);
		loading = new QLabel(this,"loadinglabel");
		loading->setBackgroundColor(QColor(0,0,0));
		loading->setAlignment( Qt::AlignCenter );
		loading->resize(width(),height());
		loading->setPixmap(pm);
		loading->show();
		
		delete IController;
		IController = NULL;
		
		myHost="single";
		stick=0;
		printf("GAME Mode: %s\n",(const char*)myHost);
		//MPG
		GController = new Controller( 	myHost,myPl,stick, this, "GC" );
		QObject::connect( GController, SIGNAL(quit()), this, SIGNAL(quit()) );
		QObject::connect (this,SIGNAL(sigResize(QResizeEvent*)),GController, SLOT(sltResize(QResizeEvent*)));
		//MPG
		QObject::connect (GController,SIGNAL(sigGameOver(QString,int,int)),this, SLOT(sltResetController(QString,int,int)));
		
		GController->resize(width(),height());
		GController->show();
		
		//		delete loading;
		//		loading=NULL;
	};
	
	
};



void Game::sltResetController(QString host,int p,int st)
{
	printf("------------- CONTROLLER RESET ----------------\n");
	
	myHost=host;myPl=p;stick=st;
	if (loading!=NULL) { delete loading; };
	
	QImage img("texturen/loading.bmp");
	img = img.smoothScale(width()*0.80f,height()*0.80f);
	img = img.smoothScale(width(),height());
	QPixmap pm;
	pm.convertFromImage(img);
	loading = new QLabel(this,"loadinglabel");
	loading->setBackgroundColor(QColor(0,0,0));
	loading->setAlignment( Qt::AlignCenter );
	loading->resize(width(),height());
	loading->setPixmap(pm);
	loading->show();
	
	
	delete GController;
	
	printf("GAME Mode: %s\n",(const char*)myHost);
	GController = new Controller( 	myHost,myPl,stick, this, "GC" );
	
	
	QObject::connect( GController, SIGNAL(quit()), this, SIGNAL(quit()) );
	QObject::connect (this,SIGNAL(sigResize(QResizeEvent*)),GController, SLOT(sltResize(QResizeEvent*)));
	//MPG
	QObject::connect (GController,SIGNAL(sigGameOver(QString,int,int)),this, SLOT(sltResetController(QString,int,int)));
	
	GController->resize(width(),height());
	GController->show();
	
};


void Game::resizeEvent(QResizeEvent* event)
{
	resize(event->size());
	emit sigResize(event);
};


#include"game_moc.cpp"
