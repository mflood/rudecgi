// Interactive.cc
//
// Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005 Matthew Flood
// See file AUTHORS for contact information
//
// This file is part of RudeCGI.
//
// RudeCGI is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
// 
// RudeCGI is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with RudeCGI; (see COPYING) if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//------------------------------------------------------------------------



#include "Interactive.h"

#ifndef INCLUDED_AbstractParser_H
#include "AbstractParser.h"
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifndef INCLUDED_STDLIB_H
#include <stdlib.h>
#define INCLUDED_STDLIB_H
#endif

namespace rude{
namespace cgiparser{

std::string inputstring;

void Interactive::setCaseSensitive(bool iscase)
{
	std::cout << "Case sensitivity has been turned ";
	if(iscase)
	{
		std::cout << "on\n";
	}
	else
	{
		std::cout << "off\n";
	}
}

int Interactive::numTotal()  const
{
		std::cout << "Enter the number of data objects that exist:";
		std::getline(std::cin, inputstring);
		int retval = atoi(inputstring.c_str());
		return retval;
}

const char *Interactive::fieldAt(int index) const
{
	std::cout << "Please specify the fieldname at position "<< index << " (from 0)\n";
	std::getline(std::cin, inputstring);
	return inputstring.c_str();
}

bool Interactive::exists(const char *fieldname) const
{
	std::cout << "Does the field '" << fieldname << "' exist? (y|n)";
		std::getline(std::cin, inputstring);

	if(inputstring == "y")
	{
		std::cout << "Returning true\n";
		return true;
	}
	else
	{
		std::cout << "Returning false\n";
		return false;
	}
}

int Interactive::numValues(const char *fieldname) const
{
	if(fieldname)
	{
		std::cout << "Enter the number of values that exist for fieldname '" << fieldname << "'\n";
		std::getline(std::cin, inputstring);
		int retval = atoi(inputstring.c_str());
		return retval;
	}
	else
	{
		std::cout << "The program asked the number of values for a NULL fieldname";
		return 0;
	}
}

const char *Interactive::value(const char *fieldname, int position) const
{
	std::cout << "Please specify the value for field '"<< fieldname <<"'  at position "<< position << " (from 0)\n";
	
	std::getline(std::cin,inputstring);

	char *buffer = new char[inputstring.length()+ 1] ;
	strcpy(buffer, inputstring.c_str());

	AbstractParser::unescape(buffer);
	inputstring = buffer;
	
	delete [] buffer;

	return inputstring.c_str();
}

const char *Interactive::value(int index) const
{
	std::cout << "Please specify the value for field at index'"<< index << " (from 0)\n";
	std::getline(std::cin,inputstring);

	char *buffer = new char[inputstring.length()+ 1] ;
	strcpy(buffer, inputstring.c_str());

	AbstractParser::unescape(buffer);
	inputstring = buffer;
	
	delete [] buffer;

	return inputstring.c_str();
}

const char *Interactive::filepath(const char *fieldname, int position) const
{
	std::cout << "Please specify the filepath for field '"<< fieldname <<"'  at position "<< position << " (from 0)\n";
	std::cout << "OR enter NULL for none";
	std::getline(std::cin,inputstring);

	if(inputstring == "NULL")
	{
		return 0;
	}
	return inputstring.c_str();

}

const char *Interactive::filepath(int index) const
{
	std::cout << "Please specify the filepath at position "<< index << " (from 0)\n";
	std::cout << "OR enter NULL for none";
	std::getline(std::cin,inputstring);

	if(inputstring == "NULL")
	{
		return 0;
	}
	return inputstring.c_str();

}

const char *Interactive::filename(const char *fieldname, int position) const
{
	std::cout << "Please specify the filename for field '"<< fieldname <<"'  at position "<< position << " (from 0)\n";
	std::cout << "OR enter NULL for none";
	std::getline(std::cin,inputstring);

	if(inputstring == "NULL")
	{
		return 0;
	}
	return inputstring.c_str();
}

const char *Interactive::filename(int index) const
{
	std::cout << "Please specify the filename for field at position "<< index << " (from 0)\n";
	std::cout << "OR enter NULL for none";
	std::getline(std::cin,inputstring);

	if(inputstring == "NULL")
	{
		return 0;
	}
	return inputstring.c_str();
}

const char *Interactive::contentType(const char *fieldname, int position) const
{
	std::cout << "Please specify the content-type for field '"<< fieldname <<"'  at position "<< position << " (from 0)\n";
	std::cout << "OR enter NULL for none";
	std::getline(std::cin,inputstring);

	if(inputstring == "NULL")
	{
		return 0;
	}
	return inputstring.c_str();
}

const char *Interactive::contentType(int index) const
{
	std::cout << "Please specify the content-type for field at position "<< index << " (from 0)\n";
	std::cout << "OR enter NULL for none";
	std::getline(std::cin, inputstring);

	if(inputstring == "NULL")
	{
		return 0;
	}
	return inputstring.c_str();
}

int Interactive::length(const char *fieldname, int position) const
{
	if(fieldname)
	{
		std::cout << "Enter the length of the value for fieldname '" << fieldname << " at position " << position <<" (from 0)'\n";
		std::getline(std::cin, inputstring);
		int retval = atoi(inputstring.c_str());
		return retval;
	}
	else
	{
		std::cout << "The program asked the length of a field's value for a NULL field";
		return 0;
	}
}
int Interactive::length(int index) const
{
	std::cout << "Enter the length of the value for fieldname at position " << index <<" (from 0)'\n";
		std::getline(std::cin, inputstring);
		int retval = atoi(inputstring.c_str());
		return retval;
}

const char * Interactive::datasource(int index) const
{
	std::cout << "Enter the datasource of the value (cookie, path, form) for fieldname at position " << index <<" (from 0)'\n";
	std::getline(std::cin,inputstring);

	if(inputstring == "")
	{
			inputstring = "form";
	}
	return inputstring.c_str();
}
const char * Interactive::datasource(const char *fieldname, int position) const
{
	std::cout << "Enter the datasource of the value for fieldname '" << fieldname << " at position " << position <<" (from 0)'\n";
	std::getline(std::cin,inputstring);

	if(inputstring == "")
	{
		inputstring = "form";
	}
	return inputstring.c_str();
}

}} // end namespaces

