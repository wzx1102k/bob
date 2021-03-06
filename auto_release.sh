#! /bin/bash

if [ -z $1 ]; then
echo "usage: ./auto_release.sh mod dir"
echo "---------change image name to regular------"
echo "1. ./auto_release.sh conv dir"
echo "--------move images to sample dir-----------"
echo "2. ./auto_release.sh copy srcdir desdir count"
echo "--------resize images to 200X200------------"
echo "3. ./auto_release.sh resize dir size"
echo "--------create samples---------------------"
echo "4. ./auto_release.sh create samples.vec info.dat bg.txt count img_dir size"
echo "----------harr training samples-----------------"
echo "5. ./auto_release.sh harr_train classifier samples.vec n_bg.txt npos nneg nstage size"
echo "----------lbp training samples-----------------"
echo "5. ./auto_release.sh lbp_train classifier samples.vec n_bg.txt npos nneg nstage size"
echo "---------auto------------------------------"
echo "7. ./auto_release.sh auto pos_dir neg_dir count size type"
echo "----------test-----------------------------"
echo "8. ./auto_release.sh test test_dir size"
exit 0
fi

conv()
{
	for i in ./$1/1*
	do
		DIR=${i##*.}
		break
	done
	j=1
	for i in ./$1/*
	do
		mv $i ./$1/$j
		j=`expr $j + 1`
	done

	j=1
	for i in ./$1/*
	do
		mv $i ./$1/$j.$DIR
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
	for i in ./$1/1*
	do
		DIR=${i##*.}
		break
	done
	for((i=1; i<=$3;i++));
	do
		cp ./$1/$i.$DIR $2 2>/dev/NULL
	done
}

resize()
{
echo "$2"X"$2"
	for i in ./$1/1*
	do
		DIR=${i##*.}
		break
	done
	for i in ./$1/*
	do
		convert $i -colorspace Gray $i
		convert $i -resize $2X$2 ${i%.*}.bmp
		if [ "$DIR" != "bmp" ]; then
			rm -rf $i
		fi
	done
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
	for i in $5/*
	do
		echo "$i" >> $3
		echo "$i 1 0 0 $6 $6" >> $2
	done
	opencv_createsamples -vec $1 -info $2 -bg $3 -num $4 -w $6 -h $6
}

harr_train()
{
	#opencv_haartraining -data classifier -vec p_samples.vec -bg n_bg.txt -npos 360 -nneg 120 -nstages 5 -w 50 -h 50
	#opencv_haartraining -data $1 -vec $2 -bg $3 -mem 500 -npos $4 -nneg $5 -nstages $6 -w $7 -h $7 -minhitrate 0.999 -mode ALL
    opencv_traincascade -data $1 -vec $2 -bg $3 -numPos $4 -numNeg $5 -numStages $6 -featureType HARR -w $7 -h $7 -mode BASIC
}

lbp_train()
{
    opencv_traincascade -data $1 -vec $2 -bg $3 -numPos $4 -numNeg $5 -numStages $6 -precalcdxBufSize 1000 -featureType LBP -w $7 -h $7 -minHitRate 0.999 -mode BASIC -maxFalseAlarmRate 0.3
}

auto()
{
	P_BG=p_bg.txt
	P_VEC=p_samples.vec
	P_INFO=p_info.dat
	P_DIR=p_image
	N_BG=n_bg.txt
	N_VEC=n_samples.vec
	N_INFO=n_info.dat
	N_SIZE=100
	N_DIR=n_image

	copy $1 $P_DIR $3
	copy $2 $N_DIR $3
	resize $P_DIR $4
	resize $N_DIR $N_SIZE

	P_NUM=`find $P_DIR -type f -print|wc -l`
	N_NUM=`find $N_DIR -type f -print|wc -l`
	create $P_VEC $P_INFO $P_BG $P_NUM $P_DIR $4
	create $N_VEC $N_INFO $N_BG $N_NUM $N_DIR $N_SIZE

	CLASS=sample_classifier
	#NPOS=360
	#NNEG=120
	#NNEG=$[$3/4]	
	NNEG=2400	
	#NPOS=$[$3-$NNEG-5]	
	NPOS=1200	
	NSTAGE=20
	echo $5
    if [ "$5" == "harr" ]; then
	    harr_train $CLASS $P_VEC $N_BG $NPOS $NNEG $NSTAGE $4
    else
        lbp_train $CLASS $P_VEC $N_BG $NPOS $NNEG $NSTAGE $4
    fi
}

test()
{
    VEC=test.vec
    DAT=test.dat
    BG=test.txt
    NUM=`find $1 -type f -print|wc -l`
    resize $1 $2
	cd $1
    create $VEC $DAT $BG $NUM . $2
	opencv_performance -data ../sample_classifier.xml -info $DAT -ni
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
	resize $2 $3
fi

if [ "$1" == "create" ]; then
	create $2 $3 $4 $5 $6
fi

if [ "$1" == "auto" ]; then
	auto $2 $3 $4 $5 $6
fi

if [ "$1" == "test" ]; then
    test $2 $3
fi




