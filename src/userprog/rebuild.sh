# rebuild script
make clean
make
cd build

# make file disk
pintos-mkdisk filesys.dsk --filesys-size=2
pintos -f -q

# compile examples
cd ../../examples
make

# make copy example program to pintos disk
cd ../userprog/build
pintos -p ../../examples/echo -a echo -- -q