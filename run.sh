#!/bin/sh

if ! test -d build
then
	mkdir build
fi

cd build
cmake ..
if make
then
	clear
	./Platformer 5289412085745606899
fi
