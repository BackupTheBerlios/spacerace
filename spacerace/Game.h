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



#ifndef _GAME_H_
#define _GAME_H_

#include <qwidget.h>
#include <qmainwindow.h>
#include <qimage.h>
#include <qlabel.h>

#include "controller.h"
#include "introcontroller.h"


class Game:public QWidget
{

	Q_OBJECT



	public:
		Game(QMainWindow * parent, const char * name=0, WFlags f=0);
		virtual ~Game();


	protected:
		void resizeEvent(QResizeEvent*);


	private:
		Controller* GController;
		IntroController* IController;
		QLabel* loading;
		QString myHost;
		int myPl;
		int stick;

	public slots:
		void endIntro();
		void sltResetController(QString,int,int);



	signals:
		void sigResize(QResizeEvent* event);
		void quit();


};


#endif