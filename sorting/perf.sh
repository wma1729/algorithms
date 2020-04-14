#!/bin/sh

genrand 100 > 100.txt
genrand 1000 > 1000.txt
genrand 10000 > 10000.txt
genrand 100000 > 100000.txt
genrand 1000000 > 1000000.txt

for i in 100.txt 1000.txt 10000.txt 100000.txt 1000000.txt
do
	echo "sort `wc -l $i` items"
	echo "bubble: `sort -quiet -in $i -bubble`"
	echo "selection: `sort -quiet -in $i -selection`"
	echo "insertion: `sort -quiet -in $i -insertion`"
	echo "merge recursive: `sort -quiet -in $i -merge_v1`"
	echo "merge iterative: `sort -quiet -in $i -merge_v2`"
	echo "quick normal partition: `sort -quiet -in $i -quick_v1`"
	echo "quick 3-way partition: `sort -quiet -in $i -quick_v2`"
	echo "heap: `sort -quiet -in $i -heap`"
done

rm -f 100.txt 1000.txt 10000.txt 100000.txt 1000000.txt
