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




#include "MySound.h"


MySound::MySound(const QString& name)
{
	initOK=true;

	effect=true;

	printf("Loading Sound Effect: %s\n",(const char *)name);

}


MySound::MySound(const QString& name,int volMus)
{
	initOK=true;
	effect=false;


}


 
MySound::~MySound()
{
 
}


void MySound::play()
{
	printf("Tadaaa!\n");
}


void MySound::stopAll()
{
	if (!initOK) { return; };
}

void MySound::stop()
{
	if (!initOK) { return; };
}
