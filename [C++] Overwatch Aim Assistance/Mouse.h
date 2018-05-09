/*
 *Overwatch Aim Assitance
 *Copyright (C) 2016  Juan Xuereb
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.

 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef MOUSE_H
#define MOUSE_H

#include <Windows.h>
#include <iostream>

class Mouse
{
private:
	const float constant = 0.116f; //0.090 seems to work better sometimes
	float mouseSensitivity = 15.0f; //found in game menu
	float modifier = 0.0f; //modifier = sens*const. Calculated on init()
	
	bool suspendThreads = false;
	int threads = 0;

	int screenWidth, screenHeight;
	int centreScreenX, centreScreenY;
	void calibrateCoordinates(int &x, int &y); //Returns screen coords based on sensitivity and modifier.

public:
	Mouse(int captureWidth, int captureHeight, const float sensitivity);

	void terminateThreads();
	void moveTo(int x, int y);
	void moveSmooth(int x, int y, int moveSpeed);
	void click(int ms);

};

#endif // !MOUSE_H
