# CRes

C ressources is a simple experiment of making self-extracting executables.

## Building

Simply run `make` to build the CRes program.

## How does it work?

CRes is a C program that will read and generate C code containing data from
files it reads. If you compile this generated C `out.c` file, then you will
have a self-extractible executable nammed `extractible`.

First, run `cres` over some files you want to distribute :

    :::console
    $ cres AUTHORS COPYING

Then, you can compile the created `out.c` file by running the specific make
target :

    :::console
    $ make extractible

Finally, move the `extractible` file somewhere and run it to extract the
files :

    :::console
    $ ./extractible
    Extracting AUTHORS...
    Extracting COPYING...
    $ ls
    AUTHORS  COPYING

## Copyright

CRes is brought to you under GNU Affero GPLv3. For further informations, please
read the COPYING file.
