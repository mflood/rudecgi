// FileData.cc
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



#include "FileData.h"

#ifndef INCLUDED_STRING_H
#include <string.h>
#define INCLUDED_STRING_H
#endif

#ifndef INCLUDED_CTYPE_H
#include <ctype.h>
#define INCLUDED_CTYPE_H
#endif

namespace rude{
namespace cgiparser{
const char *FileData::s_defaultcontenttype = "application/octet-stream";

int FileData::s_maxfilename=100;

void FileData::setDefaultContentType(const char *contenttype)
{
	s_defaultcontenttype = contenttype;
}
void FileData::setMaxFilename(int maxlength)
{
	s_maxfilename = maxlength;
}

FileData::FileData()
{
	d_length = 0;
	d_value = 0;
	
	d_name="";
	d_contenttype="";
	d_filename="";
	d_filepath="";
}
	
FileData::~FileData()
{
	delete [] d_value;
}

const char *FileData::getFieldName() const
{
	return d_name.c_str();
}
	
const char *FileData::getValue() const
{
	if(d_value)
	{
		return d_value;
	}
	return "";
}
		
int FileData::getLength() const
{
	return d_length;
}

const char *FileData::getFilepath() const
{
	return d_filepath.c_str();
}
	
const char *FileData::getFilename() const
{
	return d_filename.c_str();
}
	
const char *FileData::getContentType() const
{
	if(d_contenttype != "")
	{
		return d_contenttype.c_str();
	}
	return s_defaultcontenttype;
}

void FileData::setFieldName(const char *name)
{
	d_name = name? name: "";
}
	
void FileData::setValue(const char *value, int length)
{
	char *newvalue = 0;
	d_length = 0;
	if(value)
	{	d_length = length;
		newvalue = new char[length+1];
		for(int x=0; x< length; x++)
		{
			newvalue[x] = value[x];
		}
		newvalue[length] = 0;
	}
	delete [] d_value;
	d_value = newvalue;
}
	
void FileData::setFilepath(const char *path)
{
	if(path)
	{
		d_filepath = path;
		if(d_filename == "")
		{
			setFilename(path);
		}
	}
	else
	{
		d_filepath = "";
	}
}
	
void FileData::setFilename(const char *name)
{
	// eliminate illegal characters
	if(name)
	{
		int length = strlen(name);
		// make enough space to hold entire name, if need be
		//
		char *newname = new char[length + 1];

		// copy the incoming name to newname
		//
		strcpy(newname, name);

		// begin will be the start of the word
		//
		// look for the last forward slash
		//
		char *begin = strrchr(newname, '/');
		if(begin)
		{
			// found forward slash!!
			begin++;
			length = strlen(begin);
		}
		else
		{
			begin = strrchr(newname, '\\');
			if(begin)
			{
				// found backward slash
				begin++;
				length=strlen(begin);
			}
			else
			{
				// no slashes, point to beginning of name
				begin = newname;
			}
		}

			
		int x = 0;
		char *ptr = newname;
		
		// keep the filename down to the right length
		//
		if(s_maxfilename && length > s_maxfilename)
		{
			// increment x so we start where it will yield maxfilename characters
			//
			x += length - s_maxfilename;
		}
			
		for(; x < length ; x++)
		{
			// keep regular characters
			//
			if(isalnum(begin[x]) || begin[x] == '.')
			{
				*ptr = begin[x];
				ptr++;
			}
			else if(begin[x] == ' ')
			{
					// spaces become underscores
					//
					*ptr = '_';
					ptr++;
			}
			// everything else is ignored
		}
			
		// NULL terminate the newname
		//
		*ptr = 0;

		// assign it to string object
		//
		d_filename = newname;

		// reap memory
		//
		delete newname;
	}
	else
	{
		d_filename = "";
	}
}

const char *FileData::datasource() const
{
	return "form";
}

void FileData::setContentType(const char *contenttype)
{
	d_contenttype = contenttype? contenttype: "";
}

}} // end namespaces

