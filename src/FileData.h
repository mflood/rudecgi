// FileData.h
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



#ifndef INCLUDED_FileData_H
#define INCLUDED_FileData_H

#ifndef INCLUDED_DataObject_H
#include "DataObject.h"
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace rude{
namespace cgiparser{


class FileData: public DataObject{

	static int s_maxfilename;
	static const char *s_defaultcontenttype;

	std::string d_name;
	std::string d_contenttype;
	std::string d_filename;
	std::string d_filepath;
	int d_length;
	char *d_value;

protected:
	

public:

	static void setDefaultContentType(const char *contenttype);
	static void setMaxFilename(int maxlength);

	FileData();
	virtual const char *getFieldName() const;
	virtual const char *getValue() const;
	virtual int getLength() const;
	virtual const char *getFilepath() const;
	virtual const char *getFilename() const;
	virtual const char *getContentType() const;

	void setFieldName(const char *name);
	void setValue(const char *value, int length);
	void setFilepath(const char *path);
	void setFilename(const char *name);
	void setContentType(const char *contenttype);
	const char *datasource() const;
	virtual ~FileData();
};
}}
#endif

