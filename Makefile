c++ = g++
dependency = ms.cpp p2d.o kernel.o
chn_dep = channel.hpp matrix.hpp dmatrix.hpp complex.hpp cmatrix.hpp
mf_dep = mf.cpp $(chn_dep) complex.hpp cmatrix.hpp 
ann = /usr/local/share/ann/ann_1.1.2
flags = -O2 -Wall -std=c++11
comp_only = -c
lib_ann = -I$(ann)/include -L$(ann)/lib -lANN
debug = -DDEBUG

kernel.o: kernel.cpp kernel.h
	$(c++) -c $< $(flags) -o $@

p2d.o: p2d.cpp p2d.h
	$(c++) -c $< $(flags) -o $@

ms: $(dependency)
	$(c++) $^ $(flags) $(lib_ann) -o $@

nd: normal_dist.cpp
	$(c++) $^ $(flags) -o $@

clean:
	-@rm ./*.o
	-@rm nd
	-@rm ms
trash: 
	-@rm ./cluster/cluster_* 2>/dev/null || :

sim1: sim1_script.sh nd ms
	@ make trash; \
	./sim1_script.sh

.PHONY: clean trash 
