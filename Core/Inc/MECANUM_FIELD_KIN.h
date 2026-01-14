#ifndef __MECANUM_FIELD_KIN_H__
#define __MECANUM_FIELD_KIN_H__

#include "math.h"
#include "stdint.h"

// test

#define PI 3.1415
#define WHEEL_RADIUS 0.485f // Ban kinh banh xe (m)
#define ROBOT_LENGTH 0.52f  // Chieu dai robot (m)
#define ROBOT_WIDTH 0.43f   // Chieu rong robot (m)

typedef struct
{

    float Vx;           // Van toc theo truc x (m/s)
    float Vy;           // Van toc theo truc y (m/s)
    float omega;        // Toc do goc (rad/s)
    float theta;        // Goc huong robot (rad)
    float max_speed;    // Van toc toi da (m/s)
    float max_omega;    // Toc do goc toi da (rad/s)
    uint8_t is_yaw_fix; // Co khoa yaw (0/1)
    float fix_angle;    // Goc khoa (rad)
    float IMU_theta;    // Goc do IMU do (rad)
    float u[4];         // Toc do banh xe (rpm)
} Mecanum_Robot;

void MecanumRobot_Init(Mecanum_Robot *robot, float max_speed, float max_omega);
void Joystick_To_Velocity(Mecanum_Robot *robot, float joy_x, float joy_y, float joy_z);
void MecanumRobot_CalculateWheelSpeeds(Mecanum_Robot *robot);

float rads_to_rpm(float rads);
float rpm_to_rads(float rpm);

#endif // __MECANUM_FIELD_KIN_H__
