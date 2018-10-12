#ifndef BRAKE_HPP
#define BRAKE_HPP

#include "Utils.h"
#include "SourceManager.hpp"
#include "MotionModel.hpp"
#include <cmath>

//TODO: Better define these constants
#define RPM_PDR 600.0  //point of diminishing returns
#define DISK_CIRCUMFRENCE 10.0 //In Meters
#define RPM_TO_MS (DISK_CIRCUMFRENCE/60.0) // RPM to Meters per second
#define MS_TO_RPM (60.0/DISK_CIRCUMFRENCE) // Meters per second to RPM
//TODO: Better define this constant
#define RPM_TO_USEC (25.0/400.0) // Usec/RRPM

#define BRAKE_OFF -50.0	//change these constants


#define KP 1.0
#define KD 0.0
#define KI 1.0

class Brake {
  public:

/*
     * Constructor
     * Setup the PWM pins
     */
    Brake();

    /*
     * Arms the motors
     */
    void enable_brakes();

    /*
     * Disarms the motors
     */
    void disable_brakes();

    /*
     * is_enabled returns enabled variable
     */
    bool is_enabled();

    /*
     * Uses a PI loop to calculate the appropriate motor throttle
     */
    int16_t calculate_brakeForce(double dt, int16_t last_brakeForce);

    /*
     * Sets the motor throttle to a specific value
     */
    void set_brakeForce(int16_t value);

    void debug_set_pwm_pin(std::string & path);

  private:

    void set_enable(bool enable);

    static std::string pwm_chip;
    static std::string pwm_pin;

    double integral;

    bool enabled;

};

#endif
