#!/bin/bash
echo Filename
read "filename"
echo ThresholdValue
read "t"
counter=0
filesize=$(stat -c%s "$filename")
partition()
{
	local f=$1
	filesize=$(stat -c%s "$f")
        if [ "$filesize" -gt "$t" ]
                then
			let counter=counter+1
			local c=$counter
			v=$(wc -l < $f) 
			
			w=$(($v/2))
			
			q=$(($w+1))
				
		        split -l $q $f $counter
			partition "$c"aa
			partition "$c"ab 
                        sort -n "$c"aa "$c"ab  > k
                        mv k $f

                else
                        sort -n $f > o
                        mv o $f
        fi
   }
if [ "$filesize" -gt "$t" ]
        then
                partition $filename 
        else
                d=n
                sort -n $filename > $d
                mv $d $filename
fi
exit
