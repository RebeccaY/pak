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

#ifndef PAK_H
#define PAK_H

#include <iostream>
#include <fstream>
#include <array>
#include <cstdint>
#include <vector>
#include <cstring>
#include <set>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <map>
#include <dirent.h>
//#include <sys/statfs.h>

#ifndef CLI
#include <QDebug>
#endif


#include "directoryentry.h"
#include "treeitem.h"

#include "func.h"

#ifndef CLI
#include "qfunc.h"
#endif


enum Errors {
    NO_ERROR,
    NOT_PACK_FILE,
    INCORRECT_DIRECTORY_LENGTH,
    FILE_NOT_OPEN,
    PATH_TOO_LONG,
    DIRECTORY_NOT_OPEN,

};

class Pak
{
    friend DirectoryEntry;
    friend TreeItem;

public:
    Pak(const char *filename);
    Pak();
    ~Pak();

    int open(const char *filename, bool createIfNew = false);
    int close();
    int exportPak(const char *exportPath);
    int exportDirectory(const char *exportPath, TreeItem *rootItem = nullptr);
    int importDirectory(const char *importPath, TreeItem *rootItem = nullptr);
    void writeEntry(DirectoryEntry &entry);
    int writePak(const char *filename);
    int exportEntry( std::string& entryname, TreeItem* source );
    void reset(); // Clears the pak file.  Start new.  // Loses all changes
    TreeItem *addChild(stringList &dirList, TreeItem *entry);
    void deleteChild(TreeItem *entry, const int row);
    void deleteEntry(TreeItem *root, const int row);
    void updateIndex(DirectoryEntry &entry);
    TreeItem *rootEntry(void);
    void setVerbose(bool verbosity);
    std::fstream &getFileHandle(void);
    int addEntry(std::string path, const char*filename, TreeItem *rootItem);
#ifdef CLI
    void printChild(TreeItem *item);
#endif
private:
    size_t memused;
    bool verbose;
    std::string pakFile;
    std::array<char, 4> signature;
    int32_t directoryOffset;
    int32_t directoryLength;
    int32_t thisDirectoryEntryOffset;
    int numEntries;
// std::vector<DirectoryEntry> entries;
    TreeItem m_rootEntry;
    std::fstream file;
    int currentPakDataPosition;
    bool loadingDir; // This is used by importDir so that when it calls itself, it knows whether is in the the process
    // of recursion, or just starting.

    void resetPakDirectory();
    void makeDirectoryTree(TreeItem *item);

    void loadDir(DirectoryEntry entry);
    int writePakDir(TreeItem *item);
    void loadData(DirectoryEntry &entry);
};

#endif // PAK_H
