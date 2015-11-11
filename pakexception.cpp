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

#include "pakexception.h"

PakException::PakException(const char *what, const char *where)
{
  m_message = what;
  m_where = where;
}



PakExceptionIndexOutOfRange::PakExceptionIndexOutOfRange(int index, int indexSize)
{
  m_index = index;
  m_indexSize = indexSize;

  m_message = "Index out of range for deletion.\n";
  m_message += "Tried to access ";
  m_message += m_index;
  m_message += " > tree size of ";
  m_message += m_indexSize;
}

