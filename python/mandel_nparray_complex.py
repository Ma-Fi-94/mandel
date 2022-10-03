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
z0 = complex(0,0)
LIMIT = 2

def iterate(c):
    i = 0
    z = z0
    while i < MAXITER:
        if abs(z) > LIMIT:
            break
        
        z = z**2 + c
        i = i + 1
     
    return i

# Set up timer
print("Starting execution.\n");
start_time = time.time()

# Setup
iterations = np.zeros((XPIXELS, YPIXELS))
    
# Do the calculation
for i in range(XPIXELS):
    for j in range(YPIXELS):
        c = complex(XMIN + i*XSTEP, YMIN + j*YSTEP)
        iterations[i,j] = iterate(c);
    
# Print out overall time
print("--- %s seconds ---" % (time.time() - start_time))




