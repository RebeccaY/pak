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
#define VERSION "0.1.0"
#include <iostream>
#include <unistd.h>
#include <linux/limits.h>
#include <cassert>
#include "pakexception.h"
#include "exceptionhandler.h"

#include "pak.h"

void printLicense(void)
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

void print_help(void)
{
    std::cout << "Use : pak [options] -i/-o pakfile.pak -d directory/to/import/from/or/to\n\n"
              "Options :\n"
              " -i Import directory tree to pak.\t"
              " -e export pak to directory tree.\n"
              " -d Directory to import from/export to.\n"
              " -V Show license.\n"
              " -v Increase verbosity.\n"
              "\nPak will either import a directory tree into a pak file, or export a pak\n"
              "files contents into a directory tree.\n"
              "Using it easy.  Just pass the filename to the -i option to import files into a new pak\n"
              "file, or pass the filename to the -e option to export files from an existing pak\n"
              "file.  The -d option when importing selects where to import files from.\n"
              "This option when exporting selects where to export the files to.\n\n"
              "Example: pak -i test.pak -d /windows/mymod\n"
              "Import the directory tree under mymod into the file test.pak\n";
}


int main(int argc, char **argv)
{

    int optch;
    std::string pakfilename;
    std::string workingpath;
    //std::string importpath;
    bool importpak = false;
    bool exportpak = false;
    bool verbose = false;
    char *currentPath;


   // auto memo = get_mem_total();

    std::cout << "PAK: Build and exports PAK files for Quake, Quake 2 and Quake/Quake2\nengine based games.\n"
              << "(C) Dennis Katsonis (2015).\t\tVersion " << VERSION << "\n\n";

    if (argc <= 1) {
        print_help();
        return 0;
    }

    while ((optch = getopt(argc, argv, "e:i:d:Vv")) != -1) {
        switch (optch) {
        case 'V':
            printLicense();
            return 0;
            break;
        case 'd':
            workingpath = optarg;
            break;
        case 'e':
            exportpak = true;
            pakfilename = optarg;
            break;
        case 'v':
            verbose = true;
            break;
        case 'i':
            importpak = true;
            pakfilename = optarg;
            break;
        }			// End switch.
    }				// End while.

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
    
    
    if (importpak) {
        std::cout << "Creating " << pakfilename << std::endl;
        Pak pak;
        if (verbose) {
            pak.setVerbose(true);
        }
        if (workingpath.empty()) {
            workingpath = ".";
        }
        try {
            pak.importDirectory(workingpath.c_str(), nullptr);
	    //chdir(currentPath);
	    pak.writePak(pakfilename.c_str());
        } catch (PakException &e) {
            exceptionHander(e);
            pak.close();
        }
        // pak.test();
    }

    if (exportpak) {
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

    return 0;
}


