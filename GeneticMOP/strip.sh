#!/bin/bash

array_file=sampled.m

if [ -e $array_file.sampled ]
then
	rm $array_file.sampled ;
fi

count=0
while read line
do
#	if [[ $(echo $line | grep '; ...' ) == "" ]]
#	then
#		echo $line >> $array_file.sampled
#		continue
#	fi
	
	if [ $[ $[ RANDOM ] % 100 ] -lt $1 ] ;
	then
		echo $line >> $array_file.sampled
	else
		count=$(($count + 1))
	fi
done < $array_file

echo \# Skipped $count lines of $(wc -l $array_file | awk ' { print $1 } ')
