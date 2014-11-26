// cgi.h
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



#ifndef INCLUDED_RUDE_CGI_H
#define INCLUDED_RUDE_CGI_H

#include <iostream>

namespace rude{

namespace cgiparser{
class AbstractImplementation;
}

//=
// This is the public interface to the entire component. 
// Since it is foreseen that the interface will include cookie and path gathering info in the future, (oh - update - it does now!)
// the interface is separated from its implementation using the bridge pattern.
// <p>
//
// <h3>1. Optional Methods to call first</h3>
// 
// These methods configure the <b>CGI</b> component before it parses any incoming data. 
// As such, they must be called before the first call to {@link #instance CGI::instance()}, which triggers the parsing. 
// Calling these methods after the parsing occurs has no effect.
// <p>
// <ul>
// <li>{@link #parseCookies CGI::parseCookies(bool)}</li>
// <li>{@link #parsePath CGI::parsePath(bool)}</li>
// <li>{@link #parsePathMap CGI::parsePathMap(bool)}</li>
// <li>{@link #setPathMapVoid CGI::setPathMapVoid(const char *emptyname)}</li>
// <li>{@link #addPathMapName CGI::addPathMapName(const char *pathname)}</li>
// <li>{@link #setPathDelimiter CGI::setPathDelimiter(char)}</li>
// <li>{@link #maxPostLength CGI::maxPostLength(int bytes)}</li>
// </ul>
//
// 
//
// <h3>2. Obtaining the CGI object</h3>
// After all optional instance methods are called, it is safe to access the single instance of the CGI object. 
// Since the CGI object uses the Singleton design pattern, it is globally accessable from everywhere in your program. 
// However, since only one instance ever exists - don't delete it, unless you are absolutely sure your program is done with it.
// <p>
// <ul>
// <li>{@link #instance CGI::instance()}</li>
// </ul>
//
// 
//
// <h3>3. Using the CGI object  (exploratory methods)</h3>
//
// After obtaining the CGI object, the following methods allow you to find out what data was sent..
// <p>
// <ul>
// <li>{@link #setCaseSensitive setCaseSensitive(bool)}</li>
// <li>{@link #numValues numValues()}</li>
// <li>{@link #numValues(constchar*) numValues(const char *fieldname)}</li>
// <li>{@link #exists exists(const char *fieldname)}</li>
// <li>{@link #fieldnameAt fieldnameAt(int index)}</li>
// </ul>
//
// 
// 
// <h3>4. Using the CGI object (data methods)</h3>
//
// The rest of the methods deal with accessing a particular piece of data, and they all exist in three forms.
// The first form, <code><b>method(int index)</b></code>, accesses a data-pair at a specified index within the collection, regardless of the fieldname.
// The second form, <code><b>method(const char *fieldname)</b></code>, accesses the first data-pair identified by fieldname, 
// in the likely event that only one data-pair with the given fieldname was submitted. 
// For those cases where multiple values were submitted with the same identifying fieldname, the third form,
// <code><b>method(const char *fieldname, int position)</b></code>, allows access to each data-pair associated with the name.
//<p>
// <ul>
// <li>{@link #value}</li>
// <li>{@link #datasource}</li>
// <li>{@link #isFile}</li>
// <li>{@link #length}</li>
// <li>{@link #contenttype}</li>
// <li>{@link #filepath}</li>
// <li>{@link #filename}</li>
// </ul>
//=
class CGI{

	static rude::cgiparser::AbstractImplementation *s_implementation;

protected:

public:

	//=
	// Returns the Singleton instance of the <b>CGI</b> object.
	// The first call to this method triggers the parsing of all incoming data.
	// Subsequent calls to this method simply return the initialized <b>CGI</b> instance.
	// <p>
	// <b>Example:</b>
	// <p>
	// <code>
	// CGI parser;
	// </code>
	//=
	CGI();
	
	static void finish();

