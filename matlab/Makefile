matlab:
	matlab -nosplash -nodesktop -nojvm -r 'make; quit' | tail +10

octave:
	octave --no-history --silent make.m
	rm -rf *.o

install:
	mv *.mex* ~/local/bin/qp_mex
	cp qpgi.m ~/local/bin/qp_mex

clean:
	rm -rf *.mex*

.PHONY: clean matlab octave
