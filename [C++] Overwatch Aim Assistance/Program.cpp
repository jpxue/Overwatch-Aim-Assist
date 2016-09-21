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

#include "Capture.h"
#include "Time.h"
#include "Mouse.h"

using namespace std;

//======== Setup & Settings ========//
char* WindowName = "Overwatch"; //IMP: if overwatch is not detected your window name is different due to language differences! 
								//You can find your window name via this tool: https://www.mediafire.com/?c4qe6lpkw6s57cb

float MouseSensitivity = 15.00f; //Change this to your sensivitiy!!!

bool Aimbot = true;
bool HumanLikeMovements = false; //Should we use human like mouse movements?
bool Headshots = false; //Should we aim at the head?

bool Triggerbot = false; //Should we enable the trigger bot? Not recommended in certain maps
int BurstShootTime = 100; //Amount of time to hold left click in ms. Varies depending on champion being used.

//Virtual Keys: https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
int AimbotKey = 0x1; //VK_LBUTTON (LEFT MOUSE KEY)
int TriggerbotKey = 0x12; //VK_MENU (ALT KEY) 

int CaptureRate = 100; //How fast/slow you want the screenshots to be captured (in milliseconds)

int main(void)
{
	Capture recorder(WindowName);

	cout << "[========= SETTINGS ========]" << endl;
	cout << "- Mouse sensitivity : " << MouseSensitivity << endl;
	cout << "- Mouse movements : " << string(HumanLikeMovements ? "Human-like" : "Aimbot-like") << endl;
	cout << "- Aiming for the : " << string( Headshots ? "Head" : "Body" ) << endl;
	cout << "- Aimbot : " << string(Aimbot ? "Enabled" : "Disabled") << endl;
	cout << "- Triggerbot : " << string(Triggerbot ? "Enabled" : "Disabled") << endl;
	cout << "- Window name to scan for : '" << WindowName << "'" << endl << endl;

	cout << "[========= IMPORTANT ========]" << endl;
	cout << "Limit FPS to 'DISPLAY BASED'!!! Completely eliminates stuttering." << endl;
	cout << "Make sure that the display mode is 'BORDERLESS WINDOWED'!!!" << endl;
	cout << "Aim assistance will work only on detection of a health bar." << endl;
	cout << "Press 'CAPSLOCK' to terminate the program at anytime!" << endl;
	cout << "If not running an english version you need change the 'WindowName' variable, otherwise Overwatch will not be detected!" << endl << endl;

#ifdef _DEBUG
	cout << "DEBUG mode detected; please run in RELEASE mode for speed!" << endl << endl;
#endif

	cout << "Scanning for '" << WindowName << "' process handle (if stuck here read the comments).";
	while (!recorder.isWindowRunning())
	{
		cout << ".";
		Sleep(1000);
	}
	cout << endl << "Overwatch found!!!" << endl;
	
	recorder.switchToWindow(); //Switch to Overwatch
	Sleep(1000);
	Beep(1000, 250);

	//======    Main Code    ======//
	Screenshot screeny;
	Mouse mousey(recorder.getWidth(), recorder.getHeight(), MouseSensitivity);
	int x, y;
	bool run = true;

	while (run)
	{		
		if (GetAsyncKeyState(VK_CAPITAL))
			run = false;	

		//======   TRIGGER BOT   ======//
		if (Triggerbot)
		{
			if (GetAsyncKeyState(TriggerbotKey) & 0x8000)
			{
				while (!recorder.screenshotGDI(screeny)) //Should probably do better logic because it does not need to be called twice per loop
					Sleep(CaptureRate);

				if (screeny.triggerBot())
					mousey.click(BurstShootTime);
			}
		}

		//======   AIMBOT   ======//
		if (Aimbot)
		{
			if (GetAsyncKeyState(AimbotKey) & 0x8000)
			{
				while (!recorder.screenshotGDI(screeny)) //Should probably do better logic because it does not need to be called twice per loop
					Sleep(CaptureRate);

				if (screeny.findPlayer(x, y, Headshots))
				{
					if (HumanLikeMovements)
					{
						mousey.moveSmoothAuto(x, y);
					}
					else
					{
						mousey.moveTo(x, y); //use moveSmooth for human like movements and omit waitTillNextFrame				
						//if (!Triggerbot || (Triggerbot && BurstShootTime < 40))
						recorder.waitTillNextFrame(screeny); //eliminates mouse from 'overshoot' because the same frame is captured 2x in a row. Can be omitted if using moveSmooth, Sleep or click functions. 
					}
				}
			}
		}
	
		Sleep(1);
	}

	screeny.FreeMemory();
	Beep(1500, 500);

	cout << "Press 'Enter' to close!";
	cin.ignore();

	return 0;
}
