Collection of C++ files for the generation of a kd tree and querying of tree to
find k-NN of an n-dimensional vector.  Files should be straightforward enough
to use.

Dependencies:
	- Standard C / C++ libraries, cmath, cstring, etc ... 
	- cnpy, not necessary, just aids in reading of data when using

These files will eventually be wrapped in a C wrapper, a fortran wrapper, and
passed to f2py to allow a python interface to this C++ function.

To compile, run:

	g++ *.cpp -L/usr/local/lib -lcnpy -o [executable name]

This is assuming you have installed cnpy correctly and are using it.  Otherwise,
comment out the cnpy lines and input your data.

- Gabriel Vacaliuc - edited - 04/11/16
