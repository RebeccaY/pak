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

#ifndef PAKEXCEPTION_H
#define PAKEXCEPTION_H

#include <stdexcept>

class PakException : public std::exception
{
public:
  PakException() =default;
  PakException(const char *what, const char *where);
  virtual ~PakException() = default;
  virtual const char *what() const noexcept override {return m_message.c_str(); }
  virtual const char *where() const noexcept {return m_where.c_str(); }
private:
  std::string m_message;
  std::string m_where;
};

class PakExceptionIndexOutOfRange : public PakException
{
public:
  PakExceptionIndexOutOfRange() = default;
  PakExceptionIndexOutOfRange(int index, int indexSize);
  virtual ~PakExceptionIndexOutOfRange() = default;
  virtual const char *what() const noexcept override { return "Index out of range"; }
  virtual const char *where() const noexcept {return m_message.c_str(); }
private:
  int m_index;
  int m_indexSize;
  std::string m_message;
};

#endif // PAKEXCEPTION_H
