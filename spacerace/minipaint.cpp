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



#include<qapplication.h>
#include<qpushbutton.h>
#include<qmainwindow.h>
#include<qmenubar.h>
#include<qpopupmenu.h>



#include <stdio.h>
#include "game.h"

#include <math.h>

#include <stdlib.h>
#include <time.h>



int main(int argc, char** argv)
{
	srand( (unsigned)time( NULL ) );	// Zufallsgenerator init

	QApplication qapp(argc, argv);
	QMainWindow qmainwindow(0,0);
	qmainwindow.resize(800, 600);
	Game GameController( &qmainwindow );

	QObject::connect( &GameController, SIGNAL(quit()), &qapp,SLOT(quit()) );
	

	QMenuBar* qmenubar=qmainwindow.menuBar();

	// Menue "File" 
/*	QPopupMenu qmenu_file(qmenubar, "MenuFile");
	qmenu_file.insertItem("Quit", &qapp, SLOT(quit()));
	qmenu_file.insertItem("Load", GameController, SLOT(sltLoad()));
	qmenubar->insertItem("&File", &qmenu_file);
*/


//--------------- T E S T S-------------------------------------

/*--------------------------------------*/

	qmainwindow.setCaption("[Qt:] Space Race");
	qmainwindow.setCentralWidget(&GameController);
	qapp.setMainWidget( &qmainwindow );
	qmainwindow.show();


	return (qapp.exec());
}

