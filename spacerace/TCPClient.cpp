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




#include "TCPClient.h"
#include <stdio.h>



TCPClient::TCPClient( const QString &host, Q_UINT16 port )
{
    socket = new QSocket( this );
    connect( socket, SIGNAL(connected()),SLOT(socketConnected()) );
    connect( socket, SIGNAL(connectionClosed()),SLOT(socketConnectionClosed()) );
    connect( socket, SIGNAL(readyRead()),SLOT(socketReadyRead()) );
    connect( socket, SIGNAL(error(int)),SLOT(socketError(int)) );

    // connect to the server
    printf("Trying to connect to the server %s:%d\n",(const char*)host,port);
	
    socket->connectToHost( host, port );
	ds=NULL;
	ds=new QDataStream ( socket );
	lostSum=0;
	go=false;
	iWin=false;
	error=false;
}

TCPClient::~TCPClient()
{
	printf("Closing Client: Packets lost during connection: %d\n",lostSum);
	delete ds;
}

void TCPClient::closeConnection()
{
    socket->close();
    if ( socket->state() == QSocket::Closing ) {
        // We have a delayed close.
        connect( socket, SIGNAL(delayedCloseFinished()),
                SLOT(socketClosed()) );
    } else {
        // The socket is closed.
        socketClosed();
    }
}



void TCPClient::updateServer()
{
	if (!error)
	{
		outSt=outSh->getState();
		(*ds) << opCodeOut
		<< outSt.pos.x << outSt.pos.y << outSt.pos.z
		<< outSt.ori.x << outSt.ori.y << outSt.ori.z
		<< outSt.up.x  << outSt.up.y  << outSt.up.z ;
		socket->flush(); 
	}
}



void TCPClient::setShips(Ship* s,OtherShip* ots,OtherShip* ots3,OtherShip* ots4)
{
	if (!error)
	{
		inSh=ots;
		outSh=s;
		three=ots3;
		four=ots4;
	}
}


void TCPClient::socketReadyRead()
{
	// read from the server

	
	if (!error)
	{
		int lost= -1;
		State tre,fur;
		fur.objType=1;
		tre.objType=1;
		inSt.objType=1;
		
		while(!ds->atEnd ())
		{
			lost++;
			(*ds)   >> opCodeIn 
			>> inSt.pos.x >> inSt.pos.y >> inSt.pos.z
			>> inSt.ori.x >> inSt.ori.y >> inSt.ori.z
			>> inSt.up.x  >> inSt.up.y  >> inSt.up.z ;	

			(*ds)   >> opCodeIn 
			>> tre.pos.x >> tre.pos.y >> tre.pos.z
			>> tre.ori.x >> tre.ori.y >> tre.ori.z
			>> tre.up.x  >> tre.up.y  >> tre.up.z ;	

			(*ds)   >> opCodeIn 
			>> fur.pos.x >> fur.pos.y >> fur.pos.z
			>> fur.ori.x >> fur.ori.y >> fur.ori.z
			>> fur.up.x  >> fur.up.y  >> fur.up.z ;	
		}
		if (lost>0)
		{
			lostSum+=lost;
			printf("Client: %d Packets lost - sum: %d \n",lost,lostSum);
		}
		

   		inSh ->setState(inSt);
		three->setState(tre);
		four ->setState(fur);


	//	printf("%d\n",opCodeIn);
		if((opCodeIn==1)&&(!go)) 
		{
			go=true;
			emit serverStartsMP();
			printf("serverStartsMP\n");
		}

		if((opCodeIn==2)&&(!iWin)) 
		{
			emit serverWonMP();
			printf("serverWonMP\n");
		}


	}

}



void TCPClient::socketConnected()
{
	printf( "========= Connected to server ===============\n" );
}

void TCPClient::socketConnectionClosed()
{
	printf( "Connection closed by the server\n" );
}

void TCPClient::socketClosed()
{
	printf( "Connection closed\n" );
}

void TCPClient::socketError( int e )
{
	printf( "Error number %d occurred\n" ,e);
	error=true;
	emit 	criticalError();
}



#include "TCPClient_moc.cpp"