	//=
	// Returns the version of the CGI library
	//=
	static const char *version();
	//=
	// Sets the delimiter used between the name and value of data stored in PATH_INFO (default is '<b>=</b>').
	// <p>
	// So, for example, if PATH_INFO was: <p>
	// <p>
	// <code><b>
	// /color=light%20blue/age=16/sessionid=429842
	// </b></code>
	// <p>
	// Then, the fields and values extracted from PATH_INFO would be:
	// <p>
	// <table border=1>
	// <tr><th>Name</th><th>Value</th></tr>
	// <tr><td>color</td><td> light blue</td></tr>
	// <tr><td>age</td><td>16</td></tr>
	// <tr><td>sessionid</td><td>429842</td></tr>
	// </table>
	// <p>
	// If you expected the data to be stored in the following manner instead:
	// <p>
	// <code><b>
	// /color:light%20blue/age:16/sessionid:429842
	// </b></code>
	// <p>
	// Then you would set the delimiter to '<b>:</b>'.
	// <p>
	// If the delimiter is set to NULL, then path data is not extracted by delimiters, only the PathMap if it exists.
	// <p>
	//=
	static void setPathDelimiter(char delimiter);
	//=
	// Appends a pathname to the set of path names. This allows mapping from PATH_INFO directly into data--pairs.
	// <p>
	// For example, instead of
	// <p>
	// <code>http://mydomain.com/mypage?color=red&age=16&name=mark</code>
	// <p>
	// You can use PATH_INFO, and map the fieldnames color, age, and name directly to the path,
	// <p>
	// <code>CGI::addPathMapName("color");</code><br>
	// <code>CGI::addPathMapName("age");</code><br>
	// <code>CGI::addPathMapName("name");</code><br>
	// <p>
	// And use the following url instead:
	// <p>
	// <code>http://mydomain.com/mypage/red/16/mark</code>
	//=
	static void addPathMapName(const char *pathname);

	//=
	// Use this method to set a value in the path that is to be interpreted as empty data (default is "<b>default</b>").
	// <p>
	// So, for example, if you set up the path map as follows:
	// <p>
	// <pre><code><b>
	// CGI::addPathMapName("color");
	// CGI::addPathMapName("age");
	// CGI::addPathMapName("name");
	// </b></code></pre>
	// <p>
	// Then, given the following url:
	// <p>
	// <code><b>
	// http://mydomain.com/program.exe/red/<font color=blue>default</font>/mark
	// </b></code>
	// <p>
	// Then the following data will be extracted:
	// <p>
	// <table border=1>
	// <tr><th>Name</th><th>Value</th></tr>
	// <tr><td>color</td><td>red</td></tr>
	// <tr><td>name</td><td>mark</td></tr>
	// </table>
	// <p>
	// <b>Note:</b> <i>age</i> is ignored, since the empty value "<font color=blue><b>default</b></font>" was provided )
	// <p>
	//=
	static void setPathMapVoid(const char *emptyname);

	//=
	// Setting this to <b>true</b> enables the parsing of PATH_INFO using a pathmap. By default, this is set to <b>false</b>.<br>
	// If set to true, parsing by <i>path map</i> occurs after parsing by <i>delimiter</i>. 
	// Paths are mapped only after failing to be parsed by delimiter - either by not haveing the delimiter present in the path section, 
	// or having {@link #parsePath parsePath()} set to false.
	// <p>
	// <b>Example:</b>
	// <p>
	// <code>
	// CGI::parsePathMap( true );
	// </code>
	//<p>
	//=
	static void parsePathMap(bool shouldParse);
	//=
	// Enable / disable parsing of PATH_INFO (default enabled). 
	// Setting this to <b>false</b> disables the parsing of PATH_INFO using a character delimiter. 
	// If the delimiting character is not present, the path info is ignored. 
	// If the delimiting character is not present and {@link #parsePathMap parsePathMap()} is enabled, 
	// then the path info for the section in question is Mapped to the next pathmap index.
	// <p>
	// <b>Example:</b>
	// <p>
	// <code>
	// CGI::parsePath( false );
	// </code>
	//<p>
	//=
	static void parsePath(bool shouldParse);
	//=
	//	Enable / Disable cookie parsing (default <b>enabled</b>). 
	// Setting this to false disables the parsing of cookie data.
	// <p>
	// <b>Example:</b>
	// <p>
	// <code>
	// CGI::parseCookies( false );
	// </code>
	//<p>
	//=
	static void parseCookies(bool shouldParse);
	//=
	// Sets the maximum number of bytes that is acceptable for POST'ed data. The default value, 0, allows an unlimited length. 
	// If a non-zero positive number is used, then parsing of POST'ed data will not occur if the length is equal to or greater than the maximum allowable length.
	// If you don't expect large amounts of data to be sent to the application, setting a maximum POST thresh-hold will prevent D.O.S. attacks involving large 
	// data transmissions. To eliminate POSTed data altogether, use the value 1. Note that parsing of the query-string, PATH_INFO, and cookies will still occur
	// regardless of whether or not POST'ed data is ignored.
	// <p>
	// <b>Example:</b>
	// <p>
	// <code>
	// CGI::maxPostLength( 1048576 ); // 1 MB File Upload Limit
	// </code>
	//<p>
	// @param bytes A value from 0 to the maximum positive signed long value.
	//=
	static void maxPostLength(long bytes);


