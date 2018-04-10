#pragma once
#include "stdafx.h"
#include <inttypes.h>
#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <memory>
using namespace std;

struct IFDfield
{
	uint16_t	tag;
	uint16_t	type;
	uint32_t	count;
	uint32_t	offset;
	IFDfield *	nextField;
};

struct IFD
{
	uint32_t			address;
	uint16_t			entries;
	IFDfield *			fieldList;
	short				index;
	uint32_t			nextIFDoffset;
	bool				processed;
	IFD *				nextIFD;
	uint16_t			origin; // 0x8769 0xA005 for indicating non-linked IFD
	IFDfield *			lastField;

};

class IFDlist
{
public:

	IFD *		ifd;
	short		ifdCount;
	uint32_t	offsetBase;
	IFD *		lastIFD;

	IFDlist();
	~IFDlist();

	IFDfield * insertField( IFDfield, IFD *);

	IFD * insertIFD( IFD i, IFDlist * s );	// insert an IFD at the end of the list
};

