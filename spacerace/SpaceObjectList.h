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





#ifndef _SPACEOBJECTLIST_H_
#define _SPACEOBJECTLIST_H_

#include "SpaceObject.h"
#include "State.h"
#include <vector>

#include "enum_names.h"


class SpaceObjectList
{
	
public:
	SpaceObjectList();
	virtual ~SpaceObjectList();
	
	std::vector<SpaceObject*> list;
	
	inline void addItem( SpaceObject* s) { list.push_back(s); }
	inline void updateItem( SpaceObject* s, const int& nr)	{ list[nr]=s; }	
	inline SpaceObject* getSpObj(const int& nr)const	 { return list[nr]; }
	
	void reset();
	inline void setTime(const int& t)	{ myTime=t; }
	inline int getTime() const	{ return myTime; }
	inline void delItem(const int& i) 
	{	
		printf("Dynamisches Objekt %d gelöscht!\n",i);
		list.erase(list.begin()+i); 
	}
	
	void show() ;
	
	
	inline int getCount() const { return list.size(); }
	
	//bool allGatesFinished();	// nicht mehr benötigt
	void resetGates();
	
	
	
private:
	
	
	int myTime;
	
};

#endif 