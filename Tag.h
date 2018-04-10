#pragma once
#include "stdafx.h"
#include <inttypes.h>
#include <string>

using namespace std;

class Tag
{
public:

	uint16_t		_tag;
	uint16_t		_type;
	uint32_t		_count;
	uint32_t		_offset;
	string			_fieldDescription;
	string			_fieldName;
	string			_fieldValue;
	string			_tagValue;

	Tag();
	~Tag();
	void print();
};

