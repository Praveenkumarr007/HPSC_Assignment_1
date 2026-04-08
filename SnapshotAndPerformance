#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <omp.h>
#include <chrono>

struct Particle {
    double x, y, z;
    double fx, fy, fz;
    double radius;
};

// Global variables for the simulation
const int N = 5000;
const double k_n = 5000.0;

// The function we are going to parallelize!
void compute_contacts(std::vector<Particle>& particles, int num_threads) {
    // Set the number of OpenMP threads
    omp_set_num_threads(num_threads);

    // OpenMP directive to split this loop across multiple CPU cores
    // We use schedule(dynamic) because some particles have more neighbors than others
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < N; ++i) {
        double local_fx = 0, local_fy = 0, local_fz = 0;

        for (int j = 0; j < N; ++j) {
            if (i == j) continue; // Don't check a particle against itself

            double dx = particles[j].x - particles[i].x;
            double dy = particles[j].y - particles[i].y;
            double dz = particles[j].z - particles[i].z;
            double dist = std::sqrt(dx*dx + dy*dy + dz*dz);

            double overlap = particles[i].radius + particles[j].radius - dist;

            if (overlap > 0 && dist > 0) {
                // Spring force
                double force = k_n * overlap;
                local_fx -= force * (dx / dist);
                local_fy -= force * (dy / dist);
                local_fz -= force * (dz / dist);
            }
        }
        // Write the calculated force to the particle
        particles[i].fx += local_fx;
        particles[i].fy += local_fy;
        particles[i].fz += local_fz;
    }
}

int main() {
    std::vector<Particle> particles(N);

    // 1. Initialize 5000 particles randomly in a 3D box
    for (int i = 0; i < N; ++i) {
        particles[i].x = ((double)rand() / RAND_MAX) * 10.0;
        particles[i].y = ((double)rand() / RAND_MAX) * 10.0;
        particles[i].z = ((double)rand() / RAND_MAX) * 10.0;
        particles[i].radius = 0.1;
    }

    // 2. Prepare the Output File for the Python Artist
    std::ofstream outfile("data_performance.csv");
    outfile << "Threads,Time,Speedup,Efficiency\n";

    std::cout << "Running OpenMP Profiling for N=" << N << " particles...\n";

    double time_1_thread = 0.0;
    int thread_counts[] = {1, 2, 4, 8};

    // 3. Test the performance with 1, 2, 4, and 8 threads
    for (int threads : thread_counts) {
        
        // Start a timer
        auto start = std::chrono::high_resolution_clock::now();
        
        // Run the math heavy function
        compute_contacts(particles, threads);
        
        // Stop the timer
        auto stop = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = stop - start;
        double time_taken = duration.count();

        // Calculate Speedup and Efficiency
        if (threads == 1) time_1_thread = time_taken;
        double speedup = time_1_thread / time_taken;
        double efficiency = speedup / threads;

        std::cout << "Threads: " << threads 
                  << " | Time: " << time_taken << "s"
                  << " | Speedup: " << speedup 
                  << " | Efficiency: " << efficiency << "\n";

        // Write to notebook for Python
        outfile << threads << "," << time_taken << "," << speedup << "," << efficiency << "\n";
    }
    outfile.close();

    // 4. Save a snapshot of the 3D particle cloud for the final plot
    std::ofstream snapfile("data_snapshot_final.csv");
    snapfile << "X,Y,Z\n";
    for(int i=0; i<N; ++i) {
        snapfile << particles[i].x << "," << particles[i].y << "," << particles[i].z << "\n";
    }
    snapfile.close();

    std::cout << "Done! Saved data_performance.csv and data_snapshot_final.csv\n";
    return 0;
}
