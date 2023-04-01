# Glitch
Open Sourced project to glitch files and images in a number of unique ways.

# Introduction
This project began as a file fuzzer for the purpose of vulnerability research (passing applications corrupted files to attempt to provoke unintended behaviors).  However, it was frequently used in art projects such as [D3AD DANKS](https://objkt.com/collection/KT1LtooAMMH8Ro71pyvEVTo8XnW14y5PtpiA), [Trash Art](https://objkt.com/collection/KT1RQEwVBmz3AZXr2U4LJrZWhUEFzKL23C8r), [Phantoms](https://objkt.com/collection/KT19bmxutZnuCw5NXakqSJNFgkoa159UdP73), and [Glitched Nouns](https://twitter.com/glitchednouns).  Thus the code has been purposed for image corruption (glitching) and released publicly, Open Sourced (under the MIT License) as 'glitch'.

# Installation
This is a C++ project with some scripts in Python and BASH to help run it for various purposes.  The following installation instructions pertain to Linux (Ubuntu 18.04 in particular).

To compile the code you must first install CMake: 

`> sudo apt-get install -y cmake`

**Optional:** To create GIFs, you need to install Image Magick:

`> sudo apt-get install -y imagemagick`

Then run CMake from the command line in the parent directory of this repository (where CMakeLists.txt lives):

`> cmake .`

Now run Make to build the binaries:

`> make`

# Running the Code
Code has been tested on Linux and Mac OSX.  It will probably work for Windows, but `run.sh` will *not* work unless you run it in Cygwin or some other comparable Linux emulator.

The file corruption binary (glitch) only runs a single pass of corruption.  There are many options for corruption types.

Most of the time in Glitch Art, we want a GIF of a series of corruptions, aggregated together in a GIF.  Two scripts can help with that:

1. run.sh
2. create_gif.py

# Tips
The code works best on BMP's.  The compression formats in JPEGs, PNGs, etc are difficult to corrupt without destroying the image entirely.

Note that if you run too many corruptions, you are likely to destroy the image, rendering it unusable.  This is because the file headers of the image themselves will get corrupted and the OS won't know what to do with the file.

Thus running more corruption passes with less corruptions is preferred to running multiple successive corruptions on a single file.

# Contribution
Feel free to fork this repo as you see fit.  The addition of more corruption mechanisms is always a good way to expand the capabilities of the application.

# Known Issues
There seems to be an issue with the random number generation.  It is not as "random" as it should be, producing a consistent 'band' across some images.
