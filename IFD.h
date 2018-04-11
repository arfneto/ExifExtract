#pragma once

#include "stdafx.h"

#include <inttypes.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <memory>
#include <inttypes.h>

using namespace std;

struct fileHeader
{
	uint16_t	byteOrder;
	uint16_t	tiffID;
	uint32_t	offset;
};

struct IFDentry
{
	uint16_t	tag;
	uint16_t	type;
	uint32_t	count;
	uint32_t	offset;

	char *		value;
};

class IFDlog
{
private:
	uint16_t			entries;
	vector<IFDentry>	ifdEntry;
	uint32_t			nextIFD;
public:
};

