# MuesumGuideRobot

Software for the museum guide robot designed by Glasgow University Team Design Project 4 - Group 8


## Building the robot software

Building directly on a Raspberry Pi, `make && sudo make install` should suffice to build and install the software.

Cross-compiling will require, the `make` variables `TOOLCHAIN` and `REMOTE_INSTALL` to be set, for example:
```
make TOOLCHAIN=arm-linux-gnueabihf-
make install REMOTE_INSTALL=root@raspberrypi.local
```
The `install` target will require root access on target device -- for a local build this is easily handled by `sudo`, but for a remote build the ssh user must have root access. If `bindir` is used to modify install location, the copy may take place without root privileges.

N.B. I haven't actually tried a full build so the makefile might need tweaks, but it should in theory work as written.

## Adding new source files

If new source files are added or existing ones renamed, the `SRCS` variable in the makefile should be edited accordingly. C source files (`.c` extension) and C++ source files (`.cpp` extension) are accepted here -- other extensions will require further edits to the makefile. C/C++ header files (`.h` extension) should not be included in the `SRCS` variable, and, in fact, do not need to be mentioned in the makefile at all.

## Adding new libraries

Similarly, any libraries required for linking must be included in the `LIBS` variable in the makefile. Any time a new library is referenced in the source code, the makefile should be edited accordingly.

### Installing OpenCV

Find the instructions on https://solarianprogrammer.com/2019/09/17/install-opencv-raspberry-pi-raspbian-cpp-python-development/


