#include "stdafx.h"
#include "IFDlist.h"


IFDlist::IFDlist()
{
	offsetBase = 0;
	ifdCount = 0;

	ifd = (IFD *)::operator new(sizeof(IFD));
	ifd->address = -1;
	ifd->entries = 0;
	ifd->fieldList = nullptr;
	ifd->index = -1;
	ifd->lastField = nullptr;
	ifd->nextIFD = nullptr;
	ifd->nextIFDoffset = 0;
	ifd->processed = false;
}


IFDlist::~IFDlist()
{
}





IFDfield * IFDlist::insertField(
	IFDfield field,
	IFD * pIFD
)
{
	IFDfield * f = (IFDfield *)::operator new(sizeof(IFDfield));
	*f = field;

	// insert a field *f into field array *pIFD
	f->nextField = nullptr;	// just to be sure
	if ( pIFD->fieldList == nullptr )
	{
		// first field
		pIFD->fieldList = f;
		pIFD->entries = 1;
		pIFD->lastField = f;
	}
	else
	{
		// 
		// lastField points to tail: just link this new one
		//
		IFDfield * p = pIFD->lastField;
		p->nextField = f;
		pIFD->lastField = f;
		pIFD->entries = pIFD->entries + 1;
	}
	return f;
};	// end insertIield()





IFD * IFDlist::insertIFD(
	IFD ifd,
	IFDlist * s
)
{
	IFD * f = (IFD *)::operator new(sizeof(IFD));
	*f = ifd;

	f->nextIFD = nullptr;	// this will be the last	
	if (s->ifd == nullptr)
	{
		// first ifd
		s->ifdCount = 1;
		s->ifd = f;
		s->ifd->index = s->ifdCount - 1;	// first IFD is IFD0
		s->lastIFD = f;
	}
	else
	{
		// not the first. Use pointer lastIFD
		IFD * p = s->lastIFD;
		p->nextIFD = f;
		s->ifdCount = s->ifdCount + 1;
		f->index = s->ifdCount - 1;
		s->lastIFD = f;
	}
	return f;
};	// end insertIFD()
