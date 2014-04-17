#!/bin/bash

array_file=sampled.m

if [ -e $array_file ]
then
	rm $array_file
fi

for prefix in $(echo sampled inc ga)
do
	echo $prefix
	
	echo all_$prefix = [... >> $array_file
	
	sed 's/.* -> \[\(.*\)\]/\t\1 ; ... /g' `echo $prefix`_points.txt >> $array_file
	echo ] >> $array_file
	
	echo scatter\(all_$prefix\(:,1\), all_$prefix\(:,2\)\) >> $array_file
	echo 'drawnow' >> $array_file
	echo input\(\"that was all $prefix. press any key to continue\"\) >> $array_file
	
	echo pareto_$prefix = [... >> $array_file
	
	sed 's/\(.*\)/\t\1 ; ... /g' `echo $prefix`_pareto.txt >> $array_file
	echo ] >> $array_file
	
	echo scatter\(pareto_$prefix\(:,1\), pareto_$prefix\(:,2\)\) >> $array_file
	echo 'drawnow' >> $array_file
	echo input\(\"that was pareto $prefix. press any key to continue\"\) >> $array_file
done

#octave $array_file
