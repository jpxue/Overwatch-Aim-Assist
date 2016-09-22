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
#ifndef CAPTURE_H
#define CAPTURE_H

#include <Windows.h>
#include "Screenshot.h"

// Takes a screenshot and saves it inside a Screenshot class lal //
class Capture
{
	friend class Screenshot;
private:
	char *WindowName; //Handle name of window
	void release(HWND &hwnd, HDC &hdc, HDC &captureDC, HBITMAP &hBmp);

public:
	Capture(char *windowName);
	~Capture();

	bool isWindowRunning();
	bool screenshotGDI(Screenshot &screeny); //Obtains screenshot via win32. Outputs data into _LastFrame struct
	int getWidth();
	int getHeight();
	void switchToWindow();
	bool captureNewFrame(Screenshot &currentFrame);
};


#endif 
