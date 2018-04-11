
#include "stdafx.h"
#include "TIFFfile.h"

using namespace std;

TIFFfile::TIFFfile() : status(-1) {};

TIFFfile::TIFFfile(
	string	fileName
)
{
	isLoaded = false;
	diskFile = fileName;
	jpgFile.open(diskFile, ios::in | ios::binary);
	if (!jpgFile)
	{
		cout << "could not open " << fileName << " in desired mode" << endl;;
		status = -1;
		return;
	}
	jpgFile.read( (char *) buffer, __BUFFER0JPEG__);
	bufSize = (int) jpgFile.gcount();
	std::printf(
		"\t%d bytes loaded from disk\n",
		bufSize
	);

	next = 0;
	atEof = jpgFile.eof();
	atError = jpgFile.failbit || jpgFile.badbit;
	total = 0;
	inDataArea = false;
	ignored = 0;
	jpgEnd = false;
	st = 0;
	lsbFirst = false;

	ifdSet.offsetBase = -1;
	ifdSet.ifd = nullptr;

}	// end TIFFfile()

TIFFfile::~TIFFfile()
{
	jpgFile.close();
	cout << diskFile << " is now closed in the destrutor" << endl;;
	status = 1;
}

void TIFFfile::dumpAPP0(int address)
{

	int	x = jfif.xDensityL + jfif.xDensityM * 256;
	int	y = jfif.yDensityL + jfif.yDensityM * 256;
	printf("\n\tAPP0 address is 0x%X\n", address);
	printf("\tIdentifier is %s\n", jfif.identifier);
	printf(
		"\tVersion is %d.%02d\n",
		(short)jfif.majorVersion, (short)jfif.minorVersion
	);
	switch (jfif.units)
	{
	case 0:
		printf(
			"\tdensity is %dx%d pixels\n", x, y
		);
		break;
	case 1:
		printf(
			"\tdensity is %dx%d dots per inch\n", x, y
		);
		break;
	case 2:
		printf(
			"\tdensity is %dx%d dots per cm", x, y
		);
		break;
	}	//	end switch
	printf("\t%dx%d thumbnail\n",
		jfif.xThumbnail,
		jfif.yThumbnail
	);
	return;
}

void TIFFfile::dumpEXIF(int address)
{
	printf("\tdumpEXIF: APP1 address is 0x%X\n", address);
	printf("\tIdentifier is '%s'\n", exif.identifier);
	printf("\tByte Order: 0x%04X\n", exif.endian);
	if (exif.endian = 0x4949)
	{
		lsbFirst = true;
		printf("\tLSB first: little endian\n");
	}
	else
	{
		lsbFirst = false;
		printf("\tMSB first: big endian\n");
		return;
	}
	printf(
		"\tEXIF id is 0x%04X\n",
		(short)exif.aa
	);
	printf(
		"\tIFD0 offset is %d\n",
		(short)exif.offset
	);
	printf(
		"\tIFD0 address in disk is 0x%X\n",
		exif.offset + ifdSet.offsetBase
	);
	return;
}	//	end dumpEXIF()

void TIFFfile::dumpFrom(
	const char *	title,
	const short		l,
	const unsigned char * buf
) const
{
	//
	// dump first len bytes
	//
	printf("\n%s [first %d bytes]\n", title, l);
	for (int i = 0; i < l;)
	{
		printf("%02" PRIX8 " ", *(buf + i));
		i += 1;
		if (i % 10 == 5)
		{
			printf("    ");
		}
		else
		{
			if ((i % 10 == 0) && (i != 0))
			{
				printf("\n");
			}
		}
	}
	printf("\n");
}	// end dumpFrom()

/////

bool TIFFfile::dumpTags(const string fileName)
{
	fstream	out;
	TagMeans &		tagDict = ifdSet.exifTags;	// book of tags
	FieldDict		field;		// local field
	char *			pChar;

	out.open(fileName);
	if (!out.is_open())
	{
		printf("\tdumpTabs(): error creating '%s'\n", fileName.c_str());
		//return false;
	}

	if (!isLoaded)
	{
		printf("\tdumpTabs(): file '%s' is not loaded yet. Aborting.\n", diskFile.c_str());
		out.close();
		return false;
	}

	// point to IFD0, list them all
	pIFD = ifdSet.ifd;
	for (int n = 0; n < ifdSet.ifdCount; n++)
	{
		printf(
			"\tIFD%d: %d entries. address 0x%X, index is %d\n",
			n,
			pIFD->entries,
			pIFD->address,
			pIFD->index
		);
		pIFD = pIFD->nextIFD;
	};

	// point to IFD0, list all tags
	pIFD = ifdSet.ifd;
	for (int n = 0; n < ifdSet.ifdCount; n++)
	{
		printf(
			"\n\tIFD%d: %d entries. address 0x%X, index is %d\n",
			n,
			pIFD->entries,
			pIFD->address,
			pIFD->index
		);
		IFDfield * p = pIFD->fieldList; // 1st field
		for (int f = 1; f <= pIFD->entries; f++)
		{
			if (tagDict.explainTag(p->field.tag, field))
			{
				pChar = p->field.value;
				printf(
					"\t%3d: %s %s 0x%X (%d) =[%s]\n",
					f,
					field._hexID.c_str(),
					field._tagName.c_str(),
					p->field.offset,
					p->field.offset,
					pChar
				);
			}
			else
			{
				printf("\t%3d: ***** tag 0x%X (%d) not in book...\n", f, p->field.tag, p->field.tag);
			}
			p = p->nextField;
		}	// end for f
		pIFD = pIFD->nextIFD;
	}
	return true;
}	// end dumpTags()

