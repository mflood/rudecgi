// DataRepos.h
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




#ifndef INCLUDED_DataRepos_H
#define INCLUDED_DataRepos_H

#ifndef INCLUDED_ClientData_H
#include "ClientData.h"
#endif

#ifndef INCLUDED_VECTOR
#include <vector>
#define INCLUDED_VECTOR
#endif

namespace rude{
namespace cgiparser{

class DataObject;
class WordMatch;

class DataRepos: public ClientData{

	bool d_case;
	WordMatch *d_wordmatcher;
	std::vector<DataObject*> d_elements;
	
public:

	DataRepos();
	void setWordMatch(WordMatch *matcher);
	void setCaseSensitive(bool iscase);
	
	int numTotal() const;
	const char *fieldAt(int index) const;
	int numValues(const char *fieldname) const;
	const char *value(int index) const;
	const char *value(const char *fieldname, int position) const;
	const char *filepath(int index) const;
	const char *filepath(const char *fieldname, int position) const;
	const char *filename(int index) const;
	const char *filename(const char *fieldname, int position) const;
	const char *contentType(int index) const;
	const char *contentType(const char *fieldname, int position) const;
	virtual int length(int index) const;
	int length(const char *fieldname, int position) const;

	const char *datasource(int index) const;
	const char *datasource(const char *fieldname, int position) const;
	bool exists(const char *fieldname) const;
	
	void addDataObject(DataObject *o);

	~DataRepos();

};

}} // end namespaces

#endif

