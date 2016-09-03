#! /bin/bash

if [ -z $1]; then
echo "usage: ./auto_change_name.sh dir"
exit 0
fi

j=1
for i in ./$1/*.*
do
mv 	$i ./$1/$j.jpg
j=`expr $j + 1`
done
