//CommandLineExtractor.cpp
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




#include "CommandLineExtractor.h"

#ifndef INCLUDED_DataRepos_H
#include "DataRepos.h"
#endif

#ifndef INCLUDED_Interactive_H
#include "Interactive.h"
#endif

#ifndef INCLUDED_URLEncodeParser_H
#include "URLEncodeParser.h"
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

namespace rude{
namespace cgiparser{

ClientData *CommandLineExtractor::extract()
{
	std::string command;
	
	std::cout << "Enter querystring or type 'i' for interactive mode'\n";
	std::getline(std::cin, command);
	if(command[0] == 'i')
	{
		// return interactive mode client data object
		Interactive *interactive = new Interactive();
		return interactive;
	}
	else
	{
		// build normal client data object

		DataRepos *repository = new DataRepos();
		URLEncodeParser parser;

		int length = command.length();
		char *buffer = new char[length + 1];
		strcpy(buffer, command.c_str());
		
		parser.parse(repository, buffer, length);
		
		delete [] buffer;
		
		return repository;
	}
}

}} // end namespaces

