/*
 * Utility to manipulate Quake PAK data files.
 * Copyright (C) 2015  Dennis Katsonis <dennisk (at) netspace dot net dot au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef FUNC_H
#define FUNC_H


//#include <sys/sysinfo.h>
//#include <string>
#include <algorithm>
#include <array>
#include <string>
#include <fstream>
#include <string>
#include <vector>

using stringList = std::vector<std::string>;

enum class fileTypes {
  Map,
  Texture,
  Graphic,
  Sound,
  Demo,
  Model,
  Skin,
  Data,
  Config,
  Other
};


const int PAK_HEADER_SIZE = 12;
const int PAK_DATA_LABEL_SIZE = 56;
const int DIRECTORY_ENTRY_SIZE = 64;
using pakDataLabel = std::array< char, int(PAK_DATA_LABEL_SIZE) >;

bool fexists(std::string filename);
#ifdef CLI
std::string absoluteFileName(pakDataLabel fname);
#endif
std::string arrayToString(pakDataLabel &filename);

void stringToArray(std::string s, pakDataLabel &arrai);
void clearArrayAfterNull(pakDataLabel &array);

#ifdef CLI
std::string getFileName(const std::string &filename);
#endif

template <typename T>
stringList tokenize(T &text)
{
    stringList directoryList;
    auto pos = text.begin();
    //auto dirName = &entry.filename;

    while (pos != text.end()) {
        auto fit = std::find(pos, text.end(), '/');
        if (fit != text.end()) {
            auto x = fit - pos;
            std::string s;
            s.resize(x);
            std::copy(pos, fit, s.begin());

            if (s == "..") {
                s = "dotdot";
            } // This is a workaround for the Quake 2 pak0.pak file.
            // It contains as a path '..' for the ctank, which screws things up for this program
            // when trying to create this directories.
            // We'll convert this back to '..' when importing.

	    if (s.size() > 0 ) {
	      directoryList.push_back(s);
	    }
            pos = fit;
        }
        pos++;
    }
    return directoryList;
}




unsigned long get_mem_total();


#endif // FUNC_H

