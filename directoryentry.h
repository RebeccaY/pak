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

#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H

#include <array>
#include <cstdint>
#include <memory>
#include <fstream>
#include <list>
#include "func.h"

#ifndef CLI
#include "qfunc.h"
#endif

#include <unistd.h>
#include <dirent.h>
#include "pakexception.h"

class DirectoryEntry
{
public:
    DirectoryEntry();
    DirectoryEntry ( DirectoryEntry &&other );
    DirectoryEntry &operator=(DirectoryEntry &&other);
    DirectoryEntry ( DirectoryEntry &other ) = delete;
    ~DirectoryEntry();
    const char *data();
    void clear();
    std::array<char, 56> filename;

    int loadData ( std::fstream &fin ); // stream should be already open
    int loadData( const char *filename); // load data from file.
    int saveData ( std::fstream &fout ); // stream should be already open
    void exportFile( const char *path, std::fstream &fin );
    int getLength() const;
    void setLength(const int32_t &value);
    int32_t getPosition() const;
    void setPosition(const int32_t &position);
    bool isLoaded() const;
private:
    bool m_loaded;
    int32_t m_position;
    std::unique_ptr<char[]> entryData;
    int32_t m_length;
    bool m_fileLinked; // Whether the data is linked to a file, or an open pak
    // If linked to a file, the file must remain
};



#endif // DIRECTORYENTRY_H

