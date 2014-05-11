#!/bin/bash

for project in Rtree Representer GeneticMOP
do
	cd $project/build
	cmake -DCMAKE_BUILD_TYPE=Debug .
	make clean
	make
	make install
	cd -
done