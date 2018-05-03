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

#include <iostream>

#ifdef   __linux
    #include <unistd.h>
    #include <linux/limits.h>
#elif __WIN32
    #include "getopt.h"
#elif __APPLE__
    #include <unistd.h>
#endif

#include <cassert>
#include "pakexception.h"
#include "exceptionhandler.h"

#include "pak.h"
#include "version.h"

static void printHeader(void)
{
       std::cout << "PAK: Build and exports PAK files for Quake, Quake 2 and related games.\n"
		<< "(C) Dennis Katsonis, Dekay Software (2015).\t\tVersion " << VERSION << "\n\n";
 
}
static void printLicense(void)
{
    std::cout << "This program is free software: you can redistribute it and/or modify\n"
              "it under the terms of the GNU General Public License as published by\n"
              "the Free Software Foundation, either version 3 of the License, or\n"
              "(at your option) any later version.\n\n"
              "This program is distributed in the hope that it will be useful,\n"
              "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
              "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
              "GNU General Public License for more details.\n"
              "You should have received a copy of the GNU General Public License\n"
              "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n";
}

static void print_help(void)
{
    std::cout << "Use : pak [options] -i/-o pakfile.pak -d directory/to/import/from/or/to\n\n"
              "Options :\n"
              " -i Import to this pak file.\t\t"
              " -e export from this pak file.\n"
              " -d Directory to import/export/delete or to extract to.\n"
              " -V Show license.\t\t\t"
	      " -p Internal pak path to use.\n"
	      " -D File to import/export/delete.\t"
              " -v Increase verbosity.\n"
              " -l List contents of PAK file.\t\t"
              " -x Delete from this PAK file.\n\n"
              "Pass the filename to the -i option to import files into\n"
              "a new pak file, or pass the filename to the -e option to export files from\n"
              "an existing pak file.  The -d option when importing selects where to\n"
	      "import files from.\n"
              "This option when exporting selects where to export the files to.\n\n"
              "Example: pak -i test.pak -d /windows/ogre -p sound\n"
              "Import the directory tree under windows/ogre into the file test.pak\n"
	      "under the 'sound' directory." << std::endl;
}



