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



#include "TCPServer.h"
#include <stdio.h>


TCPServer::TCPServer(Q_UINT16 port,QObject* parent):
QServerSocket( port, 1, parent )
{
    if ( !ok() ) 
	{
		
		printf("Failed to bind to port: %d\n",port);
		
    }
	printf("New server startet. Listening  port: %d\n",port);
	inUse=false;
}

TCPServer::~TCPServer()
{
	printf("Closing Server\n");
}

void TCPServer::newConnection( int socket )
{
	if (!inUse)
	{
        TCPClSocket *s = new TCPClSocket( socket, this );
        emit newConnect( s );
		printf("================ New Client on %d ============\n", socket);
		inUse=true;
	}
}

#include "TCPServer_moc.cpp"