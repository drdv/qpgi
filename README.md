# A vanilla implementation of the Goldfarb-Idnani dual algorithm for strictly convex QPs of the form

```
	minimize   0.5*x'*H*x + x'*h
	subject to C*x <= c
```

# Notes:

1. I wouldn't trust this solver if I were you (it hasn't been properly tested).
   Essentially, it summarizes a week of work and I doubt that I would have time
   to develop it any further.

2. It relies on the [Eigen](http://eigen.tuxfamily.org) library.

3. I have borrowed ideas about code structure from [qpmad](https://github.com/asherikov/qpmad).
