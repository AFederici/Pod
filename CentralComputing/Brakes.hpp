#ifndef BRAKES_HPP
#define BRAKES_HPP

#include "Utils.h"
#include "SourceManager.hpp"
#include "MotionModel.hpp"
#include "Simulator.hpp"
#include <cmath>


class Brakes {
  public:

    /*
     * Constructor
     * Setup the PWM pins
     */
    Brakes();

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


    void debug_set_pwm_pin(std::string & path);

  private:

    void set_enable(bool enable);

    //static std::string pwm_chip;
    //static std::string pwm_pin;

    double integral;

    bool enabled;

};

#endif
