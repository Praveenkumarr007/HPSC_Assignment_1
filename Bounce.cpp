#include <iostream>
#include <fstream>
#include <algorithm> // For std::max

struct Particle {
    double x, y, z;
    double vx, vy, vz;
    double fx, fy, fz;
    double mass;
    double radius;
};

int main() {
    // 1. Simulation Parameters
    // We use a much smaller timestep (0.001) because spring calculations 
    // become unstable and "explode" if the time steps are too big!
    double dt = 0.001;          
    double t_max = 10.0;
    double g = -9.81;
    
    // Spring-Dashpot Parameters (You can tweak these!)
    double k_n = 5000.0;  // Spring stiffness (how bouncy the floor is)
    double gamma_n = 5.0; // Damping coefficient (how fast it loses energy)

    Particle p1;
    p1.x = 0; p1.y = 0; p1.z = 10.0;
    p1.vx = 0; p1.vy = 0; p1.vz = 0;
    p1.mass = 1.0;
    p1.radius = 0.1;

    // 2. Prepare the new CSV file
    std::ofstream outfile("data_bounce.csv");
    outfile << "Time,Height,KineticEnergy\n";

    std::cout << "Starting Particle Bounce Simulation...\n";

    // 3. Time Integration Loop
    for (double t = 0; t <= t_max; t += dt) {
        
        // Calculate Kinetic Energy: 1/2 * m * v^2
        double ke = 0.5 * p1.mass * (p1.vz * p1.vz);
        
        // Write data to file
        outfile << t << "," << p1.z << "," << ke << "\n";

        // A. Reset forces
        p1.fz = 0;

        // B. Add Gravity
        p1.fz += p1.mass * g;

        // C. Check Wall Contact (Floor is at z = 0)
        double overlap = p1.radius - p1.z; 
        
        if (overlap > 0) { // If overlap > 0, the particle is hitting the floor!
            
            // Calculate the Spring-Dashpot force
            // Damping opposes velocity. If moving down (-vz), damping pushes up (+)
            double damping_force = -gamma_n * p1.vz; 
            double spring_force = k_n * overlap;
            
            double wall_force = spring_force + damping_force;
            
            // Equation 11: Prevent nonphysical attractive forces
            wall_force = std::max(0.0, wall_force); 
            
            p1.fz += wall_force;
        }

        // D. Update Velocity (Semi-implicit Euler)
        p1.vz += (p1.fz / p1.mass) * dt;

        // E. Update Position
        p1.z += p1.vz * dt;
    }

    outfile.close();
    std::cout << "Simulation complete! Data saved to data_bounce.csv\n";

    return 0;
}
