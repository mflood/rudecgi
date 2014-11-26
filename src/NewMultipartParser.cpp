// NewMultipartParser.cpp
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


// NOTES - pass in NON - BINARY MODE DATA and BINARY MODe DATA -
// to test AUTO-DETECT NEWLINE STUFF!!!
//


#include "NewMultipartParser.h"

#ifndef INCLUDED_DataRepos_H
#include "DataRepos.h"
#endif

#ifndef INCLUDED_FileData_H
#include "FileData.h"
#endif

#ifndef INCLUDED_CTYPE_H
#include <ctype.h>
#define INCLUDED_CTYPE_H
#endif

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif


// DEBUG
//#include <stdio.h>;

namespace rude{
namespace cgiparser{


NewMultipartParser::NewMultipartParser(const char *boundary)
{
	d_boundary=boundary ? boundary : "";
}


NewMultipartParser::~NewMultipartParser()
{

}

void NewMultipartParser::setBoundary(const char *boundary)
{
	d_boundary=boundary ? boundary : "";
}

// NOTE: the original buffer gets mangled!!
//
bool NewMultipartParser::parse(DataRepos *repository, char *data, int length)
{
		//printf("Content-type: text/html\n\n");

		// Build --<boundary>
		//
		std::string realboundary="--";
		realboundary+= d_boundary;

		// we're going to search for boundaries one character at at time,
		// so record the current position we're looking at here:
		// we can't use a tokenizer, because the data may contain null bytes.
		
		int boundarypos=0;
		unsigned int datalength = 0;
		char *datastart=0;
		
		for(int x=0; x < length; x++)
		{
			//printf("%c", data[x]);
			if(data[x] == realboundary[boundarypos++])
			{
				if(realboundary[boundarypos] == 0)
				{
					// we have found a boundary
					////////////////////////////
					//printf("\n<br>*****Found boundary!****<br>\n");
					if(datastart == (char *)0)
					{
						// its the first boundary
						// do nothing with stuff we already have..
					}
					else
					{

						// the data length we have been computing includes
						// the length of the boundary we just read - so subtract
						// the length of the boundary from the datalength
						datalength -= realboundary.length();
						
						if(datalength < 0)
						{
							// should return false here - something terrible has happened
							// if the datalength is less than 0
							datalength = 0;
						}
						
						// We have stripped the newlines at the beginning of the data,
						// so the data starts with the first header.
						// we will not strip off the trailing newlines,
						// we will delegate that responsibility to the data parser
						//
						//printf("\n<br>*****Sending %d bytes to parser*****<br>\n", datalength);
						//printf("'");
						//fwrite(datastart, sizeof(char), datalength, stdout);
						//printf("'");
						
						parseSection(repository, datastart, datalength);
					}

					// see if this is was last boundary
					//
					if(x + 2 > length)
					{
						// no data left to read, must be...
						//printf("\n<br>*****FOUND THE LAST BOUNDARY - returning*****</br>\n");
						// Should actually return false, 
						// since we ran out of data before last boundary was found.....
						return true;
						
					}
					else if(data[x+1] =='-' && data[x+2] =='-')
					{
						// yes - return
						//printf("\n<br>*****FOUND THE LAST BOUNDARY - returning*****</br>\n");
						return true;
					}

					// get rid of newline chars that follow this boundary.
					// 
					while( data[x+1] == '\n' || data[x+1] == 0x0a || data[x+1] == 0x0d)
					{
						x++;
					}

					// Reset boundary position so we can start looking for the next one
					// 
					boundarypos=0;
					datastart=&data[x+1];
					datalength=0;
				}
				
			}
			else
			{
				// reset boundary search
				boundarypos=0;
			}
			datalength++;
		}
		return true;
}



void NewMultipartParser::parseSection(DataRepos *repository, char *data, int length)
{
		// This method takes a chunk of data assumed to be a section in
		// a multipart form.  It parses the header and determines the beginning and
		// end of the data, and converts these findings into a FileData object.
		// Once the FileData object is set-up, this function stores it in the repository.
		//////////////////
		


		// Create the FileData object that we will populate
		//
		FileData *parseElem=new FileData();

		unsigned int datalength=length;
		int linelength;
		
		// parse headers one line at a time
		// 
		while(   (  linelength = strcspn(data, "\r\f\n")    )     )
		{
			// figure out what the first character of newline is
			// and save it for later use
			//
			char CR = data[linelength];
			
			// null terminate this line
			data[linelength] = 0;

			//printf("\n<br>*****Found header line: '%s'*****<br>\n", data);

			// examine this header line for facts
			//
			// we have to search for all up front (from index 0),
			// since more than one can appear on the same line....
			//
			// POSSIBLE BUG SOURCE: Case senstivity of these identifiers..
			// 
			char *filenameptr=strstr(data, " filename=");
			char *nameptr=strstr(data, " name=");
			char *contentptr=strstr(data, "Content-Type");
					
			char *tempptr;
			if( filenameptr )
			{
				// find first quote
				tempptr=strstr(filenameptr, "\"");
				if(tempptr)
				{
					filenameptr=++tempptr;
				}
				// find end quote
				tempptr=strstr(filenameptr, "\"");
				if(tempptr)
				{
					*tempptr=0;
				}
				parseElem->setFilename(filenameptr);
			}
			if( nameptr )
			{
				// find first quote
				tempptr=strstr(nameptr, "\"");
				if(tempptr)
				{
					nameptr=++tempptr;
				}
				// find end quote
				tempptr=strstr(nameptr, "\"");
				if(tempptr)
				{
					*tempptr=(char) NULL;
					
				}
				parseElem->setFieldName(nameptr);
			}			
			if(contentptr)
			{
				// find first space
				tempptr=strstr(contentptr, " ");
				if(tempptr)
				{
					contentptr=++tempptr;
				}
				// find end quote
				int seglength=strcspn(contentptr, ";");
				tempptr+=seglength;
				*tempptr=(char) NULL;
				parseElem->setContentType(contentptr);
				////printf("Found CONTENTTYPE: '<B>%s</B>'<BR>", contentptr);
			}


			// reset data start to just beyond the null character
			// we set in the first line
			//
			datalength -= (linelength +1);
			data += linelength +1;
			// if we are sitting on top of the LF section of a CRLF, we need to move past it
			if(isspace(*data) && *data != CR)
			{
				data++;
				datalength--;
			}
			if(datalength < 0)
			{
				datalength=0;
			}
		} // end of header parsing loop

		// chomp off the newlines at the beginning and end
		//
		// POSSIBLE BUG SOURCE: what if newlines are at the start or end of the actual data??
		// 
		//while(datalength && (*data == '\n' || *data  == 0x0a || *data == 0x0d))
		//{
		//		std::cerr << __FILE__ << ":" <<__LINE__ << ":removed Beginning Newline\n"; 
		//		data++;
		//		datalength--;
		//}
		//while(datalength && (data[datalength-1] == '\n' || data[datalength-1]  == 0x0a || data[datalength-1] == 0x0d ))
		//{
		//	std::cerr << __FILE__ << ":" <<__LINE__ << ":removed Trailing Newline\n"; 
		//	datalength--;
		//}

                // New way to chomp - take 2 off each end and call it a day
		//
                for(int a=0; a< 2; a++)
		{
			if(datalength)
			{
				data++;
				datalength--;
			}
		} 

                for(int a=0; a< 2; a++)
		{
			if(datalength)
			{
				datalength--;
			}
		} 

		parseElem->setValue(data, datalength);
		repository->addDataObject(parseElem);
}
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

}}
