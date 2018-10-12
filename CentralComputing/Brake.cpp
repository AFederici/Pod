#include "Brake.hpp"
using namespace Utils;

#ifdef SIM
std::string Brake::pwm_chip = "tests/";
std::string Brake::pwm_pin =  "tests/";
#else
std::string Brake::pwm_chip = "/sys/class/pwm/pwmchip0/";
std::string Brake::pwm_pin =  "/sys/class/pwm/pwmchip0/pwm0/";
#endif

Brake::Brake(){
}

void Brake::enable_brakes() {
  }

void Brake::disable_brakes() {
  }

void Brake::set_enable(bool enable){
}

bool Brake::is_enabled(){
  return enabled;
}

int16_t Brake::calculate_brakeForce(double dt, int16_t last_brakeForce){
    return (int16_t) 0;
}

void Brake::set_brakeForce(int16_t value) {
}
