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

#include <cassert>

#include "func.h"


bool fexists(std::string filename)
{
  bool x;
  std::ifstream ifile(filename.c_str());
  x = ifile.is_open();
  ifile.close();
  return x;
}

unsigned long get_mem_total() {

  //  struct sysinfo info;
  // sysinfo(&info);
  //return info.totalram;
  return 0ul;
}

#ifdef CLI
std::string getFileName(const std::string &filename)
{
    std::string finalname;
    finalname.reserve(PAK_DATA_LABEL_SIZE);

#ifdef __linux
    const auto it = std::find(filename.rbegin(), filename.rend(), '/');
#else
    const auto it = std::find(filename.rbegin(), filename.rend(), '\\');
#endif
    
    const auto fit = it.base();
    const auto x = filename.end() - fit;
    finalname.resize(x);
    std::copy(fit, filename.end(), finalname.begin());
    return finalname;  
}

#endif

#ifdef CLI
std::string absoluteFileName(pakDataLabel fname)
{
    std::string filename;
    filename.reserve(PAK_DATA_LABEL_SIZE);

#ifdef __linux
    const auto it = std::find(fname.rbegin(), fname.rend(), '/');
#else
    const auto it = std::find(fname.rbegin(), fname.rend(), '\\');
#endif

    const auto fit = it.base();
    const auto x = fname.end() - fit;
    filename.resize(x);
    std::copy(fit, fname.end(), filename.begin());
    return filename;

}

#endif

std::string arrayToString(pakDataLabel &filename)
{
    std::string filename_stringified(PAK_DATA_LABEL_SIZE, '\0');
    std::copy(filename.begin(), filename.end(), filename_stringified.begin());
    assert(filename_stringified.size() <= PAK_DATA_LABEL_SIZE);
    return filename_stringified;
}



void stringToArray(std::string s, pakDataLabel &arrai)
{
    assert(s.size() <= PAK_DATA_LABEL_SIZE);
    const auto endpos = std::copy(s.begin(), s.end(), arrai.begin());
    for (auto pos = endpos; pos != arrai.end(); ++pos) {
        *pos = '\0';
    }

}

void clearArrayAfterNull ( pakDataLabel& array ) {
    auto pos = std::find ( array.begin(), array.end(), char ( 0 ) );
    if ( pos == array.end() ) {
        return;
    } else {
        while ( pos != array.end() ) {
            *pos = char ( 0 );
            ++pos;
        }
    }
    return;
}


