
# Boost.Nowide

This repository is a partial clone of [Boost.Nowide](https://github.com/artyom-beilis/nowide). It was created
to allow an out-of-the-box standalone build on Windows. While the original repository _does_ contain all
the neccessary files and instructions to use nowide in standalone mode, the script that does the needed modifications
to the source files needs a Unix shell - not very feasible when on Windows...

## About this partial clone

The exact version copied is
[ec9672b6cd883193be8451ee4cedab593420ae19](https://github.com/artyom-beilis/nowide/tree/ec9672b6cd883193be8451ee4cedab593420ae19).
I took the files needed for standalone mode and applied the changes that the script applies. The final directory
structure comes with a CMakeLists.txt file which builds and installs the files - at least for my needs...
