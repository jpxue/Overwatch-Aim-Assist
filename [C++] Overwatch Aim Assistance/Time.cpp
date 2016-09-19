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

#include "Time.h"

void Time::timerStart()
{
	tBegin = clock();
}

double Time::timerStop()
{
	tEnd = clock();
	double elapsed_secs = double(tEnd - tBegin) / CLOCKS_PER_SEC;
	return elapsed_secs;
}

string Time::getDate(char delimiter)
{
	time_t t = time(0);   //now
	struct tm *now = localtime(&t);

	ostringstream ss; //use iss or oss and NOT ss (bulkier)
	ss << now->tm_mday << delimiter << (now->tm_mon + 1)
		<< delimiter << (now->tm_year + 1900);

	return ss.str();
}

string Time::getTime(char delimiter)
{
	time_t t = time(0);
	struct tm *now = localtime(&t);

	ostringstream ss;
	ss << now->tm_hour << delimiter << now->tm_min << delimiter << now->tm_sec;

	return ss.str();
}