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




#ifndef _ASTEROID_H
#define _ASTEROID_H 1

#include "Vektor3f.h"
#include "RotMatrix.h"
#include "State.h"
#include "SpaceObjectList.h"

#include "Mesh.h"

#include <qobject.h>
#include <stdio.h>

#include <qfiledialog.h> 
#include <qstring.h> 
#include <qtextstream.h>
#include <qstringlist.h>
#include <vector>
#include <math.h>
#include <windows.h>

#include <qgl.h>            // Header File For The QT OpenGL32 Library

#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library


class Asteroid :public SpaceObject
{


	Q_OBJECT


	public:

		Asteroid(Mesh*);
		virtual ~Asteroid();



	private:



	public slots:


	signals:


};


#endif 