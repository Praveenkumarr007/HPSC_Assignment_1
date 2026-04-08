#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct Particle {
    double y, vy;
};

int main() {
    // Array of different timestep sizes to test
    std::vector<double> dt_values = {0.1, 0.05, 0.01, 0.005, 0.001, 0.0005, 0.0001};
    
    double t_end = 1.0;       // Evaluate error at exactly 1 second
    double g = -9.81;         // Gravity
    
    // Analytical solution at t = 1.0s: y = 0.5 * g * t^2
    double y_exact = 0.5 * g * t_end * t_end; 

    std::ofstream outfile("error_data.csv");
    outfile << "dt,Error\n";

    for (double dt_step : dt_values) {
        Particle p = {0.0, 0.0}; // Start at y=0 with zero velocity
        double t = 0.0;
        double current_dt = dt_step;
        
        // Run simulation until t_end
        while (t < t_end) {
            // Prevent overshooting the exact end time
            if (t + current_dt > t_end) {
                current_dt = t_end - t;
            }
            
            // Semi-implicit Euler
            p.vy += g * current_dt;
            p.y += p.vy * current_dt;
            t += current_dt;
        }
        
        // Calculate Absolute Error
        double error = std::abs(y_exact - p.y);
        outfile << dt_step << "," << error << "\n";
    }

    outfile.close();
    std::cout << "Error analysis complete. Data written to error_data.csv\n";
    return 0;
}