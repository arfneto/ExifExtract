#include "stdafx.h"
#include <iostream>
#include "Tag.h"

using namespace std;

Tag::Tag()
{
}

Tag::~Tag()
{
}

void Tag::print()
{
	cout <<	
		"Tag is " << _tag << " HEX [" << _tagValue << "]" << endl;
	cout << "Name is: " << _fieldName << endl;
	cout << "Description is: " << _fieldDescription << endl;
	cout << "Value is: '" << _fieldValue << "'" << endl;
}
