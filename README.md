# Operating System PCF

> Miguel Abreu
> Copyright 2007-2012

## Development Environment
OS:
* Windows 7 SP1 32 bits.

SDK:
* DJGPP Version 2.03;
* Microsoft Visual Basic Run Time v.5;
* `mcopy` and `mmd`;
* Custom `build` app (not in this repo, comment line 34 of `SYSTEM_PATH\Makefile` to compile);
* Cygwin (with package libintl3).

Testing:
* Bochs 2.3.7.

Others:
* Doxygen 1.5.6.

## Quick Start
* Set `ROOT_PATH` var;
* Set `TOOLS_PATH` var (e.g.: `TOOLS_PATH=%ROOT_PATH%\tools\bin`);
* Set `SOURCE_PATH` var (e.g.: `SOURCE_PATH=%ROOT_PATH%\OSPCF-Source`);
* Set `RELEASE_PATH` var (e.g.: `RELEASE_PATH=%ROOT_PATH%\release`);
* Set `SYSTEM_PATH` var (e.g.: `SYSTEM_PATH=%SOURCE_PATH%\system`);
* Set `DRIVERS_PATH` var (e.g.: `DRIVERS_PATH=%SYSTEM_PATH%\drivers`);
* Set `SYSTEM_OUT` var (e.g.: `SYSTEM_OUT=%SYSTEM_PATH%\bin`);
* Set `LIB_PATH` var (e.g.: `LIB_PATH=%SOURCE_PATH%\lib`);
* Set `APP_PATH` var (e.g.: `APP_PATH=%SOURCE_PATH%\app`);
* Set `INCLUDE_PATH` var (e.g.: `INCLUDE_PATH=%SOURCE_PATH%\include`).

## To Compile
After setting correct PATHS go to `SOURCE_PATH` and type `make all`. Kernel binary will be in `SYSTEM_OUT`.

## To Run
GRUB is needed to correctly boot the system (version 0.95 is the only one i used for testing). To test in bochs make a floppy image (or .iso image) using `mcopy` and `mmd`. Kernel binary path must be in `\kernel` path and applications must be in `\bin` path. 32MB of RAM is enough to run the system. Kernel prints debug information through COM1 (only if parameter `debug0=true` is passed by GRUB in kernel loading).
