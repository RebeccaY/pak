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

#include "directoryentry.h"

DirectoryEntry::DirectoryEntry() :
  m_loaded(false),
  m_position(0),
  entryData(nullptr),
  m_length(0)
{

}

DirectoryEntry::DirectoryEntry(DirectoryEntry && other)
{
  filename = std::move(other.filename);
  m_position = other.m_position;
  m_length = other.m_length;
  m_loaded = other.m_loaded;
  if (other.m_loaded) {
      entryData = std::move(other.entryData);
      other.entryData.reset(nullptr);
    }

}

DirectoryEntry &DirectoryEntry::operator=(DirectoryEntry &&other)
{
  if (this != &other) {
      entryData.reset(nullptr);
      filename = std::move(other.filename);
      m_position = other.m_position;
      m_length = other.m_length;
      m_loaded = other.m_loaded;
      entryData = std::move(other.entryData);
      if (other.m_loaded) {
          other.entryData.reset(nullptr);
        }
    }
  return *this;
}


DirectoryEntry::~DirectoryEntry()
{

}

int DirectoryEntry::loadData(const char *filename)
{

  std::ifstream fin;
  fin.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
  try {
    fin.open(filename, std::ios::in | std::ios::binary);
    entryData.reset(new char[m_length]);
    fin.read(entryData.get(), m_length);
    m_loaded = true;
    fin.close();
  } catch ( std::istream::failure &e ) {
    throw (PakException("Error loading data", e.what()));
  } catch (std::bad_alloc &e) {
    throw (PakException("Out of memory", e.what()));
  }
  return 0;
}

int DirectoryEntry::loadData(std::fstream &fin)
{

  if (fin.is_open() != true) {
      return -1;
    }
  auto oldpos = fin.tellg(); // preserve the current position
  if (!m_loaded) {
      try {
        entryData.reset(new char[m_length]);

        fin.seekg(m_position, std::ios::beg);
        fin.read(entryData.get(), m_length);
      } catch ( std::istream::failure &e ) {
        fin.seekg(oldpos, std::ios::beg);
        throw (PakException("Error loading data", e.what()));
      } catch (std::bad_alloc &e) {
        throw (PakException("Out of memory", e.what()));
      }
      fin.seekg(oldpos, std::ios::beg);    // restore file position for the caller.
      m_loaded = true;
    }
  return 0;
}

int DirectoryEntry::saveData(std::fstream &fout)
{

  return 0;
}

void DirectoryEntry::exportFile(const char *path, std::fstream &fin)
{
  // We need to load the data here....
  if (!m_loaded) {
      loadData(fin);
    }

  chdir(path);
  std::ofstream fout;
  fout.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
  try {
#ifndef CLI
    fout.open(absoluteFileName(filename).toStdString().c_str(), std::ios::binary | std::ios_base::out);
#else 
    fout.open(absoluteFileName(filename).c_str(), std::ios::binary | std::ios_base::out);
#endif
    fout.write(entryData.get() , m_length);
    fout.close();
  }  catch ( std::ifstream::failure &e ) {
    throw (PakException("Error writing file", "path"));
  }

  clear();

}

const char *DirectoryEntry::data()
{

  return this->entryData.get();
}

void DirectoryEntry::clear()
{
  m_loaded = false;
  entryData.reset(nullptr);
}
int32_t DirectoryEntry::getPosition() const
{
  return m_position;
}
void DirectoryEntry::setPosition(const int32_t &position)
{
  m_position = position;
}

bool DirectoryEntry::isLoaded() const
{
  return m_loaded;
}


int DirectoryEntry::getLength() const
{

  return static_cast<int>(m_length);
}

void DirectoryEntry::setLength(const int32_t &value)
{
  m_length = value;
}