void TIFFfile::dumpIFDset(IFDlist * l)
{
	uint32_t		base{};	// to compute field addresses

	printf("************************************************************\n");
	if (l->ifd == nullptr )
	{
		printf("\n\tdumpIFDset:\tIFD list is empty\n\n");
		return;
	};
	// dumps all data
	printf(
		"\n\tdumpIFDset:\t%d entries,\tGlobal Offset is 0x%X\n",
		l->ifdCount,
		l->offsetBase 
		);
	IFD *		head = l->ifd;
	IFDfield *	field{};

	//
	// dump IFDs from 1 to ifdCount
	//
	for (
		int n = 1;
		n <= l->ifdCount;
		n++
		)
	{
		// IFDn
		int i = n - 1;
		if ( (*head).processed)
		{
			printf(
				"\n\tIFD%d (PROCESSED):\n\n\tIndex %d, %d field (s), disk address is [0x%X]. Next is [0x%X]\n",
				(*head).index,
				(*head).index,
				(*head).entries,
				(*head).address,
				(*head).nextIFDoffset
			);
		}
		else
		{
			printf(
				"\n\tIFD%d (NOT PROCESSED):\n\n\tIndex %d, %d field (s), disk address is [0x%X]. Next is [0x%X]\n",
				(*head).index,
				(*head).index,
				(*head).entries,
				(*head).address,
				(*head).nextIFDoffset
			);

		}
		if ((*head).entries < 1)
		{
			printf("\t***** no fields yet *****\n");
			head = (*head).nextIFD;
		}
		else
		{
			field = (*head).fieldList;
			base = (*head).address + 2;
			for (
				int f = 1;
				f <= (*head).entries;
				f++)
			{
				// list a field
				printf(
					"\t[%4d/%4d]:\ttag 0x%X,\ttype 0x%X,\tcount 0x%X,\toffset 0x%8X\t@0x%X\n",
					f,
					(*head).entries,
					field->field.tag,
					field->field.type,
					field->field.count,
					field->field.offset,
					base
				);
				base += 12;	// 12 bytes field
				field = field->nextField;
			};
			head = (*head).nextIFD;
		}
	};// end for n
	printf("************************************************************\n");
	return;
}

