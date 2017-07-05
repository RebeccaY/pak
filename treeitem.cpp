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

#include "treeitem.h"

std::unique_ptr< TreeItem > createTreeItem(const std::string label, TreeItem *parent)
{

  std::unique_ptr<TreeItem> x(new TreeItem(label, parent));
  return x;
}


void TreeItem::deleteChildTree(const int pos)
{
  assert(pos < childItems.size());
  childItems.erase(childItems.begin()+pos);
}

void TreeItem::deleteChildTree(std::vector< std::unique_ptr< TreeItem > >::iterator it)
{
  childItems.erase(it);
}


void TreeItem::traverseForEachChild(void(Pak::*func)(TreeItem * ), Pak *obj)
{
  
  for (auto &it : childItems)
    {
      it->traverseForEachChild(func, obj);
    }

  for (auto &it : childItems)
    {
      (*obj.*func)(it.get());
    }

}

void TreeItem::traverseForEachItem(void(Pak::*func)(DirectoryEntry &item), Pak *obj)
{
  for (auto &it : childItems)
    {
      it->traverseForEachItem(func, obj);
    }

  for (auto &item : items)
    {
      (*obj.*func)(item);
    }

}


TreeItem::TreeItem(const std::string o_label, TreeItem *o_parent)
{
  parent = o_parent;
  directoryLabel = o_label;

}

TreeItem::~TreeItem()
{
  clear();
}

void TreeItem::appendChild(std::unique_ptr<TreeItem> child)
{
  childItems.push_back(std::move(child));

}

TreeItem *TreeItem::child(int row)
{
  return childItems.at(row).get();
}


int TreeItem::childIndexOf(const TreeItem *ptr)
{
  int x{};
  for (const auto &i : childItems)
    {
      if (i.get() == ptr) {
          return x;

        }
      ++x;
    }
  return 0;
}



int TreeItem::row() const
{
  if (parent) {
      return parent->childIndexOf(const_cast<TreeItem*>(this));
    }

  return 0;
}

TreeItem *TreeItem::paren()
{
  return parent;
}

int TreeItem::childCount() const
{
  return childItems.size() ;
}

int TreeItem::columnCount() const
{
  return 1;
}


DirectoryEntry &TreeItem::data(unsigned int row)
{

  if (row >= items.size()) {
      throw PakExceptionIndexOutOfRange(row, items.size());
    }
  return items.at(row);
}


TreeItem *TreeItem::parentItem()
{
  return parent;
}

TreeItem *TreeItem::findChild(std::string searchTerm, bool create)
{
  if (childItems.empty() && create) {
      std::unique_ptr<TreeItem> x = createTreeItem(searchTerm, this);
      appendChild(std::move(x));
      return childItems.back().get();
    }
  for (const auto & i : childItems) {
      if (i->label() == searchTerm) {
          return i.get();
        }
    }

  if (create == false) {
      return nullptr;
    } else {
      std::unique_ptr<TreeItem> x(new TreeItem(searchTerm, this));
      appendChild(std::move(x));
      return childItems.back().get();
    }
  return nullptr;
}

std::string TreeItem::label() const
{
  return directoryLabel;
}

std::string TreeItem::pathLabel() const
{
  std::string fullpath;

  if (parent != nullptr) {
      fullpath = parent->pathLabel();
    }

  if (parent != nullptr) {
      fullpath += directoryLabel;
      fullpath += "/";
    } else {
     // fullpath +="/"; // Add leading slash only for root entry;
    }

  return fullpath;
}

DirectoryEntry* TreeItem::findEntry ( const std::string searchTerm )
{
  for ( auto &x : items )
    {
      if ( std::equal ( searchTerm.begin(), searchTerm.end(), x.filename.begin() ) )
        {
	  return &x;
        }
    }
    return nullptr;
}

void TreeItem::appendItem(DirectoryEntry &entry)
{
  // Check to see if there is an existing entry
  for (auto &x : items) {
      if(std::equal(entry.filename.begin(), entry.filename.end(), x.filename.begin())) {
          // We have an existing one.  Error!
          std::string message{"Found duplicate entry : "};
          message += arrayToString(entry.filename);
          throw (PakException("Duplicate entry", message.c_str()));
       }
    }
  items.push_back(std::move(entry));
}

void TreeItem::deleteItem(const unsigned int row)
{
  if (row >= items.size()) {
      throw PakExceptionIndexOutOfRange(row, items.size());
    }
  auto it = items.begin();
  std::advance(it, row);
  items.erase(it);

}

int TreeItem::size() const
{
  return items.size();
}

void TreeItem::clear()
{
  items.clear();
  childItems.clear();
}



TreeItemItr TreeItem::begin()
{
  return items.begin();
}

TreeItemItr TreeItem::end()
{
  return items.end();
}

TreeItemItr &TreeItem::operator*()
{
  return itr;
}


