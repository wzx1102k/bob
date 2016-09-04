#! /bin/bash

if [ -z $1 ]; then
echo "usage: ./auto_release.sh mod dir"
echo "---------change image name to regular------"
echo "1. ./auto_release.sh conv dir"
echo "--------move images to sample dir-----------"
echo "2. ./auto_release.sh copy srcdir desdir count"
echo "--------resize images to 200X200------------"
echo "3. ./auto_release.sh resize dir"
echo "--------create samples---------------------"
echo "4. ./auto_release.sh create samples.vec info.dat bg.txt count img_dir"
echo "---------auto------------------------------"
echo "5. ./atuo_release.sh auto srcdir desdir count"
exit 0
fi

conv()
{
	j=1
	for i in ./$1/*.*
	do
		mv 	$i ./$1/$j.jpg
		j=`expr $j + 1`
	done
}

copy()
{
	if [ -d $2 ]; then
		rm -rf $2/*
	else
		mkdir $2			
	fi
	for((i=1; i<=$3;i++));
	do
		cp ./$1/$i.jpg $2 
	done
}

resize()
{
	for i in ./$1/*.jpg
	do
		convert $i -resize 200X200 ${i%.*}.png
	done
	rm -rf ./$1/*.jpg
}

create()
{
	#opencv_createsamples -vec samples.vec -info info.dat -bg bg.txt -num 20 -show -w 200 -h 200
	if [ -f $2 ]; then
		rm -rf $2
	fi
	if [ -f $3 ]; then
		rm -rf $3
	fi
	for i in ./$5/*.png
	do
		echo "$i" >> $3
		echo "$i 1 0 0 200 200" >> $2
	done
	opencv_createsamples -vec $1 -info $2 -bg $3 -num $4
}

if [ -z $2 ]; then
echo "no dir to conv or copy"
exit 0
fi

if [ "$1" == "conv" ]; then
	conv $2
fi

if [ "$1" == "copy" ]; then
	copy $2 $3 $4
fi

if [ "$1" == "resize" ]; then
	resize $2
fi

if [ "$1" == "create" ]; then
	create $2 $3 $4 $5 $6
fi

if [ "$1" == "auto" ]; then
	if [ "$3" == "p_image" ]; then
		BG=p_bg.txt
		VEC=p_samples.vec
		INFO=p_info.dat
	else
		BG=n_bg.txt
		VEC=n_samples.vec
		INFO=n_info.dat
	fi
	copy $2 $3 $4
	resize $3
	create $VEC $INFO $BG $4 $3	
fi