void TIFFfile::fileLoad()
{
	uint32_t		segmentStart{ 0 };
	uint32_t		segmentMarker{ 0 };

	uint32_t		len{ 0 };
	uint32_t		toRead{ 0 };
	int				nAPP{ -1 };

	uint16_t		dWord{};
	uint32_t		offset{};
	uint32_t		base{};

	IFDfield *		pField;

	int				n{};
	char			sMsg[80];
	char			sValue[80];

	uint16_t		nIFD{};
	uint16_t		nEntries{};
	bool			found = false;

	next = 0;
	lastAccessed = 0;
	highestAccessed = 0;

	while (next < bufSize)
	{
		marker = buffer[next];
		next += 1;
		switch (st)
		{
		case 0:
			if (marker == 0xff)
			{
				st = 1;
			}
			else
			{
				st = 15;
			}
			break;

		case 1:
			// post FF. FF00 seems to be an escape sequence when in data area
			if (marker != 0)
			{
				// a new header
				inDataArea = false;
			}
			segmentStart = next - 2;	// FFXX
			segmentMarker = marker;
			switch(marker)
			{

			case 0:
				// it seems to be an escape sequence into the
				// data segments to follow every FF with 00
				// so just go on in this case since FF00 means
				// just an escaped 0xff
				st = 2;
				break;

			case 0xC0:
				sprintf_s(sValue, "SOF - Start of Frame");
				st = 3;
				break;

			case 0xC4:
				sprintf_s(sValue, "DHT - Define Huffman Table");
				st = 3;
				break;

			case 0xCC:
				sprintf_s(sValue, "DAC - Define Arithmetic Conditioning Table(s)");
				st = 3;
				break;

			case 0xD8:
				st = 8;
				updateHighestOffset(next-1);
				logThis( { "FFD8", segmentStart, 2, "SOI - Start of image"	}, "" );
				break;

			case 0xD9:
				logThis(
					{ "FFD9", segmentStart, 2, "EOI - End of image" }, ""
				);
				st = 15;
				break;

			case 0xDA:
				inDataArea = true;
				logThis(
					{ "FFDA", segmentStart, 0, "SOS - Start of Scan" }, ""
				);
				st = 2;
				break;

			case 0xDB:
				sprintf_s(sValue, "DQT - Define Quantization Table");
				st = 3;
				break;

			case 0xDD:
				sprintf_s(sValue, "DRI - Define Restart Interoperability");
				st = 3;
				break;

			case 0xDF:
				sprintf_s(sValue, "EXP - Expand Reference Images");
				st = 3;
				break;

			case 0xE0:
				sprintf_s(sValue, "APP0 - JPEG File Interchange Format JFIF marker");
				st = 3;
				break;

			case 0xE1:
				sprintf_s(sValue, "APP1 - Exif attribute Information");
				st = 3;
				break;

			case 0xE2:
			case 0xE3:
			case 0xE4:
			case 0xE5:
			case 0xE6:
			case 0xE7:
			case 0xE8:
			case 0xE9:
			case 0xEA:
			case 0xEB:
			case 0xEC:
			case 0xED:
			case 0xEE:
			case 0xEF:
				sprintf_s(sValue, "APP%2X - Reserved for Application Use", (marker - 0xE0));
				st = 3;
				break;

			case 0xFE:
				sprintf_s(sValue, "COM - JPEG Comment marker");
				st = 3;
				break;

			default:
				sprintf_s(sMsg, "FF%2X", marker);
				logThis(
					{ sMsg, segmentStart, 0, "***** unknown marker *****" }, ""
				);
				st = 8;
				break;

			}	// end switch marker
			// end case st 1
			break;

		case 2:
			// skipping possibly compressed data
			if (marker == 0xff)
			{
				// if it is FF00 just go, else end of segment
				st = 1;
			}
			updateHighestOffset(marker);
			break;

		case 3:	// first byte of length
			len = 256 * marker;
			st = 4;
			break;

		case 4:	// second byte of length
			len = len + marker;
			toRead = len - 2;
			printf(" Segment FF%2X:  %X (%d) bytes\n", segmentMarker, len, len);
			sprintf_s(sMsg, "FF%2X", marker);
			logThis(
				{ sMsg, segmentStart, len+2, sValue }, ""
			);
			switch (segmentMarker)
			{

			case 0xDA:	// Start of Frame SOF
				// this segment has no defined length except for a 12-byte header
				// just skip all data until FFD9
				next = next + 12 -2;	// skip 12-bytes header;
				updateHighestOffset(next);
				next -= 1;
				inDataArea = true; // now must pass over image data
				st = 2;
				break;

			case 0xE0:	// APP0 is a 16 bytes area, including 2 bytes for length 
				dumpAPP0(next-4);
				next = next + len - 2;
				updateHighestOffset(next);
				next -= 1;
				st = 8;
				break;

			case 0xE1:	// APP1 Exif data
				memcpy(&exif.identifier, buffer + next, 6);
				next += 6;	// Exif\0\0
				ifdSet.offsetBase = next;	// global offset is after Exif\0\0
				memcpy(&exif.endian, buffer + next, 2);
				next += 2;
				memcpy(&exif.aa, buffer + next, 2);
				next += 2;
				memcpy(&exif.offset, buffer + next, 4);
				next += 4;
				offset = exif.offset;
				dumpEXIF(ifdSet.offsetBase - 6 - 4);
				updateHighestOffset(next);
				currentIFD = getNewIFD(& ifdSet, 0);	// IFD0
				// pointing now to first byte of ifd field count. must go back one
				// byte before returning to loop
				next -= 1;
				st = 6;
				break;

			default:
				next = next + len - 2;
				updateHighestOffset(next);
				next -= 1;
				st = 8;
				break;
			}	// end switch(marker)
			break;

		case 5:
			// FFD9: end game
			printf("\n\t***** FFD9 end game *****\n");
			return;
			break;

		case 6:
			offset = processIFD();
			if (currentIFD->origin == 0x8769)
			{
				sprintf_s(sMsg, "st 6 @ 0x%X", next);
				logThis({ sMsg, 0, 0, "0x8769 Exif IFD" }, "");
				next -= 1;
				st = 7;
				break;
			};
			if (currentIFD->origin == 0x8825)
			{
				sprintf_s(sMsg, "st 6 @ 0x%X", next);
				logThis({ sMsg, 0, 0, "0x8825 GPS IFD" }, "");
				next -= 1;
				st = 7;
				break;
			};
			if (currentIFD->origin == 0xA005)
			{
				sprintf_s(sMsg, "st 6 @ 0x%X", next);
				logThis({ sMsg, 0, 0, "0xA005 InterOp IFD" }, "A005");
				next -= 1;
				st = 7;
				break;
			};
			if (offset == 0)
			{
				sprintf_s(sMsg, "st 6 last IFD");
				logThis({ sMsg, 0, 0, "Last IFD in chain" }, "");
				next -= 1;
				st = 7;
				break;
			};
			currentIFD = getNewIFD(&ifdSet, (offset + ifdSet.offsetBase));	// prepare new IFD
			next = (ifdSet.offsetBase + offset) - 1;
			st = 6;
			break;

		case 7:
			// after IFD list process
			// time to scan fields and look for possible new included IFD
			// first, get next IFD marked as not processed

			// where are we?
			found = false;
			pIFD = ifdSet.ifd;	// point to IFD0
			while (true)
			{
				if (!(pIFD->processed))
				{
					found = true;	// 1st not processed
					break;
				}
				else
				{
					if (pIFD->nextIFD == nullptr)
					{
						break;	// all processed
					}
					else
					{
						pIFD = pIFD->nextIFD;
					}
				}
			};	// end while
			if (!found)
			{
				printf("\n\t***** no IFD to process *****\n");
				next = highestAccessed - 1;
				st = 0;
				//st = 8;
				break;
			}
			// process this one;
			currentIFD = pIFD;
			printf( "\n\t***** next IFD is IFD%d *****\n", currentIFD->index);
			if (currentIFD->entries == 0)
			{
				// an empty IFD here means that we are pointing to 
				// an special IFD found before
				// and must fill in the data into the Field Array
				// state 6
				printf(
					"\tIFD %d at 0x%X is empty: will scan for fields\n",
					currentIFD->index,
					currentIFD->address
				);
				printf(
					"\tglobal offset is 0x%X, address in disk is 0x%X\n",
					ifdSet.offsetBase,
					currentIFD->address + ifdSet.offsetBase
				);
				next = ifdSet.offsetBase + currentIFD->address - 1;
				st = 6;
				break;
			}
			next = currentIFD->address + 2;
			pField = currentIFD->fieldList;	// 1st field
			n = 1;
			do
			{
				processField(
					&(*pField).field,
					ifdSet.offsetBase,
					next - 12,
					n,
					currentIFD->entries);
				pField = pField->nextField;
				n += 1;
			} while (pField != nullptr);
			printf(
				"\t***** %d fields for IFD %d at 0x%X\n",
				currentIFD->entries,
				currentIFD->index,
				currentIFD->address
			);

			currentIFD->processed = true;
			next -= 1;
			// done with this IFD
			break;

		case 8:
			// looking for segments, after the first
			if (marker == 0xff)
			{
				st = 1;
			}
			else
			{
				if (!inDataArea)
				{
					ignored += 1;
				}
			}
			break;

		case 15:
			isLoaded = true;
			// test
			return;
			break;

		default:
			break;
		}	// end switch
	}	// end while()
}	// end fileLoad()

