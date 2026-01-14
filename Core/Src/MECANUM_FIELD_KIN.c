#include "MECANUM_FIELD_KIN.h"

float rads_to_rpm(float rads)
{
    return (rads * 60.0f) / (2.0f * PI);
}

float rpm_to_rads(float rpm)
{
    return (rpm * 2.0f * PI) / 60.0f;
}

void MecanumRobot_Init(Mecanum_Robot *robot, float max_speed, float max_omega)
{
    robot->Vx = 0.0f;
    robot->Vy = 0.0f;
    robot->omega = 0.0f;
    robot->theta = 0.0f;
    robot->max_speed = max_speed;
    robot->max_omega = max_omega;
    robot->is_yaw_fix = 0;
    robot->fix_angle = 0.0f;
    robot->IMU_theta = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        robot->u[i] = 0.0f;
    }
}

void Joystick_To_Velocity(Mecanum_Robot *robot, float joy_x, float joy_y, float joy_z)
{
    float deadzone = 0.05f;
    if (fabs(joy_x) < deadzone)
        joy_x = 0.0f;
    if (fabs(joy_y) < deadzone)
        joy_y = 0.0f;
    if (fabs(joy_z) < deadzone)
        joy_z = 0.0f;

    robot->Vx = joy_x * robot->max_speed;
    robot->Vy = joy_y * robot->max_speed;
    robot->omega = joy_z * robot->max_omega;
}

void MecanumRobot_CalculateWheelSpeeds(Mecanum_Robot *robot)
{
    float Vx_r = robot->Vx * cosf(robot->theta) + robot->Vy * sinf(robot->theta);
    float Vy_r = -robot->Vx * sinf(robot->theta) + robot->Vy * cosf(robot->theta);

    float sum_LW = ROBOT_LENGTH + ROBOT_WIDTH;

    float w1 = (Vx_r - Vy_r - sum_LW * robot->omega) / WHEEL_RADIUS; // truoc trai
    float w2 = (Vx_r + Vy_r + sum_LW * robot->omega) / WHEEL_RADIUS; // truoc phai
    float w3 = (Vx_r + Vy_r - sum_LW * robot->omega) / WHEEL_RADIUS; // sau phai
    float w4 = (Vx_r - Vy_r + sum_LW * robot->omega) / WHEEL_RADIUS; // sau trai

    robot->u[0] = rads_to_rpm(w1);
    robot->u[1] = rads_to_rpm(w2);
    robot->u[2] = rads_to_rpm(w3);
    robot->u[3] = rads_to_rpm(w4);
}
