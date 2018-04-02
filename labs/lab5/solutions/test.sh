#! /bin/sh

R=$1
C=$2

I=$3
O=$4

./Mazemake ${R:=80} ${C:=120} > maze.txt

./Mazesolve < maze.txt > testpath.txt
./mazesolve < maze.txt > solpath.txt

if ! diff -q testpath.txt solpath.txt > /dev/null ; then
    echo Mazesolve did not produce the same path as the solution executable with no arguments. See "testpath.txt" and "solpath.txt" for differences.
    exit 10
else
    echo Mazesolve produced the same path as the solution executable with no arguments.
fi

./Mazesolve ${I:=20} ${O:=7200} < maze.txt > testpath1.txt
./mazesolve ${I:=20} ${O:=7200} < maze.txt > testpath2.txt

if ! diff -q testpath.txt solpath.txt > /dev/null ; then
    echo Mazesolve did not produce the same path as the solution executable with arguments. See "testpath1.txt" and "solpath1.txt" for differences.
    rm testpath1.txt testpath2.txt
    exit 13
else
    echo Mazesolve produced the same path as the solution executable with arguments.
fi
rm testpath1.txt testpath2.txt

./Mazeshow maze.txt > maze1.ppm
./mazeshow maze.txt > solmaze1.ppm

if ! diff maze1.ppm solmaze1.ppm > /dev/null ; then
    echo Mazesolve did not produce the same maze PPM as the solution executable with no path given.
    rm testpath.txt
    rm solpath.txt
    exit 11
else
    echo Mazesolve produced the same maze PPM as the solution executable with no path given.
fi

./Mazeshow maze.txt testpath.txt > maze2.ppm
./mazeshow maze.txt solpath.txt > solmaze2.ppm

if ! diff maze2.ppm solmaze2.ppm > /dev/null ; then
    echo Mazesolve did not produce the same maze PPM as the solution executable with path given.
    exit 12
else
    echo Mazesolve produced the same maze PPM as the solution executable with path given.
fi

rm maze.txt
rm testpath.txt
rm solpath.txt
# Makes PNGs of each PPM in case you still want to see them
cat maze1.ppm | convert - maze1.png
cat maze2.ppm | convert - maze2.png
rm maze1.ppm
rm maze2.ppm
rm solmaze1.ppm
rm solmaze2.ppm
