#include "stdafx.h"
#include <string>
#include "Tags.h"

using namespace std;
Tags::Tags()
{
	_fileName = "test";
	_offsetBase = 0;
}

Tags::~Tags()
{
}

void Tags::print()
{
	cout << endl << "file is " <<_fileName << endl;
	cout << "Total of " << _tagMap.size() << " entries " << endl;
}

