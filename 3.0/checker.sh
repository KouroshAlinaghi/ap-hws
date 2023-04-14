#!/bin/sh

g++ --std=c++11 $PWD/main.cpp -o $PWD/test.out

for ((i=1; i<5; i++)) do
    $PWD/test.out < "$PWD/$i/$i.in.txt" > "$PWD/$i/$i.my.txt"
    DIFF=$(diff -Bbq "$PWD/$i/$i.my.txt" "$PWD/$i/$i.out.txt")
    if [ "$DIFF" != "" ]
    then
        echo "Wrong answer on test $i!"
        diff -Bb "$PWD/$i/$i.my.txt" "$PWD/$i/$i.out.txt"
    fi
done
