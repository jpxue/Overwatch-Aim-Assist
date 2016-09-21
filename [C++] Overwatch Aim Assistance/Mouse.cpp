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

#include "Mouse.h"

bool dynamicScaling = false;

int aimSpeed = 7;
int maxDistance = 2500;
int minDistance = 250;

Mouse::Mouse(int captureWidth, int captureHeight, const float sensitivity)
{
	screenWidth = captureWidth;
	screenHeight = captureHeight;
	centreScreenX = captureWidth / 2;
	centreScreenY = captureHeight / 2;
	mouseSensitivity = sensitivity;

	widthTol = (0.5f * (float)screenWidth) / 100.0f; //% of screen widtht
	heightTol = (0.5f * (float)screenHeight) / 100.0f; //% of screen height

	modifier = mouseSensitivity*constant;
}

/* Calculates actual coordinates for mouse movement based on sensitivity and a constant. */
void Mouse::calibrateCoordinates(int &x, int &y)
{
	x = x - centreScreenX;
	y = y - centreScreenY;

	//float modX = 0.116f*(15.0f*(16.0f / 5.0f);

	x = (int)((float)x / modifier);
	y = (int)((float)y / modifier);
}

/* Moves to x,y coordinates after processed into actual coordinates (based on sensitivity and const). */
void Mouse::moveTo(int x, int y)
{
	calibrateCoordinates(x, y);
	mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	//std::cout << x << "," << y << std::endl;// << "      pred: " << predX << "," << predY << std::endl;
}

/* Mouse click @ 0,0 */
void Mouse::click(int leftDownMS)
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(leftDownMS);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void Mouse::moveSmoothAuto(int x, int y)
{
	int dist = abs((x - centreScreenX)*(y - centreScreenY));
	int speed = 0;

        if (dynamicScaling)
        {
		if (dist < 500) //25x25
			speed = 10;
		if (dist > 500 && dist < 2500) //50x50
			speed = 9;
		else if (dist > 2500 && dist < 22500) //150x150
			speed = 9;
		else if (dist > 22500 && dist < 90000) //300x300
			speed = 8;
		else if (dist > 90000)
			speed = 6;
        }
        else
        {
        	if (dist => minDistance && dist <= maxDistance) //Static aim speed and distance, IMO it's more legit because you are able to set a max distance 
			speed = aimSpeed;
        }

	//std::cout << speed << "   Disp: " << dist << "         " << "xy: " << x << "," << y << std::endl;
	moveSmooth1(x, y, 50, speed);
}

/* Moves mouse smoothly to a particular point:
   Time taken for mouse to move to point = 1000 - (aimspeed*10)
   So if aimspeed is 6; time taken to move to point would be 400ms */
void Mouse::moveSmooth1(int x, int y, int steps, int aimSpeed)
{
	if (aimSpeed == 0)
		return;
		
	calibrateCoordinates(x, y);
	int div = (1000 / steps);
	int sleep = div - (aimSpeed*(div / 10));

	//float estimatedTime = ((float)sleep * (float)steps) / 1000.0f;

	float divX = (float)x / (float)steps;
	float divY = (float)y / (float)steps;

	float X = 0.0f;
	float Y = 0.0f;
	for (int i = 0; i < steps; i++)
	{
		X += divX;
		Y += divY;
		if (X > 1.0f || X < 1.0f) //1 pixel is the smallest absolute value that we can move
		{
			mouse_event(MOUSEEVENTF_MOVE, int(X), 0, 0, 0);
			X -= int(X); //we do not want to lose the remainder -> leads to inaccuracies of lost
		}

		if (Y > 1.0f || Y < 1.0f) //^
		{
			mouse_event(MOUSEEVENTF_MOVE, 0, int(Y), 0, 0);
			Y -= int(Y); //^
		}

		Sleep(sleep);
	}
}

/* Moves mouse smoothly based on aim speed and no. of steps taken.
Time taken is random because sleep occurs on moving 1.0f pixel. */
void Mouse::moveSmooth2(int x, int y, int steps, int aimSpeed)
{
	if (aimSpeed == 0)
		return;
		
	calibrateCoordinates(x, y);

	float divX = (float)x / (float)steps;
	float divY = (float)y / (float)steps;

	float movedInX = 0.0f;
	float movedInY = 0.0f;
	float X = 0.0f;
	float Y = 0.0f;

	int sleep = 10 - aimSpeed;
	bool moved = false;

	for (int i = 0; i < steps; i++)
	{
		X += divX;
		Y += divY;

		//Use absolute values so as to eliminate branching statements/conditions for -ve vals
		float absoluteX = abs(X);
		float absoluteY = abs(Y);

		if (absoluteX >= 1.0f) { //1 pixel is the smallest absolute value that we can move
			mouse_event(MOUSEEVENTF_MOVE, int(X), 0, 0, 0);
			movedInX += absoluteX;
			X -= int(X); //we do not want to lose the remainder -> leads to inaccuracies if lost
		}

		if (absoluteY >= 1.0f) {
			mouse_event(MOUSEEVENTF_MOVE, 0, int(Y), 0, 0);
			movedInY += absoluteY;
			Y -= int(Y);
		}

		//This is what emulates 'aim speed'.
		while (movedInX >= widthTol) {
			Sleep(sleep);
			movedInX -= widthTol;
		}
		while (movedInY >= heightTol) {
			Sleep(sleep);
			movedInY -= heightTol;
		}
	}
}
