
#include "dynamics.h"
#include "../graphics/gfx.h"
#include <iostream>
#include "MPU6050.h"

void updateMarple(Marple* marple, MPU6050* gyro, bool debug=false) {
    float x, y, z;
    float weighting_factor = 0.001;
    float x_pos = marple->getPos()[0], y_pos = marple->getPos()[1];
    gyro->getGyro(&x, &y, &z);
    marple->x_acceleration = x * weighting_factor;
    marple->y_acceleration = y * weighting_factor;
    //apply calculated values
    marple->x_velocity += (marple->x_acceleration);
    marple->y_velocity += (marple->y_acceleration);
    marple->move(marple->x_velocity, marple->y_velocity);
    if (debug) {
        std::cout << "pos: " << x_pos << ", " << y_pos << "\n";
        std::cout << "gyro: " << x << ", " << y << "\n";
        std::cout << "x_acceleration: " << marple->x_acceleration << ", y_acceleration: " << marple->y_acceleration << "\n";
        std::cout << "x_velocity: " << marple->x_velocity << ", y_velocity: " << marple->y_velocity << "\n";
    }
}

/*pass in marple object
 * get current velocity
 * read current angle - > acceleration
 *          100 -> -100 : (1p/s)/s -> -1p/s/s
 * apply the acceleration
 *          position + (velocity + acceleration) = new position

 once new position obtained, check for collisions
    *if in new position there is a wall
        *check if marple new position would touch wall or be inside wall
            *if inside wall, reposition so it only touches wall
            *if touching wall invert the acceleration so it "bounces off" wall
        *check if marple new position would touch hole
            *if touches hole, set new psotion to start position to teleport it back to the start.
            */
