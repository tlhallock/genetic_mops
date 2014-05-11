#!/bin/bash

rm -r out

for project in Rtree Representer GeneticMOP
do
	pushd $project/build 1>/dev/null
	make clean
	
	if [[ $(basename "$(pwd)") == build ]]
	then
		echo "Entering $(pwd)"
	else
		exit
	fi
	
	if [[ ! -e CMakeLists.txt ]]
	then
		exit
	fi
	
	for file in $(ls)
	do
		if [[ $file == CMakeLists.txt ]]
		then
			continue
		fi
		
		echo "rm -rf $file"
		rm -rf $file
	done
	
	popd  1>/dev/null
done



