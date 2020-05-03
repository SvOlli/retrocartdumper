
 Retro Cartridge Dumper
========================

This is a small project for dumping Atari 2600 cartridges. The hard- and
software is created in a way that it should be easy to adapt it for other
platforms.

The commandline tool is run like this:
```
<dumpername> <serialdevice> <carttype> <filename>
```
The parameters are:
- `<dumpername>`: right now, only one exists: dump2600
- `<serialdevice>`: the device the dumper hardware is running on
- `<carttype>`: the name of the dumping subroutine to use
                (run without parameters for a list)
- `<filename>`: the filename of the created dump

The code for the dumping hardware is in the "boards" folder, both source
and hex files for easy flashing. The source code was done using the Arduino
environment. Sorry.


Discussion happens on [AtariAge](https://atariage.com/forums/topic/305662-little-home-project-rom-dumper/).
