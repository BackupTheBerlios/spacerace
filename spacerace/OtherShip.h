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




#if !defined(AFX_OTHERSHIP_H__350BBE00_C900_11D1_841F_00E07D974127__INCLUDED_)
#define AFX_OTHERSHIP_H__350BBE00_C900_11D1_841F_00E07D974127__INCLUDED_


#include <windows.h>
#include "SpaceObject.h"
#include "enum_names.h"

class OtherShip : public SpaceObject  
{


	public:

		OtherShip(Mesh*);
		virtual ~OtherShip();
		inline void setPos(const Vektor3f& pos)
			{myPos=pos;};

	//bool collision(const Vektor3f&,const  Vektor3f&,const  float&);


};

#endif // !defined(AFX_OTHERSHIP_H__350BBE00_C900_11D1_841F_00E07D974127__INCLUDED_)
