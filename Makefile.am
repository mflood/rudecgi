#
# Copyright (C) 2005 Matthew Flood
# See file AUTHORS for contact information
#
# This file is part of RudeCGI.
#
# RudeCGI is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# RudeCGI is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with RudeCGI; (see COPYING) if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#------------------------------------------------------------------------
lib_LTLIBRARIES = librudecgi.la

librudecgi_la_SOURCES = 	src/AbstractImplementation.cpp \
	src/AbstractImplementation.h \
	src/Implementation.cpp \
	src/Implementation.h \
	src/PathMap.cpp \
	src/PathMap.h \
	src/cgi.cpp \
	src/cgi.h \
	src/ClientData.cpp \
	src/ClientData.h \
	src/DataRepos.cpp \
	src/DataRepos.h \
	src/Interactive.cpp \
	src/Interactive.h \
	src/CookieData.cpp \
	src/CookieData.h \
	src/DataObject.cpp \
	src/DataObject.h \
	src/FileData.cpp \
	src/FileData.h \
	src/PathData.cpp \
	src/PathData.h \
	src/SimpleData.cpp \
	src/SimpleData.h \
	src/CaseMatch.cpp \
	src/CaseMatch.h \
	src/CaselessMatch.cpp \
	src/CaselessMatch.h \
	src/WordMatch.cpp \
	src/WordMatch.h \
	src/AbstractFactory.cpp \
	src/AbstractFactory.h \
	src/CommandLineExtractor.cpp \
	src/CommandLineExtractor.h \
	src/Factory.cpp \
	src/Factory.h \
	src/FormdataExtractor.cpp \
	src/FormdataExtractor.h \
	src/GETExtractor.cpp \
	src/GETExtractor.h \
	src/POSTExtractor.cpp \
	src/POSTExtractor.h \
	src/TestExtractor.h \
	src/UnknownExtractor.cpp \
	src/UnknownExtractor.h \
	src/AbstractParser.cpp \
	src/AbstractParser.h \
	src/AbstractParserFactory.cpp \
	src/AbstractParserFactory.h \
	src/CookieParser.cpp \
	src/CookieParser.h \
	src/NewMultipartParser.cpp \
	src/NewMultipartParser.h \
	src/ParserFactory.cpp \
	src/ParserFactory.h \
	src/PathMapParser.cpp \
	src/PathMapParser.h \
	src/PathParser.cpp \
	src/PathParser.h \
	src/SimpleXMLParser.cpp \
	src/SimpleXMLParser.h \
	src/URLEncodeParser.cpp \
	src/URLEncodeParser.h \
	src/UnknownParser.cpp \
	src/UnknownParser.h

librudecgi_la_LDFLAGS = -version-info 5:0:0

pkginclude_HEADERS = src/cgi.h
pkgincludedir = $(includedir)/rude

man_MANS = man3/rudecgi.3

EXTRA_DIST = $(man_MANS)

AUTOMAKE_OPTIONS = gnits dist-bzip2 dist-shar dist-zip


