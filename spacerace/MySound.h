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




#if !defined(AFX_MYSOUND_H__7F3697C5_162C_4107_9323_FB6601776653__INCLUDED_)
#define AFX_MYSOUND_H__7F3697C5_162C_4107_9323_FB6601776653__INCLUDED_


#include <stdio.h>
#include <windows.h>
#include <conio.h>

#include "fmod/fmoddyn.h"
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"    /* optional */


#define FMOD_LIB  "fmod.dll"


#define FMOD_LIB_PATH  "fmod/" ## FMOD_LIB

#include <qstring.h>


class MySound  
{
public:

	MySound(const QString&);
	MySound(const QString&, int);
	virtual ~MySound();
	void play();
	void stopAll();
	void stop();


private:

	bool effect;
    FMOD_INSTANCE *fmod1;
    FSOUND_SAMPLE *samp1;
	FSOUND_STREAM *stream;

	bool initOK;

};

#endif // !defined(AFX_MYSOUND_H__7F3697C5_162C_4107_9323_FB6601776653__INCLUDED_)
