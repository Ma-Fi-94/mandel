// Simulation parameters
#define XMIN -2
#define YMIN -2
#define XSTEP 0.004
#define YSTEP 0.004
#define XPIXELS 1000 // 15k maximum
#define YPIXELS 1000
#define MAXITER 1000

// Do not touch these
#define Z0_R 0
#define Z0_I 0
#define LIMIT_2 4

// Libattopng
#include "libattopng.h"
#include "libattopng.c"
#define RGBA(r, g, b, a) ((r) | ((g) << 8) | ((b) << 16) | ((a) << 24))

// Other libs
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "mandel.h"

int* find_minmax(int* iterations) {
    /*
     * Find min., max. number of iterations present in results
    */

    int* result = malloc(2*sizeof(int));
    int min_iterations = MAXITER;
    int max_iterations = 0;
    int cur;
    
    for (int i = 0; i < XPIXELS; i++) {
        for (int j = 0; j < YPIXELS; j++) {
            cur = *(iterations+i*YPIXELS+j);
            if (cur > max_iterations) {max_iterations = cur;}
            if (cur < min_iterations) {min_iterations = cur;}
        }
    }
    
    result[0] = min_iterations;
    result[1] = max_iterations;
    return result;
}

inline int iterate(float c_r, float c_i) {
    /*
     * Iterate a single cell on the lattice
    */
    
    // Initialise iteration
    float zn_r = Z0_R;
    float zn_i = Z0_I;
    
    // Store squared values here to avoid some multiple calculations
    float zn_r_2, zn_i_2;

    // The iteration itself
    int i = 0;
    for (; i < MAXITER; i++) {
        // End iteration if |x| has reached limit
        zn_r_2 = zn_r * zn_r;
        zn_i_2 = zn_i * zn_i;
        if (zn_r_2 + zn_i_2 > LIMIT_2) {
            break;
        }
        
        // Iteration step
        zn_i = 2 * zn_r * zn_i - c_i;
        zn_r = zn_r_2 - zn_i_2 + c_r;
    }
    
    return i;
}

inline int* cmap(int i, int min, int max) {
    /*
     * Map i onto RGB tuple
    */
    
    float intensity = (log2(i) - log2(min)) / (log2(max)-log2(min));
    
    // Blues
    int r = (int) (235 * (1-intensity)*(1-intensity)*(1-intensity)) + 20;
    int g = (int) (235 * (1-intensity)*(1-intensity)) + 20;
    int b = (int) (215 * (1-intensity)) + 40;
    
    // Greyscales
    /*
    int r = (int) 255 * (1-intensity) * (1-intensity);
    int g = (int) 255 * (1-intensity) * (1-intensity);
    int b = (int) 255 * (1-intensity) * (1-intensity);
    */

    int* tuple = malloc(3*sizeof(int));
    tuple[0] = r;
    tuple[1] = g;
    tuple[2] = b;
    return tuple;
}


void make_plot(int* iterations) {
    /*
     * Generate and export the plot
    */
    
    // Find min, max of iterations needed
    int* minmax = find_minmax(iterations);
    int min_iterations = *minmax;
    int max_iterations = *(minmax+1);
    
    // Make plot itself
    libattopng_t* png = libattopng_new(XPIXELS, YPIXELS, PNG_RGBA);
    int cur,r,g,b;
    int* rgb;
    for (int i = 0; i < XPIXELS; i++) {
        for (int j = 0; j < YPIXELS; j++) {
            cur = *(iterations+i*YPIXELS+j); 
            rgb = cmap(cur, min_iterations, max_iterations);
            r = rgb[0];
            g = rgb[1];
            b = rgb[2];
            libattopng_set_pixel(png, i, YPIXELS-j, RGBA(r,g,b,255));
            free(rgb);
        }
    }
    
    // Add x axis to plot
    if (XMIN <= 0 && XMIN + XPIXELS * XSTEP >= 0) {
        int xpos_yaxis = -XMIN / XSTEP;
        for (int i = 0; i < YPIXELS; i=i+3) {
            libattopng_set_pixel(png, xpos_yaxis, i, RGBA(255,255,255,255));
        }
    }
    
    // Add y axis to plot
    if (YMIN <= 0 && YMIN + YPIXELS * YSTEP >= 0) {
        int ypos_xaxis = -YMIN / YSTEP;
        for (int i = 0; i < XPIXELS; i=i+3) {
            libattopng_set_pixel(png, i, ypos_xaxis, RGBA(255,255,255,255));
        }
    }
    
    // Export to HDD
    libattopng_save(png, "output.png");
    libattopng_destroy(png);
}

int main() {
    // Set up timer
    printf("Starting execution.\n");
    clock_t begin = clock();
    
    // Allocate memory
    int *iterations;
    iterations = (int*) calloc (XPIXELS*YPIXELS, sizeof(int));
    float c_r;
    float c_i;
    
    // Do the calculation
    for (int i = 0; i < XPIXELS; i++) {
        for (int j = 0; j < YPIXELS; j++) {
            c_r = XMIN + i*XSTEP;
            c_i = YMIN + j*YSTEP;
            *(iterations+i*YPIXELS+j) = iterate(c_r, c_i);
        }
    }
    
    // Print out calculation time
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Spent %f seconds on calculating %i pixels.\n", time_spent, XPIXELS*YPIXELS);
    begin = clock();
    
    // Make plot; print required time
    begin = clock();
    make_plot(iterations);
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Spent %f seconds on generating plot.\n\n", time_spent, XPIXELS*YPIXELS);

    return 0;
}










