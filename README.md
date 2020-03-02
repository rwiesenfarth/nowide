
# Boost.Nowide

This repository is a partial clone of [Boost.Nowide](https://github.com/boostorg/nowide). It was created
to allow an out-of-the-box standalone build on Windows. While the original repository _does_ contain all
the neccessary files and instructions to use nowide in standalone mode, the script that does the needed modifications
to the source files needs a Unix shell - not very feasible when on Windows...

## About this partial clone

The exact version copied is
[959d3d0146bf07055d80f6049d0dc765fe9dd1a0](https://github.com/boostorg/nowide/commit/959d3d0146bf07055d80f6049d0dc765fe9dd1a0).
I took the files needed for standalone mode and applied the changes that the script applies. In addition, I had to
fix some parts of the code manually, where it assumed Boost macros being present.

The final directory structure comes with a CMakeLists.txt file which builds and installs the files - at least for my
needs...