int main(int argc, char **argv)
{

    int optch;
    std::string pakfilename;
    std::string workingpath;
    std::string insertPath;
    //std::string importpath;
    bool importpak = false;
    bool exportpak = false;
    bool workWithFile = false;
    bool deleteStuff = false;
    bool verbose = false;
    bool pakPath = false;
    char *currentPath = nullptr;


    // auto memo = get_mem_total();
    
    if (argc <= 1) {
        printHeader();
        print_help();
        return 0;
    }

    while ((optch = getopt(argc, argv, "l:x:D:p:a:A:e:i:d:Vv")) != -1) {
        switch (optch) {
        case 'x': // Delete
            deleteStuff = true;
            pakfilename = optarg;
            break;
        case 'V': // Licence
            printLicense();
            return 0;
            break;
        case 'd': // Working directory
            workingpath = optarg;
            break;
        case 'D': // Working file
            workingpath = optarg;
            workWithFile = true;
            break;
        case 'e': // Export pak
            exportpak = true;
            pakfilename = optarg;
            break;
        case 'p': // subdirectory in pak to process
            insertPath = optarg;
            pakPath= true;
            break;
        case 'v': // Verbose
            verbose = true;
            break;
        case 'i': // import
            importpak = true;
            pakfilename = optarg;
            break;
        case 'l': // List
            pakfilename = optarg;
            try {
                Pak pak(pakfilename.c_str());
                pak.printChild(pak.rootEntry());
            } catch (PakException &e) {
                exceptionHander(e);
                return 1;
            }
           // pak.rootEntry()->traverseForEachChild(&Pak::printChild, &pak);
            break;
        }			// End switch.
    }				// End while.

    if ( deleteStuff && !workWithFile) {
        try {
            Pak pak(pakfilename.c_str());
            if (verbose) {
                pak.setVerbose(true);
            }
            pak.deleteChild(workingpath);
            pak.writePak(pakfilename.c_str());
        } catch (PakException &e) {
            exceptionHander(e);
            return 1;
        }
    }
    
        if ( deleteStuff && workWithFile) {
	  try {
            Pak pak(pakfilename.c_str());
            if (verbose) {
                pak.setVerbose(true);
            }
            pak.deleteEntry(workingpath);
            pak.writePak(pakfilename.c_str());
        } catch (PakException &e) {
            exceptionHander(e);
            return 1;
        }
    }
    
    if ( workWithFile && importpak ) {
        if ( !insertPath.empty() ) {
            auto insertPathPos = insertPath.end();
            --insertPathPos;
            if ( *insertPathPos != '/' ) {
                insertPath.append ( "/" );
            } // We want a trailing slash.  Add one if the user
            // did notput one.

            insertPathPos = insertPath.begin();
            if ( *insertPathPos == '/' ) {
                //We dont want a leading one.  Remove if it exists.
                insertPath.erase ( 0, 1 );
            }
        }

        try {
            Pak pak(pakfilename.c_str());
	    if (verbose) {
                pak.setVerbose(true);
            }

            TreeItem *tItem = pak.rootEntry()->findTreeItem(insertPath, true);
            if ( tItem == nullptr ) {
                tItem = pak.rootEntry();
            }
            pak.addEntry(insertPath,workingpath.c_str(), tItem);
            pak.writePak(pakfilename.c_str());
        } catch (PakException &e) {
            exceptionHander(e);
            return 1;
        }
        return 0;
    }

    if (workWithFile && exportpak) {
      auto workingPathPos = workingpath.begin();
      if (*workingPathPos == '/') {
	workingpath.erase(0, 1);
      }
        try {
            Pak pak(pakfilename.c_str());
            TreeItem *tItem = pak.rootEntry()->findTreeItem(workingpath, false);
            pak.exportEntry(workingpath, tItem);
        } catch (PakException &e) {
            exceptionHander(e);
            return 1;

        }
        return 0;
    }

    if (pakPath && importpak) {
        insertPath.append("/");
        try {
            Pak pak(pakfilename.c_str());
            TreeItem *tItem = pak.rootEntry()->findTreeItem(insertPath, true);
            pak.importDirectory(workingpath.c_str(), tItem);
            pak.writePak(pakfilename.c_str());
        } catch (PakException &e) {
            exceptionHander(e);
            return 1;
        }
        return 0;
    }

    if (pakPath && exportpak) {
        insertPath.append("/");
        try {
            Pak pak(pakfilename.c_str());
	    if (verbose) {
                pak.setVerbose(true);
            }

            TreeItem *tItem = pak.rootEntry()->findTreeItem(insertPath, false);
            pak.exportDirectory(workingpath.c_str(), tItem);
        } catch (PakException &e) {
            exceptionHander(e);
            return 1;
        }
        return 0;
    }


    if (importpak && exportpak) {
        std::cout << "Cannot import and export at the same time...\n";
        return 1;
    }

    if (pakfilename.empty()) {
        std::cout << "No filename specified...\n\n";
        print_help();
        return 1;
    }

    currentPath = getcwd(NULL, 0);


    if (importpak && !pakPath) {
        try {
            Pak pak(pakfilename.c_str());
            if (verbose) {
                pak.setVerbose(true);
            }
            if (workingpath.empty()) {
                workingpath = ".";
            }

            pak.importDirectory(workingpath.c_str(), nullptr);
            chdir(currentPath);
            pak.writePak(pakfilename.c_str());
            pak.close();
        } catch (PakException &e) {
            exceptionHander(e);
        }
        // pak.test();
    }

    if (exportpak && !pakPath) {
        std::cout << "Exporting " << pakfilename << std::endl;

        if (workingpath.empty()) {
            workingpath = ".";
        }
        try {
            Pak pak(pakfilename.c_str());
            if (verbose) {
                pak.setVerbose(true);
            }
            pak.exportPak(workingpath.c_str());

        } catch (PakException &e) {
            exceptionHander(e);

        }
    }
    free(currentPath);
    return 0;
}


