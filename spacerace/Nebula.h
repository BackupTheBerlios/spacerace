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



#ifndef _NEBULA_H
#define _NEBULA_H 1

#include "SpaceObject.h"
#include <stdlib.h>



class Nebula : public SpaceObject  
{


public:
	Nebula();
	virtual ~Nebula();
	virtual inline Vektor3f getPartPos(int i) const {return pos[i];}
	virtual inline float getPartOpac(int i) const {return opac[i];}
	virtual inline void setPartOpac(int i, float o) {opac[i]=0;}

protected:
	
	Vektor3f pos[200];
	float opac[200];

};

#endif 
