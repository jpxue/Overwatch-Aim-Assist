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

#include <iostream>
#include <Windows.h>
#include <ctime>

#include "Capture.h"
#include "Time.h"
#include "Mouse.h"

using namespace std;

const float MouseSensitivity = 15.0f; //Change this to your sensivitiy!!!

int main(void)
{
	Capture recorder("Overwatch");

	cout << "Mouse sensitivity set to: " << MouseSensitivity << "!!!" << endl;
	cout << "Make sure that the display mode is 'BORDERLESS WINDOWED'!!!" << endl;
	cout << "Scanning for Overwatch process handle.";
	while (!recorder.isWindowRunning())
	{
		cout << ".";
		Sleep(1000);
	}
	cout << endl << "Overwatch found!!!" << endl;
	cout << "Press CAPS LOCK to terminate the program at anytime!" << endl;

	recorder.switchToWindow(); //switch to overwatch
	Sleep(1000);
	Beep(1000, 250); //indicates that program has started


	//======    Main Code    ======//
	Screenshot screeny;
	Mouse mousey(recorder.getWidth(), recorder.getHeight(), MouseSensitivity);
	int x, y;
	bool run = true;

	recorder.screenshotGDI(screeny);
	while (run)
	{
		if (GetAsyncKeyState(VK_CAPITAL))
			run = false;

		//======   TRIGGER BOT   ======//
		//if (screeny.triggerBot())
		//	mousey.click(50);

		//======   AIMBOT   ======//
		if (screeny.findPlayer(x, y, false)) //set to true if you aim headshots
		{
			mousey.moveTo(x, y);
			recorder.waitTillNextFrame(screeny); //imp to eliminate jittery movements, is not need with mouse.moveSmooth
			
			//mousey.moveSmoothAuto(x, y);
		}
			
		while(!recorder.screenshotGDI(screeny)); //capture until true
	}

	screeny.FreeMemory();
	Beep(1500, 500); //indicates that loop has terminated
	cout << "Press 'Enter' to close!";
	cin.ignore();
	return 0;
}