void TIFFfile::fileStatus(){
	printf(
		"\n\tfileStatus(%s):\t***** # of log entries is %d *****\n",
		diskFile.c_str(),
		log.size()
	);
	printf(
		"\tbuffer capacity: %d\n", bufSize
	);
	printf(
		"\tPointer at: %d\n", next
	);
	printf(
		"\n\tfileStatus:\t***** # of log entries is %d *****\n",
		log.size()
	);
	for (vector<fileLog>::iterator it = log.begin(); it != log.end(); it++)
	{
		printf(
			"\t%s[0x%X,0x%X]\t[%d,%d]\t - %s\n",
			it->header.c_str(),
			it->offset,
			it->len,
			it->offset,
			it->len,
			it->name.c_str()
		);
	}
	printf(
		"\n\tfileStatus:\t***** end of log *****\n\n\n");
	return;
}	// end fileStatus()

IFD * TIFFfile::getNewIFD(IFDlist * l, uint32_t offset)
{
	IFD * p;
	// create new record
	p = (IFD *)::operator new(sizeof(IFD));
	p->index = 0;	// invalidate
	p->processed = false;
	p->address = offset;
	p->entries = 0;
	p->nextIFDoffset = 0;
	p->nextIFD = nullptr;
	p->origin = 0;
	p->fieldList = nullptr;
	p = l->insertIFD(*p, l);	// now get index
	return p;
}	// end getNewIFD()

void TIFFfile::logThis(fileLog l)
{
	// just to have control over logging
	// while testing
	log.push_back(l);
}

void TIFFfile::logThis(fileLog l, string comment)
{
	// just to have control over logging
	// while testing
	log.push_back(l);
	if (comment.length() == 0)
	{
		printf(
			"\t[%s]:\t[0x%X,0x%X]\t{%d,%d}]\t\t'%s'\n",
			l.header.c_str(),
			l.offset,
			l.len,
			l.offset,
			l.len,
			l.name.c_str()
		);
	}
	else
	{
		printf(
			"\t\t\t%s ==>\n\t[%s]:\t[0x%X,0x%X]\t{%d,%d}]\t\t'%s'\n",
			comment.c_str(),
			l.header.c_str(),
			l.offset,
			l.len,
			l.offset,
			l.len,
			l.name.c_str()
		);
	}
}

