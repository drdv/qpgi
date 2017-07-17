matlab:
	matlab -nosplash -nodesktop -nojvm -r 'make; quit' | tail +10

octave:
	octave --no-history --silent make.m
	rm -rf *.o

clean:
	rm -rf *.mex*

.PHONY: clean matlab octave
