
# Xroofs

Xroofs is a X-Plane library which makes some of the tall buildings in SimHeavens X-World (and the older X-Europe/America/Asia/Australia packages) interesting for heli pilots again.

The original LR scenery has many "towers" with heli-pads equipped. So sim-heli-pilots can exercise to handle the challenge on landing on a tall building.

However, when using the great SimHeaven X-World library, for performance reasons the tower objects being used are simplified facades without heli pads. Means, you cannot land of them anymore.

So the idea was to provide a modified version of the facades objects which have a "hard roof".

Instead of modifying the original scenery files, a library can be used to replace the objects by providing a modified copy of them. This library can be enabled and disabled any time if needed and the original SimHeaven files are not touched.

The scenery/library folder gets created when running the "xroofs.exe" ( or MacOS/Linux equivalent ). The program will look for an available X-World package and reads the facade files and stores a modified version of them in the Xroofs scenery folder. Inside this folder it also creates the library.txt file and adds 3 symbolic links.

The library can be uninstalled by simply deleting the "Custom Scenery/Xroofs" folder. Disable er enable of the library can be done in the scenry_packs.ini file. That's useful for testing the performance with/without using the library. I could not notice a significant performance degradation, but it might depend on the location you're flying at.


# Installation

Download the ZIP file and copy the correct binary (xroofs.exe for Windows, xroofs for macOS) to your X-Plane base directory and execute it. The program creates a folder "Xroofs" in your "Custom Scenery" folder. It also creates a "library.txt" file, 3  sym links and the modified copy of the FAC files inside the "Custom Scenery/Xroofs" folder.



# Uninstall

Simply delete the "Xroofs" folder in your "Custom Scenery" folder and restart X-Plane. No other file has been changed. 


# scenery_packs.ini

Make sure the Xroofs line stays ABOVE the Simheaven packages in your scenery_packs.ini.


# Build

Windows:

    requires "dirent.h"  ( i.e. from https://github.com/tronkko/dirent/blob/master/include/dirent.h )

	cl xroofs.c /D "NODEBUG" /O2


macOS or Linux:

	make xroofs


# Versions

	0.1.3  added more buildings, changed LOD to 9000 , code cleanup
	0.1.2  internal
	0.1.1  minor fixes 
	0.1.0  inital version
