PAK
===

Pak allows you to create, edit and extract files from PAK files for
Quake and Quake engine based games.

About this program
------------------
PAK files are little more than an archive, similar to ZIP or TAR files.
They have internal directories, subdirectories and files.  Maps for
Quake/Quake 2 for example, are .BSP files stored under a 'maps' directory.

Pak supports the following command line parameters

-i filename.pak
  Import to this filename.  If the pak file does not exist, it will be
  created, otherwise it is appended to.

-o filename.pak
 Export to this filename.  When importing a file or directory tree,
 this file will be created if it does not already exist.

-d
 Directory to import from, export do.  When importing to a pak, this is
 the directory containing what will be imported.  Everything under the
 directory will be added, but not the directory itself.  These will form
 the 'root' directory of the pak file.

When exporting, the files/directories will be placed within this
directory.

-D
 Import/export file.  Like the -d option, but works with files.  You can
 either add a file to the pak file, or extract a file.  When extracting,
 the full path must be specified.

-p
 Internal pak path to use.  An existing pak file can contain directories
 within it, such as 'sound/ogre'.  This option allows you to use
 a subdirectory within a pak file as the target for operations.
 For importing files or directories, the -p option exports only
 subdirectories, or imports to that directory.  This option allows you
 to specify where the file or directory tree will go.

-v
 Verbose.  Print more information.

-V
 Display licence.


Examples
--------

	pak -i test.pak -d /storage/ulysses

This creates a new file called test.pak, which will contain the contents of the directory
/storage/ulysses

	pak -o test.pak -d /temp

This extracts the contents of test.pak to the /temp directory

	pak -i test.pak -p /sound/ogre -d /storage/ogre

This imports the files/directories under storage/ogre into the pak file, placing them
within the pak under /sound/ogre

	pak -i test.pak -p /sound/ogre -D pain.wav

The same as above, but inserts pain.wav in the pak under /sound/ogre

	pak -e file.pak -p sound -d target

Exports the 'sound' directory in file.pak to directory 'target'.

	pak -e file,pak -D sound/misc/basekey.wav

Exports the file sound/misc/basekey.wav


Notes
-----

* The full path cannot exceed 56 characters.  If you are importing
files and directories, the total length of the directory names and
file name cannot exceed 56 characters due to limitation of the PAK file
format. Pakqit will stop importing if this limit is exceeded.

* Pakqit does not allow two files within one directory to have the
same name.
