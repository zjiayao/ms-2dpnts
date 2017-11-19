c++ = g++
dependency = ms.cpp p2d.o kernel.o
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

example: ms ./examples/script ./examples/script.sh
	@make trash; \
	./examples/script.sh

clean:
	-@rm ./*.o
	-@rm ms
trash:
	-@rm ./cluster/cluster_* 2>/dev/null || :

.PHONY: clean trash
