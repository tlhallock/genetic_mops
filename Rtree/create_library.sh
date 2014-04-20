

library=qtree.a
if [ -e $library ]
then
    rm -f $library
fi


o_files=""


for file in $(find "$@" | grep '\.o')
do
    if [[ ! -z $(readelf -Ws $file | grep main | grep FUNC) ]]
    then
	continue
    fi

    o_files="$o_files $file"
done

set -x
ar rcs $library $o_files
