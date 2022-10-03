# based on: https://www.learnpythonwithrune.org/numpy-compute-mandelbrot-set-by-vectorization/

import numpy as np
import time


# You may touch these
XMIN = -2
YMIN = -2
XSTEP = 0.004
YSTEP = 0.004
XPIXELS = 1000
YPIXELS = 1000
MAXITER = 1000

# No touchies! >.<
LIMIT = 2

# Set up timer
print("Starting execution.\n");
start_time = time.time()

# Iteration happens here
z = np.zeros((XPIXELS, YPIXELS), dtype=complex)

# Results go here
div_time = np.zeros(z.shape, dtype=int)

# to keep track which elements have not diverged yet
nondiverged = np.full(z.shape, True, dtype=bool)

# Set up c_{i,j} array
x = np.linspace(XMIN, XMIN+XPIXELS*XSTEP, XPIXELS).reshape((1, XPIXELS))
y = np.linspace(YMIN, YMIN+YPIXELS*YSTEP, YPIXELS).reshape((YPIXELS, 1))
c = x + 1j * y

# Iteration itself
for i in range(MAXITER):
    # Iteration step on those elements, which have not diverged yet
    z[nondiverged] = z[nondiverged]**2 + c[nondiverged]

    # Find elements which just started to diverge this round
    # We begin an array full of False ("out"),
    # and only write to it at those places, where
    # the elements in z have not diverged yet
    diverged = np.greater(np.abs(z), 2, out=np.full(z.shape, False), where=nondiverged)
    
    # set the value of the newly diverged iteration number
    div_time[diverged] = i
    
    # to remember which have diverged
    nondiverged[np.abs(z) > 2] = False


# Print out overall time
print("--- %s seconds ---" % (time.time() - start_time))




