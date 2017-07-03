#!/bin/bash
./nd <inp >oup
cp ./oup ./cluster/oup
if [ -e inp_src ]
	then
		rm ./inp_src
fi
cut -f 2 -d ':' -s ./script > ./inp_src
./ms inp_src
mv ./log ./cluster/
rm ./inp_src
cd ./cluster/
Rscript plot.r
open Rplots.pdf
cd ..