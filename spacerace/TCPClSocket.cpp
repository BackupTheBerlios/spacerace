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




#include "TCPClSocket.h"
#include "State.h"


TCPClSocket::TCPClSocket(int sock, QObject *parent, const char *name):
        QSocket( parent, name )
{
	connect( this, SIGNAL(readyRead()),SLOT(readClient()) );
	setSocket( sock );
	ds=new QDataStream ( this );
		lostSum=0;
	opCodeOut=0;
	won=false;
}

TCPClSocket::~TCPClSocket()
{
	printf("Closing ClientSocket: Packets lost during connection: %d\n",lostSum);
	delete ds;
}


void TCPClSocket::setShips(Ship* s,OtherShip* ots)
{
	inSh=ots;
	outSh=s;
	
}

void TCPClSocket::setShips(Ship* s,OtherShip* ots,OtherShip* ots3,OtherShip* ots4)
{
	inSh=ots;
	outSh=s;
	three=ots3;
	four=ots4;
}

void TCPClSocket::readClient()
{

	::State outSt;
	::State inSt;

	inSt.objType=1;
	
	int lost= -1;
	
	while(!ds->atEnd ())
	{
		lost++;

		(*ds)  >> opCodeIn
		  >> inSt.pos.x >> inSt.pos.y >> inSt.pos.z
		  >> inSt.ori.x >> inSt.ori.y >> inSt.ori.z
		  >> inSt.up.x  >> inSt.up.y  >> inSt.up.z ;	
	}
	if (lost>0)
	{
		lostSum+=lost;
		printf("Client: %d Packets lost - sum: %d \n",lost,lostSum);
	}

	inSh->setState(inSt);

	outSt=outSh->getState();
	(*ds)  << opCodeOut
	  << outSt.pos.x << outSt.pos.y << outSt.pos.z
	  << outSt.ori.x << outSt.ori.y << outSt.ori.z
	  << outSt.up.x  << outSt.up.y  << outSt.up.z ;
	
	outSt=three->getState();
	(*ds)  << opCodeOut
	  << outSt.pos.x << outSt.pos.y << outSt.pos.z
	  << outSt.ori.x << outSt.ori.y << outSt.ori.z
	  << outSt.up.x  << outSt.up.y  << outSt.up.z ;

	outSt=four->getState();
	(*ds)  << opCodeOut
	  << outSt.pos.x << outSt.pos.y << outSt.pos.z
	  << outSt.ori.x << outSt.ori.y << outSt.ori.z
	  << outSt.up.x  << outSt.up.y  << outSt.up.z ;

	this->flush(); 

	//MPG
	if ((opCodeIn==2)&&!won)
	{
		emit sigWin();
			won=true;
		printf("Client won \n");
	}

}


#include "TCPClSocket_moc.cpp"