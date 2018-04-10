#pragma once
#include "stdafx.h"
#include <inttypes.h>
#include <map>
#include <string>
#include "Tag.h"

//
// Data from CIPA DC-008-Translation-2016
//	JEITA CP-3451D
//	CIPA DC-008-2016
//
//	Exchangeable Image file formart for digital still cameras: Exif Version 2.31
//

using namespace std;

struct FieldDict
{
	string		_category;
	string		_tagName;
	string		_fieldName;
	string		_decID;
	string		_hexID;
	string		_type;
	string		_count;
};

class TagMeans
{

private:
	map<uint16_t, FieldDict>	dict;
	//
	// Interoperability Index uses tag 1.H that is also used in Exif
	// rules are: 4 bytes, type ASCII, null terminated, no default but
	// set to "???"in the constructor. Actual value set by setInterop()
	// method
	// possible values "R98" "THM" "R03"
	//
	string						InteropIndex;

public:
	TagMeans();
	~TagMeans();

	void print();

	void printTag(FieldDict &);

	bool explainTag(uint16_t &, FieldDict &);

	void setInterop(char[4]);

};

