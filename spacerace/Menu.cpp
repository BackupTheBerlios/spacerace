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



#include "Menu.h"


Menu::Menu()
{

	maxMenuPoints=5;
	menuPoint=0;
	bStart=false;
	oldMenuPoint=menuPoint;

	for (int i=0; i<=9; i++)
	{settings[i]=0;}
	numberMode=false;

	nameState=false;
 	client2=false;
	client3=false;
	client4=false;
connected=false;

}

Menu::~Menu()
{

}

void Menu::ikey( int key)
{
	if (!numberMode)
	{

		if (key==Key_8)
			cycleMenuPoints(8);
		else if (key==Key_2)
			cycleMenuPoints(2);
		else if (key==Key_4)
			cycleMenuPoints(4);
		else if (key==Key_6)
			cycleMenuPoints(6);
		

		if (key==Key_Return)
		{
			switch (menuPoint)
			{
				case 0: if (settings[0]==2)        //mehrspieler?
							settings[0]=0;
						else settings[0]=settings[0]+1;
						break;


				case 1: if (settings[1])        //mode
							settings[1]=0;
						else settings[1]=1;
						break;

				case 2: if (settings[2]==3)        //track
							settings[2]=0;
						else settings[2]=settings[2]+1;
						break;

				case 3: if (settings[3])        //exit
							settings[3]=0;
						else settings[3]=1;
						break;

				case 4: if (settings[0]==0) 
							{bStart=true;}
						else
						{
							if (settings[0]==1) 
							{
								emit startServer();
								menuPoint=5;
							}
							else
							{
								host="134.2.176.122";
								player="";
								menuPoint=6;
								nameState=true;
								numberMode=true;
							}
						}
						break;

				case 5: if (client2||client3||client4) 
						{
							bStart=true;
							emit startMPGame();
						}
						else
						{
							menuPoint=0;
							emit stopServer();
						}	
						
						break;

				case 6: {
							emit startClient(host,player.toInt());
							connected=true;
						}	
						break;

			}//switch
		}//if (key==Key_Return)

	}
	else//numberMode
	{
		if (key==Key_Return)
		{
			if(nameState)
			{		
				nameState=false;
				return;
			}
			else
			{
			numberMode=false;
			return;
			}
		}


		if (key==Key_Backspace&&nameState)
		{
			host.remove(host.length()-1,1);
			printf("host: %s\n",(const char *)host);
			return;
		}

		
		if(nameState)
		{
			char inp=key;
			host.append(inp);
			printf("host: %s\n",(const char *)host);
		}
		else
		{
			char inp=key;
			player.append(inp);
			printf("player: %s\n",(const char *)player);
			numberMode=false;
			return;

		}
	}//else


};

void Menu::cycleMenuPoints(int direction)
{
	
	
	if (direction==4)
	{
		switch (menuPoint)
		{
			case 0: oldMenuPoint=0;break;
			case 1: menuPoint=4;oldMenuPoint=1;break;
			case 2: menuPoint=4;oldMenuPoint=2;break;
			case 3: oldMenuPoint=3;break;
			case 4: if (oldMenuPoint==1 || oldMenuPoint==0)
						menuPoint=0;
					else menuPoint=3;break;
		}
	}
	else if (direction==6) 
	{
		switch (menuPoint)
		{
			case 0: menuPoint=4;oldMenuPoint=0;break;
			case 1: oldMenuPoint=1;break;
			case 2: oldMenuPoint=2;break;
			case 3: menuPoint=4;oldMenuPoint=3;break; 
			case 4: if (oldMenuPoint==2 || oldMenuPoint==3)
						menuPoint=2;
					else menuPoint=1;break;
		}
	}
	else if (direction==8) 
	{
		switch (menuPoint)
		{
			case 2: menuPoint=1;break;
			case 3: menuPoint=0;break; 
			case 4: menuPoint=0;break;
		}
	}
	else if (direction==2) 
	{
		switch (menuPoint)
		{
			case 0: menuPoint=3;break;
			case 1: menuPoint=2;break; 
			case 4: menuPoint=3;break;
		}
	}
	
};

#include "menu_moc.cpp"