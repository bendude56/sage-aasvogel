# Project Sage Aasvogel

## What is it?

Project Sage Aasvogel is a basic operating system written using Assembly language and C. This was mainly written for educational purposes, so don't expect it to be of much practical use.

## Disclaimer

This is an experimental OS designed solely for educational purposes. I am not in **any way responsible** for anything that happens to any hardware you decide to run this code on! This code is **not thoroughly tested** and will most likely cause your processor to melt and your disk drive to explode.

## Building Sage Aasvogel

Before building Sage Aasvogel, you first need to have built a proper cross compiler for the `i686-elf` platform. The tested development environment uses GCC 6.1.0 and GNU Binutils 2.26. The `i686-elf-gcc` and `i686-elf-ld` commands must also be in your `PATH` environment variable.

Once you have all the required programs installed, you can build the entire Sage Aasvogel project by running `make all`. After this, you should run `sudo make image` to actually package the floppy image in order to boot Sage Aasvogel. The resulting image will be stored in `sa.img` in the project directory.

Note that the creation of the floppy disk image using `make image` requires root permissions in order to be able to mount the disk image as a loopback device.

## Testing Sage Aasvogel

The preferred method for testing Sage Aasvogel is by using a system emulator such as [qemu](http://www.qemu.org/) with an attached debugger. Running `make emulate` will start qemu with 64MiB of RAM and the floppy disk image `sa.img` mounted on the first floppy drive. Once started, qemu will listen on port `1234` for a debugger to connect.

## License

Sage Aasvogel is released as open-source software under the terms of the GNU General Public License version 3. You can find a full copy of the license in markdown format in `LICENSE.md` or the official version on the [GNU Website](https://www.gnu.org/licenses/gpl.html).
