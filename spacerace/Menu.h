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




#ifndef MENU_H
#define MENU_H 1

#include <qobject.h>
#include <qstring.h>
#include <stdio.h>

class Menu : public QObject  
{
	
	Q_OBJECT
public:
	Menu();
	virtual ~Menu();
	void cycleMenuPoints(int);
	void ikey(int);
	inline int getMenuPoint() const {return menuPoint;};
	inline int getSetting(int i) const {return settings[i];}
	int menuPoint,oldMenuPoint;
	int maxMenuPoints;
	int settings[9];
	int bStart;
	bool numberMode;
	//	int otherPlayer2,otherPlayer3,otherPlayer4;
	QString player;
	QString host;
	bool client2,client3,client4,connected;
	
	bool nameState;
	
signals:
	void startClient( QString, int);
	void startServer();
	void stopClient();
	void stopServer();
	
	void startMPGame();
	
};

#endif 
