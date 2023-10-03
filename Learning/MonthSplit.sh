#!/bin/sh  -x


if [ "$1" = '' ]
then	
	echo "USAGE: MonthSplit.sh file"
	exit 1
fi

FILE=$1

NUM=1

for MONTH in 2023-01 2023-02 2023-03 2023-04 2023-05 2023-06 2023-07 2023-08 2023-09
do 
	grep $MONTH $FILE > xx_$NUM.dat

	NUM=`expr $NUM + 1`
done

