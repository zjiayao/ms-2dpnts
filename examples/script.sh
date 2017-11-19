#!/bin/bash

cut -f 2 -d ':' -s ./examples/script > ./examples/input_script
./ms ./examples/input_script
mv ./log ./cluster/
rm ./examples/input_script
cd ./cluster/
Rscript plot.r
open Rplots.pdf
cd ..
