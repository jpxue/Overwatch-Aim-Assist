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

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <Windows.h>
#include <iostream>
#include <vector>
#include <algorithm> //sort

using namespace std;

struct myRGB
{
	BYTE r, g, b;
	myRGB(BYTE R, BYTE G, BYTE B)
	{
		r = R;
		g = G;
		b = B;
	}
};
const myRGB HEALTH = { 255, 0, 19 }; //Overwatch Health Bar RGB Values
const myRGB ARMOR = { 0, 0, 0 }; //CBA to find these
const myRGB SHIELD = { 0, 0, 0 }; //CBA to find these

class Midline
{
	friend class Screenshot;
private:
	int length;
public:
	Midline(Screenshot &sc);
	Midline(int sWidth, int sHeight, RGBQUAD *sPixels);
	~Midline();
	RGBQUAD *pixels = NULL;
};


class Screenshot
{
	friend class Capture;
	friend class Midline;
private:
	const bool DRAW = false; //Draws filtered image to console
	int length; //IMP: Needs to be defined as there is no-way to find length of *pixels just from pointer to 1st element
	int width, height;
	RGBQUAD *pixels = NULL; //IMP: Uses GetDIBits to obtain RGBQUADs => image is inverted. Visualise via drawScreenshot(false)

	bool isHealth(RGBQUAD &pixel);
	bool isRed(RGBQUAD &pixel);
	bool isScreenyBlack();
	void debugDraw(int handleX, int handleY, int healthBarWidth, vector<POINT> border, int aimX, int aimY);
	int calculateMedian(vector<int> &values);
	bool isRGBEqual(RGBQUAD &r1, RGBQUAD &r2);

public:
	Screenshot();
	~Screenshot();

	void FreeMemory(); //Do not handle mem on destruction since we sometimes want mem region to persist
	
	//Drawing for debugging purposes
	void drawReds(bool invert);
	void drawPixel(int x, int y, COLORREF col);
	void drawScreenshot(bool invert, bool pixelated);
	void drawBlankScreenshot(bool pixelated);

	bool findPlayer(int &x, int &y, bool headshot);
	bool triggerBot();

	Screenshot& operator=(const Screenshot &other); //Does not copy RGBQUAD* put points to it only!!!
	bool operator==(const Midline &line); //Faster then comparing whole screenshot. Takes 0.000ms
	bool operator!=(const Midline &line); // !^
	bool operator==(const Screenshot &other); //Slower than comparing only midline. Takes 0.004ms
	bool operator!=(const Screenshot &other); // !^

};


#endif // !SCREENSHOT_H