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

#ifndef TIME_H
#define TIME_H

#include <ctime>
#include <sstream>

using namespace std;

class Time
{
private:
	clock_t tBegin, tEnd;

public:
	string getDate(char delimiter);
	string getTime(char delimiter);
	void timerStart();
	double timerStop();
};


#endif // !TIME_H