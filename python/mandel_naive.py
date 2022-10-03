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
X0_R = 0
X0_I = 0
LIMIT_2 = 4

def iterate(c_r, c_i):
    zn_r = X0_R
    zn_i = X0_I
    
    i = 0
    while i < MAXITER:
        zn_r_2 = zn_r * zn_r
        zn_i_2 = zn_i * zn_i
        if zn_r_2 + zn_i_2 > LIMIT_2:
            break
        
        zn_i = 2 * zn_r * zn_i - c_i
        zn_r = zn_r_2 - zn_i_2 + c_r
        
        i = i + 1
     
    return i

# Set up timer
print("Starting execution.\n");
start_time = time.time()

# Setup
iterations = []
    
# Do the calculation
for i in range(XPIXELS):
    line = []
    for j in range(YPIXELS):
        c_r = XMIN + i*XSTEP
        c_i = YMIN + j*YSTEP
        line.append(iterate(c_r, c_i))
    iterations.append(line)
    
# Print out overall time
print("--- %s seconds ---" % (time.time() - start_time))




