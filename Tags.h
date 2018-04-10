#pragma once
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <map>
#include "Tag.h"

class Tags
{
private:
	string				_fileName;
	uint16_t			_offsetBase;

public:
	map<uint16_t, Tag>	_tagMap;

	Tags();
	~Tags();
	void print();
};

