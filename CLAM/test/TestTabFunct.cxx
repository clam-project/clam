/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include "TabFunct.hxx"
#include "Assert.hxx"
#include <iostream>

using std::cout;

using namespace CLAM;

namespace CLAMTest{

class Log
{
public:
	TData operator() (const TData arg) { return log(arg); }
};

class Double
{
public:
	TData operator() (const TData arg) { return arg*2; }
};

}; //namespace


int TestTabFunction()
{
	TabFunct<CLAMTest::Log> f(11, 0, 100);
	TabFunct<CLAMTest::Double> g(3, 0, 100);
	
	for (TData x=0; x<=100; x+=1)	cout << f(x) << " ";
	
	cout << "\n";
	for (TData x=0; x<=100; x+=1)	cout << g(x) << " ";
	

	return 0;
}


int main(void)
{
	TestTabFunction();

	return 0;
}
 