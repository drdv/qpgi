BUILD_TYPE=Debug
#BUILD_TYPE=Release

build:
	rm -rf build
	mkdir build
	cd build; cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..; make

clean:
	rm -rf build
	cd matlab; make clean

.PHONY: build clean
