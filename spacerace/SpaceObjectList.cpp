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




#include "SpaceObjectList.h"
#include "BoundingField.h"



SpaceObjectList::SpaceObjectList()
{
	list.clear();
	myTime=0;	
}



void SpaceObjectList::reset()
{

	printf("SpObjList: reset! ... ");

	//save ships
	std::vector<SpaceObject*> shipList;
	for (int s=0; (s<4)&&(s<list.size()); s++)
	{
		shipList.push_back(list[s])	;	
	};	// for i

	
	int delC=0;
	for (int i=4; i<list.size(); i++)
	{
		delete list[i]	;
		delC++;
	};	// for i
	list.clear();


	//restore ships
	for ( i=0; (i<4)&&(i<s); i++)
	{
		list.push_back(shipList[i]);
	};	// for i

	printf("SpaceObjectList: %d SpO deleted\n",delC);

}




void SpaceObjectList::show() 
{
	printf( "____________Liste-Show_________\n");
	for (int i=0; i<list.size(); i++)
	{
		State st=list[i]->getState();
		printf( "Object[%d]=%d\n",i,st.objType);
	
	};	// for i
}



SpaceObjectList::~SpaceObjectList()
{
	int delC=0;
	for (int i=4; i<list.size(); i++)
	{
		delete list[i]	;		
		delC++;
	};	// for i
	list.clear();

	printf("SpaceObjectList Destructor:  %d SpO deleted\n",delC);
}




void SpaceObjectList::resetGates()
{

	for (int i=0; i<list.size(); i++)
	{
		int oT = list[i]->getObjType();

		if ( (oT==o_Gate1) || (oT==o_Gate2) )		// if Obj is Gate
		{
			list[i]->CheckPointPassed=false;
			list[i]->CheckPointAllowed=false;
		};

		if ( (oT==o_BoundingField) )
		{
			((BoundingField*)list[i])->resetGates();
		};
			
	};	// for i

	printf(" SOL: Gates reseted\n");
}
