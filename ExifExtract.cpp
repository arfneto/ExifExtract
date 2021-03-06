// fixjpg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "TIFFfile.h"

using namespace std;

int main(
	int argc,
	char ** argv)
{
	string		filename;

	if (argc < 2)
	{
		return 1;
	}
	std::cout << "\n" << argv[0] << ": file is \"" << argv[1] << "\"\n" << endl;;
	filename = argv[1];

	TIFFfile	work(filename);

	work.fileLoad();
	work.dumpIFDset(&work.ifdSet);
	work.dumpTags("d:\\Public\\test.txt");
	return(0);
}