void TIFFfile::processField(
	 IFDentry *		f,
	 uint32_t		globalOffset,
	 uint32_t		address,
	 uint16_t		pos,
	 uint16_t		t
) 
{
	char *		pField;
	char		sMsg[80];
	char		sValue[80];
	char *		pBase;
	char *		pRational;
	char *		pTempChar;
	int			n{};
	int			size;
	int			slonga{ 0 };
	int			slongb{ 0 };
	string		s;
	uint16_t	n16{};
	uint32_t	n32a{};
	uint32_t	n32b{};

	IFD	*		pIFD;

	sprintf_s(
		sMsg,
		"\tProcessField %d of %d: @0x%X, offset 0x%X",
		pos, t, 
		address, globalOffset
	);
	sprintf_s(
		sMsg, "%04X", f->tag
	);
	sprintf_s(
		sValue,
		"[%X] type: [%X] count: [%X], offset: [%X]\t(%d of %d)",
		f->tag,		f->type,
		f->count,	f->offset,
		pos, t
		);
	s = sValue;
	//logThis({ sMsg, 0, 0, sValue }, "processField ==>");

	// field types are
	// 1 - byte 8 bit unsigned
	// 2 - ascii
	// 3 - unsigned short 
	// 4 - unsigned long
	// 5 - rational a/b longs
	// 6 - signed byte 8 bits
	// 7 - undefined
	// 8 - signed short 16 bits
	// 9 - signed long 32 bits
	// 10- srational two slongs = a/b
	// 11- float 4 bytes
	// 12- float 8 bytes

	switch (f->type)
	{
	case 1:
		//	TYPE 1 is BYTE
		//
		// value line is "(BYTE[n] = 1 2 ... n)"
		//
		sprintf_s(sMsg, "%4X", f->tag);
		//
		// special case: if f->tag is 0x00 it is the GPS info tag
		//
		if (f->tag == 0x0)	// TAG 0 is GPS Version Tag
		{
			pBase = (char *)& f->offset;
			sprintf_s(sValue, "GPSInfo tag is \"%c.%c.%c.%c\"",
				(char)(*(pBase + 0) + 48),
				(char)(*(pBase + 1) + 48),
				(char)(*(pBase + 2) + 48),
				(char)(*(pBase + 3) + 48)
			);
			logThis({ sMsg, 0, 0, sValue }, "");
			//
			pField = new(char[strlen(sValue) + 1]);
			strcpy_s(pField, strlen(sValue) + 1, sValue);
			f->value = pField;
			break;
		}
		//
		// value line is "(BYTE[n] = 1 2 ... n)"
		//
		sprintf_s( sValue, "(BYTE[%d] = ", f->count );
		n = f->count;
		if (n <= 4)
		{
			// count <= 4: up to 4 bytes inline in f->offset
			pBase = (char *) & f->offset;
		}
		else
		{
			// count > 4: bytes are pointed by f->offset
			// for safety, we will show just the first 8 ones
			n = (n > 8) ? 8 : n;
			pBase = (char *)(buffer + globalOffset + f->offset);
			updateHighestOffset(globalOffset + f->offset + f->count - 1);
		}
		for (int i = 0; i < n; i++)
		{
			sprintf_s(
				sValue,
				"%s 0x%X (%d)",
				sValue,
				(int) pBase,
				(int) pBase
			);
			pBase++;
		}
		// close value string
		sprintf_s(sValue,"%s)", sValue );
		pField = new(char[strlen(sValue) + 1]);
		strcpy_s(pField, strlen(sValue) + 1, sValue);
		f->value = pField;
		break;
	
	case 2:
		// ASCII
		//
		// value line is "(ASCII[n] = "..."
		//
		n = f->count;
		sprintf_s(sValue, "(ASCII[%d] = ", n);
		//
		if (n <= 4)
		{
			// count <= 4: up to 4 bytes inline in f->offset
			pBase = (char *) & f->offset;
		}
		else
		{
			// count > 4: bytes are pointed by f->offset
			// for safety, we will show just the first 8 ones
			pBase = (char *)(buffer + globalOffset + f->offset);
			if (f->tag == 0xA420)
			{
				// ImageUniqueID seems to have a padding byte
				updateHighestOffset(globalOffset + f->offset + f->count);
			}
			else
			{
				updateHighestOffset(globalOffset + f->offset + f->count - 1);
			}
		}
		memcpy_s(
			sMsg, 80,
			pBase,
			n
		);
		sprintf_s(
			sValue,
			"%s\"%s\")",
			sValue,
			sMsg
		);
		pField = new(char[strlen(sValue) + 1]);
		strcpy_s(pField, strlen(sValue) + 1, sValue);
		f->value = pField;
		n16 = 0;	// dummy break point
		break;

	case 3:
		// unsigned short, 2 bytes
		//
		// value line is "(SHORT[n] = "1 2 ... n""
		//
		n = f->count;
		sprintf_s(sValue, "(SHORT[%d] = ", n);
		//
		if (n <= 4)
		{
			// count <= 4: up to 4 bytes inline in f->offset
			pBase = (char *)& f->offset;
		}
		else
		{
			// count > 4: bytes are pointed by f->offset
			pBase = (char *)(buffer + globalOffset + f->offset);
			// each field uses 2 bytes
			updateHighestOffset(globalOffset + f->offset + (2 * f->count) - 1);
			// now cut to the first 8 items
			n = (n > 8) ? 8 : n;
		}
		// now loop thru the items
		for (int i = 0; i < n; i++)
		{
			memcpy_s(&b16, 2, pBase, 2);
			sprintf_s(
				sValue,
				"%s0x%X (%d)",
				sValue,
				b16,
				b16
			);
			pBase += 2;
		}
		sprintf_s(sValue, "%s)", sValue);	// closes the string
		logThis({ sMsg, 0, 0, sValue }, "");
		//
		pField = new(char[strlen(sValue) + 1]);
		strcpy_s(pField, strlen(sValue) + 1, sValue);
		f->value = pField;
		n16 = 0;	// dummy break point
		break;

	case 4:
		// LONG
		//
		// value line is "(LONG[n] = "1 2 ... n""
		//
		n = f->count;
		sprintf_s(sValue, "(LONG[%d] = ", n);
		//
		if (n == 1)
		{
			pBase = (char *)& f->offset;
		}
		else
		{
			// count > 1: bytes are pointed by f->offset
			pBase = (char *)(buffer + globalOffset + f->offset);
			// each field uses 4 bytes
			updateHighestOffset(globalOffset + f->offset + (4 * f->count) - 1);
			// now cut to the first 8 items
			n = (n > 8) ? 8 : n;
		}
		// now loop thru the items
		for (int i = 0; i < n; i++)
		{
			memcpy_s(&b32, 4, pBase, 4);
			sprintf_s(
				sValue,
				"%s0x%X (%d)",
				sValue,
				b32,
				b32
			);
			pBase += 4;
		}
		sprintf_s(sValue, "%s)", sValue);	// closes the string
		logThis({ sMsg, 0, 0, sValue }, "");
		//
		pField = new(char[strlen(sValue) + 1]);
		strcpy_s(pField, strlen(sValue) + 1, sValue);
		f->value = pField;
		//
		if (f->tag == 0x8769)	// Exif IFD
		{
			//dumpFrom("Start of Exif IFD", 40, buffer + f->offset + globalOffset);
			sprintf_s(
				sValue,
				"TAG %X Exif IFD. offset is 0x%X. Address in disk is 0x%X",
				f->tag,
				f->offset,
				f->offset + globalOffset
			);
			logThis({ sMsg, 0, 0, sValue }, "8769");
			// update value on f->value;
			delete pField;
			size = strlen(sValue) + 1;
			pField = new(char[size]);
			sprintf_s(
				pField, size,
				"Exif IFD. offset is 0x%X. Address in disk is 0x%X",
				f->offset,
				f->offset + globalOffset
			);
			f->value = pField;
			//
			// set up a new IFD to run this
			pIFD = getNewIFD(&ifdSet, f->offset);
			pIFD->origin = 0x8769; // sign to not search for next in IFD chain
		}
		else
		{
			if (f->tag == 0xA005)	// Interoperability IFD
			{
				sprintf_s(
					sValue,
					"Exif Interoperability IFD. offset is 0x%X. Address in disk is 0x%X",
					f->offset,
					f->offset + globalOffset
				);
				logThis({ sMsg, 0, 0, sValue }, "");
				// update value on f->value;
				delete pField;
				size = strlen(sValue) + 1;
				pField = new(char[size]);
				sprintf_s(
					pField, size,
					"Exif Interoperability IFD. offset is 0x%X. Address in disk is 0x%X",
					f->offset,
					f->offset + globalOffset
				);
				f->value = pField;
				//
				pIFD = getNewIFD(&ifdSet, f->offset);
				pIFD->origin = 0xA005; // sign to not search for next in IFD chain
			}
			else
			{
				if (f->tag == 0x8825)	// Interoperability IFD
				{
					//dumpFrom("Start of GPS IFD", 40, buffer + f->offset + globalOffset);
					sprintf_s(
						sValue,
						"TAG %X GPS IFD. offset is 0x%X. Address in disk is 0x%X",
						f->tag,
						f->offset,
						f->offset + globalOffset
					);
					logThis({ sMsg, 0, 0, sValue }, "8825");
					// update value on f->value;
					delete pField;
					size = strlen(sValue) + 1;
					pField = new(char[size]);
					sprintf_s(
						pField, size,
						"GPS Info IFD. offset is 0x%X. Address in disk is 0x%X",
						f->offset,
						f->offset + globalOffset
					);
					f->value = pField;
					//
					// this is all for now
					pIFD = getNewIFD(&ifdSet, f->offset);
					pIFD->origin = 0x8825; // sign to not search for next in IFD chain
				}
			}
		}
		n16 = 0;	// dummy break point;
		break;

	case 5:
		//
		// 5: rational: A/B/C...
		//
		// each rational uses 2 4-byte longs where the first one is the numerator
		// and the second one is the denominator
		//
		// Value string is "(RATIONAL[n] [A1/A2] [B1/B2]...)" no hex values this time
		//
		n = f->count;
		size = 30 * n + 50;
		pRational = (char *)::operator new(size); // 30 bytes for each [A/B] pair
		sprintf_s(	pRational, size, "(RATIONAL)[%d]: ", n);
		pBase = (char *)(buffer + globalOffset + f->offset);	// pointing to start
		for (int i = 0; i < n; i++)
		{
			// build one pair in sRational
			memcpy_s(&n32a, 4, pBase, 4);
			pBase += 4;
			memcpy_s(&n32b, 4, pBase, 4);
			pBase += 4;
			sprintf_s(pRational, size,
				"%s[%d / %d]",
				pRational, 
				n32a, n32b
			);
		}
		sprintf_s(pRational, size, "%s)", pRational);
		pField = pRational;
		f->value = pField;
		sprintf_s(sMsg, 80, "%04X", f->tag);
		logThis({ sMsg, (globalOffset + f->offset), (f->count*8), sValue }, "");
		updateHighestOffset(globalOffset + f->offset + (8 * f->count) - 1);
		n16 = 0;	// dummy break point
		break;

	case 7:
		// undefined
		//
		// this is where we need to look into the tag code first
		//	it is defined by the tag code :)
		//
		sprintf_s(sMsg, 80, "%04X", f->tag);
		switch (f->tag)
		{
		case 0x0002:
			// THIS can take 3 values an of Exif version 3.1> R98, R03 or THM
			// value line will be ""Exif Interoperability version is CCC"
			pBase = (char *) & f->offset;
			sprintf_s(
				sValue, 80,
				"Exif Interoperability version is \"%c%c%c\"",
				(int) *pBase, (int)*(pBase + 1), (int)*(pBase + 2)
			);
			//
			pField = new(char[strlen(sValue) + 1]);
			strcpy_s(pField, strlen(sValue) + 1, sValue);
			f->value = pField;
			//
			logThis({ sMsg, 0, 0, sValue }, "");
			break;

		case 0x1B:
			// This the GPSProcessingMethod
			//
			// The first 8 bytes indicates the character code used
			// so this field can not be recorded inline using the offset
			// 4-bytes
			//
			// the value line will be "GPSProcessingMethod[n] (code) = "value"
			//
			// if the character code is not ASCII value will be "???"
			//
			n = f->count;	// anyway n>9 bytes
			updateHighestOffset(globalOffset + f->offset + f->count - 1);
			size = 80 + n;
			pField = (char *)::operator new(size);
			pTempChar = (char *)::operator new(size);
			pBase = (char *)(buffer + globalOffset + f->offset);
			memcpy_s(sValue, 80, pBase, 8);
			printf("\tsValue = [%s]\n", sValue);
			if ((*pBase) != 65)
			{
				sprintf_s(
					pField, size,
					"GPSProcessingMethod[%d]: (%s) = \"???\"",
					n, sValue
				);
			}
			else
			{ 
				// ok is ASCII
				pBase += 8;
				memcpy_s(pTempChar, size, pBase, (n - 8));
				*(pTempChar + n - 8) = 0;
				sprintf_s(
					pField, size,
					"GPSProcessingMethod[%d] (%s) = \"%s\"",
					n, sValue, pTempChar
				);
			}
			f->value = pField;
			n16 = 0;	// dummy break point
			delete pTempChar;
			break;

		case 0x9000:
			// 
			// value line will be ""Exif version is CCCC"
			// sure, version should be 231
			//
			pBase = (char *)& f->offset;
			sprintf_s(
				sValue,
				"Exif version is \"%c%c.%c%c\"",
				(char)*pBase,
				(char)*(pBase + 1),
				(char)*(pBase + 2),
				(char)*(pBase + 3)
			);
			//
			pField = new(char[strlen(sValue) + 1]);
			strcpy_s(pField, strlen(sValue) + 1, sValue);
			f->value = pField;
			//
			sprintf_s(sMsg, 80, "%04X", f->tag);
			logThis({ sMsg, 0, 0, sValue }, "");
			break;

		case 0x9101:
			// 
			// value line will be "4560" if RGB uncompressed
			// or other values for each channel
			//
			// output value line: "Components Configuration = [0xn1 0xn2 0xn3]
			pBase = (char *)& f->offset;
			sprintf_s(
				sValue,
				"Components Configuration = [0x%X 0x%X 0x%X]",
				(int)*pBase, (int)*(pBase + 1), (int)*(pBase + 2)
			);
			//
			pField = new(char[strlen(sValue) + 1]);
			strcpy_s(pField, strlen(sValue) + 1, sValue);
			f->value = pField;
			//
			logThis({ sMsg, 0, 0, sValue }, "");
			break;

		case 0xA000:
			// 
			// value line will be Flashpix Version
			//
			// output value line: "Flashpix Version = [VVVV]
			pBase = (char *)& f->offset;
			sprintf_s(
				sValue,
				"Flashpix Version = [%c%c.%c%c]",
				(int)*pBase, (int)*(pBase + 1), (int)*(pBase + 2), (int)*(pBase+3)
			);
			//
			pField = new(char[strlen(sValue) + 1]);
			strcpy_s(pField, strlen(sValue) + 1, sValue);
			f->value = pField;
			//
			logThis({ sMsg, 0, 0, sValue }, "");
			break;

		default:
			// 
			// here we have an undefined tag with type also undefined
			//
			// output value line: "Tag TTTT Type undefined. Offset is 0xNNN data is A B C D"
			pBase = (char *)& f->offset;
			sprintf_s(
				sValue,
				"Tag 0x%4X: Type UNDEFINED. Offset is 0x%X data is [%X] [%X] [%X] [%X]",
				f->tag,
				f->offset,
				(int)*pBase, (int)*(pBase + 1), (int)*(pBase + 2), (int)*(pBase + 3)
			);
			//
			pField = new(char[strlen(sValue) + 1]);
			strcpy_s(pField, strlen(sValue) + 1, sValue);
			f->value = pField;
			//
			logThis({ sMsg, 0, f->count, sValue }, "");
			break;
		}	// end switch(f->tag)
		break;

	case 9:
		//
		// 9: SLONG: SIGNED 32bit integer
		//

		// Value string is "(SLONG[n] A1 A2 ...)" no hex values this time
		//
		n = f->count;
		size = 30 * n + 50;
		pRational = (char *)::operator new(size); // 30 bytes for each [A/B] pair
		sprintf_s(pRational, size, "(SLONG)[%d]: ", n);
		pBase = (char *)(buffer + globalOffset + f->offset);	// pointing to start
		for (int i = 0; i < n; i++)
		{
			// build one
			memcpy_s(&slonga, 4, pBase, 4);
			pBase += 4;
			sprintf_s(pRational, size,
				"%s[%d]",
				pRational,
				slonga
			);
		}
		sprintf_s(pRational, size, "%s)", pRational);
		pField = pRational;
		f->value = pField;
		sprintf_s(sMsg, 80, "%04X", f->tag);
		logThis({ sMsg, (globalOffset + f->offset), (f->count * 4), sValue }, "");
		updateHighestOffset(globalOffset + f->offset + (4 * f->count) - 1);
		n16 = 0;	// dummy break point
		break;

	case 10:
		//
		// 10: srational: A/B/C... SIGNED
		//
		// each srational uses 2 4-byte signed longs where the first one is the numerator
		// and the second one is the denominator
		//
		// Value string is "(SRATIONAL[n] [A1/A2] [B1/B2]...)" no hex values this time
		//
		n = f->count;
		size = 30 * n + 50;
		pRational = (char *)::operator new(size); // 30 bytes for each [A/B] pair
		sprintf_s(pRational, size, "(SRATIONAL)[%d]: ", n);
		pBase = (char *)(buffer + globalOffset + f->offset);	// pointing to start
		for (int i = 0; i < n; i++)
		{
			// build one pair in sRational
			memcpy_s(&slonga, 4, pBase, 4);
			pBase += 4;
			memcpy_s(&slongb, 4, pBase, 4);
			pBase += 4;
			sprintf_s(pRational, size,
				"%s[%d / %d]",
				pRational,
				slonga, slongb
			);
		}
		sprintf_s(pRational, size, "%s)", pRational);
		pField = pRational;
		f->value = pField;
		sprintf_s(sMsg, 80, "%04X", f->tag);
		logThis({ sMsg, (globalOffset + f->offset), (f->count * 8), sValue }, "");
		updateHighestOffset(globalOffset + f->offset + (8 * f->count) - 1);
		n16 = 0;	// dummy break point
		break;

	}	// end switch
	//
	//printf("\ttag %X value is [%s]\n",
	//	f->tag,
	//	f->value
	//);
	n16 = 0; // do nothing
	return;
}	// end processField()