	//=
	// Turns case-sensitive name matching on/off (default on).
	// By <b>default</b>, matching the names of incoming data is <b>case-sensitive</b>. Setting this to false makes the matching
	// case-<i>insensitive</i>.
	// <p>
	// <b>Example:</b>
	// <p>
	// <pre><code>
	// CGI *parser = CGI::instance();
	// parser->setCaseSensitive(false);
	// const char *emailAddress1 = parser->value("EmAiL"); // will match "email"
	// parser->setCaseSensitive(true);
	// const char *emailAddress2 = parser->value("EmAiL"); // will NOT match "email"
	// </code></pre>
	//=
	void setCaseSensitive(bool isCaseSenstive);

	//=
	// Returns the total number of key=value pairs that were found. 
	// The number includes values obtained from PATH_INFO and HTTP_COOKIE in addition to the GET and POSTed data.
	//=
	int numValues() const;
	//=
	// Returns the number of values identified by <i>fieldname</i> that were sent. For example, if the following query string were sent:
	// <p>
	// <code><font color=6666cc><b>?color=blue&color=red&color=yellow</b></font></code>
	// <p>
	// Then <code><b>numValues("color")</b></code> would return 3. Note that the value associated with a fieldname <i>can</i> be empty (""). 
	// As such, the following query string would yield the same result as the one above:
	// <p>
	// <code><font color=6666cc><b>?color=&color=&color=yellow</b></font></code>
	// <p>
	// This method can also be used to determine if a field was sent at all (if the result is 0). 
	// However, the method {@link #exists exists()} is also available for this purpose.
	// <p>
	//=
	int numValues(const char *fieldname) const;
	//=
	// Returns the name of the field at the specified index within the data collection. 
	// If the fieldname is empty, the method returns "". 
	// If the index is out of bounds, the method returns NULL.
	// <p>
	//=
	const char *fieldnameAt(int position) const;
	
	//=
	// Check if a certain field exists.
	// Returns true if at least one data item matching the fieldname was sent (regardless of whether the value is empty or not), false otherwise.
	//=
	bool exists(const char *fieldname) const;
	//=
	// Determines if the data-pair identified at <i>index</i> within the collection is an uploaded file.
	// <p>
	// @return <b>true</b> if the data pair is an uploaded file, <b>false</b> otherwise
	// <p>
	//=
	bool isFile(int index) const;
	//=
	// Determines if the first data-pair identified by <i>fieldname</i> is an uploaded file.
	// <p>
	// @return <b>true</b> if the data pair is an uploaded file, <b>false</b> otherwise
	// <p>
	//=
	bool isFile(const char *fieldname) const;
	//=
	// Determines if the <i>nth</i> data-pair identified by <i>fieldname</i> is an uploaded file.
	// <p>
	// @return <b>true</b> if the data pair is an uploaded file, <b>false</b> otherwise
	// <p>
	//=
	bool isFile(const char *fieldname, int position) const;

	//=
	// Returns the source (form, path, cookie) of the data found at <i>index</i> within the collection.
	// Currently, the sources can either be "form", "cookie", or "path". 
	// If the source is "form", then the data pair was obtained from POST'ed data or the Query String. 
	// If the source is "cookie", the the data pair was extraced from the submitted cookies. 
	// If the source is "path", then the data was extracted from PATH_INFO.
	// <br>
	// Please note: <b>"form"</b> data includes file uploads (multipart/form-data) as well as XML, which are both sent as POST'ed data.
	//=
	const char *datasource(int index) const;

	//=
	// Returns the source (form, path, cookie) of the data identified by <i>fieldname</i>.
	// @see #datasource(int)
	//=
	const char *datasource(const char *fieldname) const;

	//=
	// Returns the source (form, path, cookie) of the <i>nth</i> data member identified by <i>fieldname</i>.  
	// @see #datasource(int)
	//=	
	const char *datasource(const char *fieldname, int position) const;

	//=
	// Shorthand for value(const char *fieldname);
	//=
	const char * operator [] (const char * fieldname);


	//=
	// Shorthand for fieldnameAt(x)
	//=
	const char * operator [] (int x);

