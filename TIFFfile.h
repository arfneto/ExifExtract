#pragma once

#include "stdafx.h"
#include <inttypes.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <memory>
#include <inttypes.h>
#include "IFDlist.h"
#include "Tags.h"
#include "TagMeans.h"

using namespace std;

#define		__BUFFER0JPEG__		65536

struct fileHeader
{
	uint16_t	byteOrder;
	uint16_t	tiffID;
	uint32_t	offset;
};

struct fileLog
{
	string		header;
	uint32_t	offset;
	uint32_t 	len;
	string		name;
};

struct JFIF
{
	char	identifier[5];
	char	majorVersion;
	char	minorVersion;
	char	units;
	char	xDensityM;
	char	xDensityL;
	char	yDensityM;
	char	yDensityL;
	char	xThumbnail;
	char	yThumbnail;
};

struct EXIF
{
	char		identifier[6];		// EXIF\0\0
	uint16_t	endian;				// 0x4949 little 0x4D4D big
	uint16_t	aa;					// 0x002A 0x2A00	
	uint32_t	offset;				// must be on word boundary

};

class TIFFfile
{
public:
	string	diskFile;
	bool	isLoaded;

	TIFFfile(string fileName);
	TIFFfile();

	~TIFFfile();	// destructor;

	const TIFFfile & operator =(const TIFFfile & other) = delete; // no =

	TIFFfile(const TIFFfile & other) = delete; // no copy

	void dumpAPP0(int);

	void dumpEXIF(int);

	void dumpFrom(const char * title, const short l, const unsigned char * buf) const;

	bool dumpTags(const string filename);

	void dumpIFDset(IFDlist *);

	void fileLoad();

	void fileStatus();

	IFD * getNewIFD(IFDlist *, uint32_t);

	void logThis(fileLog);
	void logThis(fileLog l, string comment);

	void processField(
		IFDfield,
		uint32_t,
		uint32_t,
		uint16_t,
		uint16_t);


	uint32_t processIFD();

	// test of dictionary of tags
	void testDict(IFDlist *);

	// test link structure
	void testLinks(IFDlist *);

	// test navigation in structure
	void testList(IFDlist *);


	void updateHighestOffset(uint32_t);

	IFDlist				ifdSet;

private:

	ifstream			jpgFile;
	unsigned char		buffer[__BUFFER0JPEG__];

	uint32_t	next{};
	uint32_t	lastAccessed{};
	uint32_t	highestAccessed{};

	uint32_t	bufSize;


	int			status;
	int			ignored;

	int			total;		// bytes read
	bool		atEof;
	bool		atError;

	short		st;			// state
	uint16_t	marker;		// byte being analyzed
	bool		inDataArea;
	bool		jpgEnd;

	vector<fileLog>		log;	// reader history
	JFIF				jfif;
	EXIF				exif;

	IFD *			pIFD;
	IFD *			currentIFD;

	uint16_t		b16{};
	uint32_t		b32{};
	uint64_t		b64{};

	bool			lsbFirst;
};

