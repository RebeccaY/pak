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

#ifndef TREEITEM_H
#define TREEITEM_H
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <cassert>
#include "func.h"

#ifndef CLI
#include "qfunc.h"
#endif

#include "pakexception.h"

#include "directoryentry.h"
class Pak;

class TreeItem;

using TreeItemItr = std::vector<DirectoryEntry>::iterator;
using const_TreeItemItr = std::vector<DirectoryEntry>::const_iterator;

std::unique_ptr<TreeItem> createTreeItem( const std::string label, TreeItem* parent);

class TreeItem
{
public:

    explicit TreeItem ( const std::string o_label, TreeItem *o_parent = nullptr );
    ~TreeItem();
    void traverseForEachChild (void(Pak::*func)(TreeItem *), Pak *obj);
    void traverseForEachItem (void(Pak::*func)(DirectoryEntry &entry), Pak *obj);
    void appendChild ( std::unique_ptr<TreeItem> child );
    void deleteChildTree( const int pos);
    void deleteChildTree( std::vector<std::unique_ptr<TreeItem>>::iterator it);
    void appendItem ( DirectoryEntry &entry );
    TreeItem *child ( int row ); // Retreive child.
    std::string label() const; // Returns the name of the directory
    std::string pathLabel() const; // Returns name of the directory and full path.
    virtual int row() const;
    TreeItem *paren();
    int size() const; // Returns number of DirectoryEntry items.
    void clear();
    TreeItemItr begin();
    TreeItemItr end();
    TreeItemItr& operator*();
    int childCount() const;
    int columnCount() const;
    TreeItem *findChild ( std::string searchTerm, bool create = false ); // Returns the child that matches the directory.  Creates one if it does not exist if flag set                              
    DirectoryEntry &data ( unsigned int row );
    DirectoryEntry* findEntry ( const std::string searchTerm );
    TreeItem *parentItem();
    void deleteItem(const unsigned int row);
private:
    std::vector<std::unique_ptr<TreeItem>> childItems;
    std::vector<DirectoryEntry> items;
    TreeItem *parent;
    std::string directoryLabel;
    int childIndexOf(const TreeItem *ptr);

    TreeItemItr itr;
};


#endif // TREEITEM_H
