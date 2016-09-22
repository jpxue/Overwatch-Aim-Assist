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

#include "Capture.h"

Capture::Capture(char *windowName)
{
	WindowName = windowName;
}

Capture::~Capture()
{
}

bool Capture::isWindowRunning()
{
	HWND hwnd = FindWindowA(0, WindowName);
	return (hwnd != NULL);
}

int Capture::getWidth() {
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd == NULL) return 0;

	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	return rekt.right - rekt.left;
}

int Capture::getHeight(){
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd == NULL) return 0;

	RECT rekt;
	GetWindowRect(hwnd, &rekt);
	return rekt.bottom - rekt.top;
}

void Capture::switchToWindow(){
	HWND hwnd = FindWindowA(0, WindowName);
	if(hwnd!=NULL) SwitchToThisWindow(hwnd, false);
}

/* Captures a screenshot and saves it into screeny which is passed by reference
   Return trype T/F indicates whether method was successful or not. */
bool Capture::screenshotGDI(Screenshot &screeny)
{
	HWND hwnd = FindWindowA(0, WindowName);
	if (hwnd == NULL)
	{
		cout << "ERROR: Overwatch HWND not found!" << endl;
		return false;
	}

	int sWidth = GetSystemMetrics(SM_CXSCREEN);
	int sHeight = GetSystemMetrics(SM_CYSCREEN);

	HDC hdc = GetDC(0);
	HDC captureDC = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, sWidth, sHeight);
	HGDIOBJ hOld = SelectObject(captureDC, hBmp);
	
	if (!BitBlt(captureDC, 0, 0, sWidth, sHeight, hdc, 0, 0, SRCCOPY | CAPTUREBLT))
	{
		cout << "ERROR: bit-block transfer failed!" << endl;
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}

	SelectObject(captureDC, hBmp);

	BITMAPINFO bmpInfo = { 0 };
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	if (!GetDIBits(hdc, hBmp, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS)) //get bmpInfo
	{
		cout << "ERROR: Failed to get Bitmap Info." << endl;
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	screeny.FreeMemory();

	int pixNo = bmpInfo.bmiHeader.biWidth *  bmpInfo.bmiHeader.biHeight;
	screeny.pixels = new RGBQUAD[pixNo];
	if (!screeny.pixels)
	{
		cout << "ERROR: Failed allocating RGBQUAD[" << pixNo << "]" << endl;
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}

	if (!GetDIBits(hdc, hBmp, 0, bmpInfo.bmiHeader.biHeight, (LPVOID)screeny.pixels, &bmpInfo, DIB_RGB_COLORS))
	{
		cout << "ERROR: Getting the bitmap buffer!" << endl;
		screeny.FreeMemory();
		release(hwnd, hdc, captureDC, hBmp);
		return false;
	}

	screeny.width = bmpInfo.bmiHeader.biWidth;
	screeny.height = bmpInfo.bmiHeader.biHeight;
	screeny.length = pixNo;

	release(hwnd, hdc, captureDC, hBmp);
	return true;
}

// Memory management for screenshot function above
void Capture::release(HWND &hwnd, HDC &hdc, HDC &captureDC, HBITMAP &hBmp)
{
	ReleaseDC(hwnd, hdc);
	DeleteObject(hBmp);
	DeleteDC(captureDC);
	DeleteDC(hdc);
}

// Pauses until next frame is found 
bool Capture::captureNewFrame(Screenshot &currentFrame)
{
	if (!currentFrame.pixels)
		return screenshotGDI(currentFrame);

	Midline prevFrameMidline(currentFrame);
	bool newFrameLoaded = false;
	while (!newFrameLoaded) //Wait till new frame loads!
	{
		bool captured = screenshotGDI(currentFrame);
		if (!captured)
			return false;

		if (currentFrame != prevFrameMidline)
		{
			newFrameLoaded = true; //not necessary
			return true;
		}
		else
			Sleep(10);
	}

	return true;
}