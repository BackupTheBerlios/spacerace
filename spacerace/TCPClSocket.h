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




#ifndef _TCPCLSOCKET_H_
#define _TCPCLSOCKET_H_

#include <qsocket.h>
#include <qdatastream.h>
#include "Vektor3f.h"
#include "State.h"
#include "OtherShip.h"
#include "Ship.h"

class TCPClSocket  : public QSocket
{
    Q_OBJECT
		
public:
	TCPClSocket(int sock, QObject *parent=0, const char *name=0);
	virtual ~TCPClSocket();
	
	//	inline void setVal(::State s){outSt=s;}
	void setShips( Ship* s, OtherShip* ots);
	void setShips(Ship* s,OtherShip* ots,OtherShip* ots3,OtherShip* ots4);
	
	int opCodeIn;
	int opCodeOut;
	bool won;
	
	
	//	inline ::State getClientState(){return inSt;}
	
private slots:
	void readClient();
	
signals:
	void sigWin();
	
private:
	QDataStream *ds;
	OtherShip* inSh;
	OtherShip* three;
	OtherShip* four;
	Ship* outSh;
	int lostSum;
	
};

#endif 


