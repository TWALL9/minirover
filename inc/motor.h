#ifndef MOTOR_H
#define MOTOR_H

/** 
 * @brief Motor direction enum
 */ 
typedef enum
{
    MOTOR_FORWARD = 0,
    MOTOR_REVERSE,
    MOTOR_NEUTRAL,
    MOTOR_BRAKE
} MotorDirection_t;

class Motor 
{
    public:
        Motor();
        MotorDirection_t get_direction(void);
        void set_speed(uint8_t duty_cycle);
        void set_direction(MotorDirection_t direction);
    
    private:
        MotorDirection_t direction;
        uint8_t speed;
        
}

#endif /* MOTOR_H */