uint32_t TIFFfile::processIFD()
{
	short		nIFD;
	int			nEntries;
	uint32_t	offset;
	char		sMsg[80];
	char		sValue[80];
	IFDfield	tempField;

	//
	// here we are pointing to the IFD field count
	// confirm position
	//
	sprintf_s(sMsg, "IFD%d", currentIFD->index);
	//dumpFrom(sMsg, 40, buffer + next);
	sprintf_s(sValue, "processIFD%d, next @0x%X origin is 0x%X", currentIFD->index, next, currentIFD->origin);
	logThis({ sMsg, 0, 0, sValue }, "");
	memcpy(&b16, (buffer + next), 2);
	nEntries = b16;
	(*currentIFD).entries = nEntries;
	(*currentIFD).address = next;
	uint32_t base = next;
	next += 2;
	nIFD = (*currentIFD).index;

	sprintf_s(sMsg, "IFD%d", nIFD);
	sprintf_s(sValue, "Image File Directory IFD%d, %d fields", nIFD, nEntries);
	logThis({ sMsg, (next - 2), (uint32_t)(2 + (12 * nEntries) + 4), sValue }, "");

	// now we are pointing to the Field Array
	// nEntries 12-byte fields
	// create a first field, blank
	currentIFD->fieldList = nullptr;
	for (
		int i = 0;
		i < nEntries;
		i++)
	{
		// build field record
		tempField.nextField = nullptr;
		memcpy(&tempField.field.tag, buffer + next, 2);		// tag
		memcpy(&tempField.field.type, buffer + next + 2, 2);	// type
		memcpy(&tempField.field.count, buffer + next + 4, 4);	// count
		memcpy(&tempField.field.offset, buffer + next + 8, 4);	// offset
		sprintf_s(sMsg, "#%d/%d", i + 1, nEntries);
		sprintf_s(
			sValue,
			"At 0x%X: Tag 0x%X, Type 0x%X, Count 0x%X, Offset 0x%X",
			((*currentIFD).address + 2 + (12 * i)),
			tempField.field.tag,
			tempField.field.type,
			tempField.field.count,
			tempField.field.offset
		);
		// insert into FieldList;
		ifdSet.insertField(tempField, currentIFD);
		logThis({ sMsg, base, 12, sValue }, "");
		next += 12;
	}	// end for
	// now we are at the 4-byte next IFD offset value
	//dumpFrom( "offset?", 10, buffer+next);
	memcpy(&offset, buffer + next, 4);
	currentIFD->nextIFDoffset = offset;
	next += 4;
	updateHighestOffset(next - 1);
	//dumpIFDset(&ifdSet);
	// next IFD or back to scan?
	return (offset);
}	// end processIFD()

