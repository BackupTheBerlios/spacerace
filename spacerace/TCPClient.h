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




#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_



#include <qsocket.h>
#include <qvector.h>
#include <qtextstream.h>


#include "State.h"

#include "OtherShip.h"
#include "Ship.h"

class TCPClient  : public QObject
{
  Q_OBJECT

public:
	TCPClient( const QString &host, Q_UINT16 port );
	virtual ~TCPClient();
	
	void setShips(Ship* s,OtherShip* ots,OtherShip* ots3,OtherShip* ots4);

	void updateServer();

	int opCodeIn;
	int opCodeOut;
	bool go,iWin;
	bool error;


public slots:
    void closeConnection();
    void socketReadyRead();
    void socketConnected();
    void socketConnectionClosed();
    void socketClosed();
    void socketError( int e );

private:
	QDataStream *ds;
    QSocket *socket;

	State inSt,outSt;
	OtherShip* inSh;
	OtherShip* three;
	OtherShip* four;
	Ship* outSh;
	int lostSum;

signals:
	void criticalError();
	void serverStartsMP();
	void serverWonMP();
	void sigWin();

};


#endif 
