// DataRepos.cc
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



#include "DataRepos.h"

#ifndef INCLUDED_DataObject_H
#include "DataObject.h"
#endif

#ifndef INCLUDED_CaseMatch_H
#include "CaseMatch.h"
#endif

#ifndef INCLUDED_CaselessMatch_H
#include "CaselessMatch.h"
#endif

namespace rude{
namespace cgiparser{

DataRepos::DataRepos()
{
	d_wordmatcher = new CaseMatch();
	d_case = true;
}


void DataRepos::setWordMatch(WordMatch *matcher)
{
	if(matcher)
	{
		delete d_wordmatcher;
		d_wordmatcher = matcher;
	}
}

void DataRepos::setCaseSensitive(bool iscase)
{
	if(d_case != iscase)
	{
		d_case = iscase;
		delete d_wordmatcher;
		if(iscase)
		{
			d_wordmatcher = new CaseMatch();
		}
		else
		{
			d_wordmatcher= new CaselessMatch();
		}
	}
}

DataRepos::~DataRepos()
{
	std::vector<DataObject*>::iterator iter;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		delete (*iter);
	}
	delete d_wordmatcher;
}
	
int DataRepos::numTotal() const
{
	return d_elements.size();
}

const char *DataRepos::fieldAt(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getFieldName())
			{
				return (*iter)->getFieldName();
			}
			return "";
		}
		x++;
	}
	return 0;
}

int DataRepos::numValues(const char *fieldname) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			x++;
		}
	}
	return x;		
}
	
bool DataRepos::exists(const char *fieldname) const
{
	std::vector<DataObject*>::const_iterator iter;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			return true;
		}
	}
	return false;
}

const char *DataRepos::value(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getValue())
			{
				return (*iter)->getValue();
			}
			return "";
		}
			x++;
	}
	return "";		
}
	
const char *DataRepos::value(const char *fieldname, int position) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			if(x == position)
			{
				if((*iter)->getValue())
				{
					return (*iter)->getValue();
				}
				return "";
			}
			x++;
		}
	}
	return "";		
}

const char *DataRepos::filepath(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getValue())
			{
				return (*iter)->getFilepath();
			}
			return "";
		}
			x++;
	}
	return "";		
}

const char *DataRepos::filepath(const char *fieldname, int position) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			if(x == position)
			{
				return (*iter)->getFilepath();
			}
			x++;
		}
	}
	// returning NULL!!!
	//
	return 0;		
}

const char *DataRepos::filename(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getValue())
			{
				return (*iter)->getFilename();
			}
			return "";
		}
			x++;
	}
	return "";		
}

const char *DataRepos::filename(const char *fieldname, int position) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			if(x == position)
			{
				return (*iter)->getFilename();
			}
			x++;
		}
	}
	// returning empty string!!!!
	// instead of NULL!!!
	//
	return 0;		
}

const char *DataRepos::contentType(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getValue())
			{
				return (*iter)->getContentType();
			}
			return "";
		}
			x++;
	}
	return "";		
}

const char *DataRepos::contentType(const char *fieldname, int position) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			if(x == position)
			{
				return (*iter)->getContentType();
			}
			x++;
		}
	}
	// returning empty string!!!!
	// instead of NULL!!!
	//
	return 0;		
}

int DataRepos::length(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getValue())
			{
				return (*iter)->getLength();
			}
			return 0;
		}
			x++;
	}
	return 0;		
}


int DataRepos::length(const char *fieldname, int position) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			if(x == position)
			{
				return (*iter)->getLength();
			}
			x++;
		}
	}
	return 0;		
}

const char *  DataRepos::datasource(int index) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(x == index)
		{
			if((*iter)->getValue())
			{
				return (*iter)->datasource();
			}
			return "";
		}
			x++;
	}
	return "";		
}

const char * DataRepos::datasource(const char *fieldname, int position) const
{
	std::vector<DataObject*>::const_iterator iter;
	int x = 0;
	for(iter = d_elements.begin(); iter != d_elements.end(); iter++)
	{
		if(d_wordmatcher->compare( (*iter)->getFieldName(), fieldname ) )
		{
			if(x == position)
			{
				return (*iter)->datasource();
			}
			x++;
		}
	}
	return 0;		
}

	
void DataRepos::addDataObject(DataObject *o)
{
	d_elements.push_back(o);
}

}} // end namespaces

