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
	stream=NULL;

    fmod1 = FMOD_CreateInstance(FMOD_LIB_PATH);


    if (!fmod1)
    {
        printf("Error : Cannot find %s!\n", FMOD_LIB);
		initOK=false;
    }


    if (fmod1->FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		initOK=false;
    }


    fmod1->FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND); 
    fmod1->FSOUND_SetDriver(0);                    /* Select sound card (0 = default) */

  
    /*
        INITIALIZE
    */
    if (!fmod1->FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(fmod1->FSOUND_GetError()));
		initOK=false;
    }

   
	printf("Loading Sound Effect: %s\n",(const char *)name);
    samp1 = fmod1->FSOUND_Sample_Load(FSOUND_UNMANAGED, name, FSOUND_NORMAL | FSOUND_2D, 0);
    if (!samp1)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(fmod1->FSOUND_GetError()));
		initOK=false;
    }
    fmod1->FSOUND_Sample_SetMode(samp1, FSOUND_LOOP_OFF);  /* this wav has loop points in it which turns looping on.. turn it off! */
}


MySound::MySound(const QString& name,int volMus)
{
	initOK=true;
	effect=false;

	samp1=NULL;

    fmod1 = FMOD_CreateInstance(FMOD_LIB_PATH);


    if (!fmod1)
    {
        printf("Error : Cannot find %s!\n", FMOD_LIB);
		initOK=false;
    }


    if (fmod1->FSOUND_GetVersion() < FMOD_VERSION)
    {
        printf("Error : You are using the wrong DLL version!  You should be using FMOD %.02f\n", FMOD_VERSION);
		initOK=false;
    }


    fmod1->FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND); 
    fmod1->FSOUND_SetDriver(0);                    /* Select sound card (0 = default) */

  
    /*
        INITIALIZE
    */
    if (!fmod1->FSOUND_Init(44100, 32, FSOUND_INIT_USEDEFAULTMIDISYNTH))
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(fmod1->FSOUND_GetError()));
		initOK=false;
    }

	fmod1->FSOUND_Stream_SetBufferSize(1000);

	printf("Loading Music: %s\n",(const char *)name);

	stream =fmod1->FSOUND_Stream_OpenFile(name,FSOUND_16BITS | FSOUND_STEREO,0);
	

    if (!stream)
    {
        printf("Error!\n");
        printf("%s\n", FMOD_ErrorString(fmod1->FSOUND_GetError()));
        fmod1->FSOUND_Close();
		initOK=false;
    }


}


 
MySound::~MySound()
{
    /*
        CLEANUP AND SHUTDOWN
    */

    if (effect) fmod1->FSOUND_Sample_Free(samp1);
		else fmod1->FSOUND_Stream_Close(stream);

    fmod1->FSOUND_Close();
    FMOD_FreeInstance(fmod1);
//	printf("Sound freed\n");
}


void MySound::play()
{
	if (!initOK) { return; };

	if (effect)
	{		
		fmod1->FSOUND_PlaySound(FSOUND_FREE, samp1);
	}
	else
		fmod1->FSOUND_Stream_Play(FSOUND_FREE, stream);
//	printf("Tadaaa!\n");
}


void MySound::stopAll()
{
	if (!initOK) { return; };
	fmod1->FSOUND_StopSound(FSOUND_ALL);
//	printf("Silence!\n");
}

void MySound::stop()
{
	if (!initOK) { return; };
	if (!effect)fmod1->FSOUND_Stream_Stop(stream);
//	printf("Stop song\n");
}
