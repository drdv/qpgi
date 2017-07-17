all: get_cpp_tests

get_cpp_tests:
	svn export https://projects.coin-or.org/svn/qpOASES/misc/testingdata/cpp cpp

get_matlab_tests:
	svn export https://projects.coin-or.org/svn/qpOASES/misc/testingdata/matlab matlab

clean:
	rm -rf cpp
	rm -rf matlab

.PROXY: clean all get_cpp_tests get_matlab_tests