void TIFFfile::testDict(IFDlist *)
{
	uint16_t	t;
	FieldDict	field;
	TagMeans &	dict = ifdSet.exifTags;

	do
	{
		// get a key from user and search for that
		cout << "tag (decimal): ";
		cin >> t;
		cout << endl << "tag is " << t << endl;
		if( ! dict.explainTag( t, field))
		{
			cout << endl << "tag " << t << " was not found " << endl;
			continue;
		}
		// that was found
		dict.printTag(field);
	} while (t != 9999);
	// at the end dump them all
	dict.print();
}	// end testDict()

void TIFFfile::testLinks(IFDlist *)
{
	//
	// this is just a test to build and navigate a field list
	// built at testList()
	//
	testList(&ifdSet);
	printf("\n\n***** after testList() *****\n\n");
	printf(
	"\tIFD count is %d, Global offset is 0x%X (%d)\n\n",
	ifdSet.ifdCount,
	ifdSet.offsetBase,
	ifdSet.offsetBase
	);
	pIFD = ifdSet.ifd;
	for (int n = 0; n < ifdSet.ifdCount; n++)
	{
	printf(
	"\tIFD%d: %d entries. address 0x%X, index is %d\n",
	n,
	pIFD->entries,
	pIFD->address,
	pIFD->index
	);
	pIFD = pIFD->nextIFD;
	};
}	// end testLinks()

