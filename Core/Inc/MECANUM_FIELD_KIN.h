#ifndef __MECANUM_FIELD_KIN_H__
#define __MECANUM_FIELD_KIN_H__

#include "math.h"
#include "stdint.h"

#define PI 3.1415
#define WHEEL_RADIUS 0.485f // Wheel radius in meters
#define ROBOT_LENGTH 0.52f  // Robot length in meters
#define ROBOT_WIDTH 0.43f   // Robot width in meters

typedef struct
{

    float Vx;           // Velocity in the x direction (m/s)
    float Vy;           // Velocity in the y direction (m/s)
    float omega;        // Angular velocity (rad/s)
    float theta;        // Orientation angle (radians)
    float max_speed;    // Maximum speed (m/s)
    float max_omega;    // Maximum angular velocity (rad/s)
    uint8_t is_yaw_fix; // Yaw fix flag
    float fix_angle;    // Fixed angle (radians)
    float IMU_theta;    // IMU measured angle (radians)
    float u[4];         // Wheel speeds (rpm)
} Mecanum_Robot;

void MecanumRobot_Init(Mecanum_Robot *robot, float max_speed, float max_omega);
void Joystick_To_Velocity(Mecanum_Robot *robot, float joy_x, float joy_y, float joy_z);
void MecanumRobot_CalculateWheelSpeeds(Mecanum_Robot *robot);

float rads_to_rpm(float rads);
float rpm_to_rads(float rpm);

#endif // __MECANUM_FIELD_KIN_H__