	//=
	// Returns the value of the data-pair that exists at the specified index. 
	// If no value exists for the data-pair at that index, the method returns the empty string (""). 
	// If the index is out of bounds, the method returns NULL.
	// <p>
	// <blockquote>
	// <b>Example:</b> Print out all name-value pairs submitted<p>
	// <code>int total=parser->numValues();<br>
	// for(int x=0; x< total; x++)<br>
	//	{<br>
	//	cout << parser->fieldnameAt(x) << "=" << parser->value(x) << "\n";<br>
	// }<br>
  	// <p><br>
	//=
	const char *value(int index) const;

	//=
	// Returns the value of the data-pair identified by fieldname. 
	// If more than one data-pair exists with the given name, then this returns the value of the first data-pair, 
	// in essence, it is the same as calling {@link #value(constchar*,int) value(fieldname, 0)} (see below) . 
	// If no value exists for the fieldname, or the fieldname is not a valid identifier, the method returns "". 
	// This method will never return NULL.
	//= 
	const char *value(const char *fieldname) const;

	//=
	// Returns the <i><b>n</b>th</i> value of the data-pair identified by <i>fieldname</i>, where <b>n</b> is determined by <i>position</i>. 
	// If no value exists for the fieldname at the given position, the method returns "". <br>
	// If the fieldname is not a valid identifier, or the position is out of bounds, then the method returns NULL.
	//<p>
	//  
   // <b>Example: </b> print out selected categories (from a form with checkboxes named "cat")
	// <p>
	// <pre><code>
	// cout  << "Selected Categories :\n";
	// int x=0; 
	// const char *category;
	// while(category = parser->value("cat", x)) // will fail after last position 
	// {
	// 	cout << category << "\n";
	//		x++;
	//	}
	// </code></pre>
	// <p>
	//=
	const char *value(const char *fieldname, int position) const;

	// file related methods
	//
	//=
	// Returns the length of the data identified at <i>index</i> within the entire collection.
	// Most form data submitted is string data, this method is used primarily for uploaded files which may contain NULL bytes.
	//=
	int length(int index) const;

	//=
	// Returns the length of the data identified by <i>fieldname</i>.
	// @see #length(int)
	//=
	int length(const char *fieldname) const;

	//=
	// Returns the length of the <i>nth</i> data member identified by <i>fieldname</i>.  
	// @see #length(int)
	//=	
	int length(const char *fieldname, int position) const;
	//=
	// Returns the MIME content-type of the data identified at <i>index</i> within the collection. 
	// If the data is not an uploaded file, then the content-type defaults to application/x-formdata
	//=
	const char *contenttype(int index) const;
	//=
	// Returns the MIME content-type of the data identified by <i>fieldname</i>.
	// @see #contenttype(int)
	//=
	const char *contenttype(const char *fieldname) const;

	//=
	// Returns the MIME content-type of the <i>nth</i> data member identified by <i>fieldname</i>.  
	// @see #contenttype(int)
	//=	
	const char *contenttype(const char *fieldname, int position) const;
	//=
	// Returns a safe filename for the uploaded file found at <i>index</i> within the entire collection. It determines the name either by parsing the filepath, or parsing the submitted filename. 
	// Spaces are converted to underscores, and illegal file characters (based on windows systems) are removed. 
	// The filename is also truncated to an acceptable length.
	//=
	const char *filename(int index) const;
	//=
	// Returns a safe filename for the uploaded file identified by <i>fieldname</i>.
	// @see #filename(int)
	//=
	const char *filename(const char *fieldname) const;
	//=
	// Returns a safe filename for the <i>nth</i> uploaded file identified by <i>fieldname</i>.  
	// @see #filename(int)
	//=
	const char *filename(const char *fieldname, int position) const;
	//=
	// Returns the original filepath of the uploaded data.
	//	@return The original filepath if it exists. If the data is not an uploaded file, then this method returns NULL.
	//=
	const char *filepath(int index) const;

	//=
	// Returns the original filepath of the uploaded data.
	//	@return The original filepath if it exists. If the data is not an uploaded file, then this method returns NULL.
	//=
	const char *filepath(const char *fieldname) const;

	//=
	// Returns the original filepath of the uploaded data.
	//	@return The original filepath if it exists. If the data is not an uploaded file, then this method returns NULL.
	//=
	const char *filepath(const char *fieldname, int position) const;

	virtual ~CGI();


};


std::ostream& operator<<(std::ostream& os, const CGI& parser);

} // end namespace rude
#endif