void TIFFfile::testList(IFDlist * list)
{
	IFD			ifd;
	IFD *		pIFD;
	IFDfield	f;
	IFDfield *	pField;

	printf("\ttestList():\n");

	// builds a new set of ifds
	list->offsetBase = 0x1234;
	list->ifdCount = 0;
	list->ifd = nullptr;

	// builds a new IFD
	ifd.address = 0;
	ifd.entries = 0;
	ifd.fieldList = nullptr;
	ifd.index = 0;
	ifd.nextIFD = nullptr;
	ifd.nextIFDoffset = 0;
	ifd.origin = 0;
	ifd.processed = false;

	// builds a new field
	f.field.tag = 0;
	f.field.type = 0;
	f.field.count = 0;
	f.field.offset = 0;

	dumpIFDset(list);

	for (
		int i = 0;
		i < 5;
		i++
		)
	{
		// fills *pIFD with data from ifd
		pIFD = (IFD *)::operator new(sizeof(IFD));
		(*pIFD) = ifd;
		(*pIFD).address = ifd.address + i + 1;
		printf(
			"\tifd: address %d, entries %d\n",
			(*pIFD).address,
			(*pIFD).entries
		);
		list->insertIFD( *pIFD, list );
		for (
			int j = 0;
			j < 5;
			j++
			)
		{
			f.field.tag = 0;
			pField = (IFDfield *)::operator new(sizeof(IFDfield));
			(*pField) = f;
			printf(
				"\tfield: tag %d, type %d\n",
				(*pField).field.tag,
				(*pField).field.type
			);
			list->insertField(*pField, list->ifd);
		}	// end for j
	}	// end for i
	dumpIFDset(list);

	// now navigate ifd and insert a few more fields
	printf("\tPhase 2. IFD count is %d\n",
		list->ifdCount);

	pIFD = list->ifd->nextIFD;	// points to second
	for (int i = 2; i <= list->ifdCount; i++)
	{
		for (
			int j = 0;
			j < 12;
			j++
			)
		{
			f.field.tag = 8*j;
			f.field.type = pIFD->index;
			f.field.count = (pIFD->entries) + 1;
			f.field.offset = j + 1;
			pField = (IFDfield *)::operator new(sizeof(IFDfield));
			(*pField) = f;
			list->insertField(*pField, pIFD );
		}	// end for j
		pIFD = pIFD->nextIFD;
	}
	dumpIFDset(list);
	return;
}   // end testList()

void TIFFfile::updateHighestOffset(uint32_t offset)
{
	lastAccessed = offset - 1;
	if (lastAccessed > highestAccessed) { highestAccessed = lastAccessed; };
}	// end updaHighestOffset()

