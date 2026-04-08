#include <iostream>
#include <vector>
#include <fstream>

// 1. Define our Particle based on the physical model requirements
struct Particle {
    double x, y, z;       // Position
    double vx, vy, vz;    // Velocity
    double fx, fy, fz;    // Force
    double mass;
    double radius;
};

int main() {
    // 2. Setup the simulation parameters
    double dt = 0.01;           // Timestep
    double t_max = 2.0;         // Total simulation time
    double g = -9.81;           // Gravity (m/s^2) pointing down in Z axis
    
    // Create one particle for the Free Fall test
    Particle p1;
    p1.x = 0; p1.y = 0; p1.z = 10.0; // Starting at 10 meters high
    p1.vx = 0; p1.vy = 0; p1.vz = 0; // Dropped from rest
    p1.mass = 1.0;
    p1.radius = 0.1;

    // 3. Prepare the CSV file to write our data
    std::ofstream outfile("data_freefall.csv");
    outfile << "Time,Z_numerical,Z_analytical\n"; // Header row

    std::cout << "Starting Free Fall Simulation...\n";

    // 4. The Time Integration Loop (Semi-Implicit Euler)
    for (double t = 0; t <= t_max; t += dt) {
        
        // A. Calculate analytical (exact) position for comparison: z(t) = z0 - 0.5 * g * t^2
        double z_analytical = 10.0 - 0.5 * 9.81 * (t * t);

        // B. Write current state to CSV before moving
        outfile << t << "," << p1.z << "," << z_analytical << "\n";

        // C. Calculate Forces (Only gravity for this test)
        p1.fx = 0;
        p1.fy = 0;
        p1.fz = p1.mass * g; 

        // D. Update Velocity: v_{n+1} = v_n + (F/m) * dt
        p1.vx += (p1.fx / p1.mass) * dt;
        p1.vy += (p1.fy / p1.mass) * dt;
        p1.vz += (p1.fz / p1.mass) * dt;

        // E. Update Position: x_{n+1} = x_n + v_{n+1} * dt
        p1.x += p1.vx * dt;
        p1.y += p1.vy * dt;
        p1.z += p1.vz * dt;
    }

    outfile.close();
    std::cout << "Simulation complete! Data saved to data_freefall.csv\n";

    return 0;
}
