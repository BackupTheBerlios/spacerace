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



#ifndef _ENUMNAMES_H
#define _ENUMNAMES_H 1


enum EnumObjects
{
	o_Ship = 0,
	o_OtherShip = 1,
	o_Cube = 4,
	o_Kugel = 5,
	o_Skybox = 6,
	o_Szene = 7,
	o_Flare=8,
	o_BoundingField = 9,
	o_Gate1 = 10, o_Gate2 = 11,
	o_Asteroid1 = 20, o_Asteroid2 = 21, o_Asteroid3 = 22, o_AsteroidField = 29,
	o_Missile=30, o_Laser=31, d_LaserBox=32,
	o_Alex = 33,
	o_Benni = 34,
	o_Mark = 35,
	o_Title = 36,
	o_Present = 37,
	o_Cockpit=38,
	o_Thruster=39,
	o_Nebula=40,

};


enum EnumTextures
{
	t_Cockpit,
	t_Stars,
	t_Numbers,
	t_Metal,
	t_Earth,
	t_Nebula,
	t_Sun,
	t_Rock1,
	t_Rock2,
	t_Rock3,
	t_Flare,
	t_Halo,
	t_Ship,
	t_Missile,
	t_MenuBase,
	t_fog,
	t_Env,
	t_PolyCockpit,
	t_Pane,
	t_Title,
	t_YouWin,
	t_YouLose,
	t_Alex,
	t_Benni,
	t_Mark,
	t_Present
 };

const unsigned int TICKRATE=30;
const unsigned int MAX_SPEED=1000;
;

#endif
