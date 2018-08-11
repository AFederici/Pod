#include "MotionModel.hpp"

bool MotionModel::initialize_source(){
  state.x[0] = 0;
  state.x[1] = 0;
  state.x[2] = 0;
  state.rpm = 0;
  state.fM = 0;
  state.fD = 0;
  
  last_time = microseconds();

  print(LogLevel::LOG_DEBUG, "Motion Model setup successful\n");
  return true;
}

void MotionModel::stop_source(){

  SourceManager::PRU_update.reset();
  SourceManager::ADC_update.reset();
  print(LogLevel::LOG_DEBUG, "Motion Model stopped\n");

}

std::shared_ptr<StateSpace> MotionModel::refresh(){
  SourceManager::ADC_update.wait();
  SourceManager::PRU_update.wait();
  SourceManager::ADC_update.reset();
  SourceManager::PRU_update.reset();

  // Grab current time
  long long cur_time = microseconds();

  // Grab data
  std::shared_ptr<PRUData> pru = SourceManager::PRU.Get();
  std::shared_ptr<ADCData> adc = SourceManager::ADC.Get();

  // Measured state variable
  StateSpace meas;
  meas.x[0] = Filter::Median(pru.get()->encoder_distance, NUM_ENC_INPUTS);
  meas.x[1] = Filter::Median(pru.get()->encoder_velocity, NUM_ENC_INPUTS);
  meas.x[2] = Filter::Median(adc.get()->accel, NUM_ACCEL); 
  meas.rpm  = Filter::Median(pru.get()->disk_RPM, NUM_MOTOR_INPUTS);
  meas.fM = Filter::motor_profile(meas.x[1], meas.rpm);
  meas.fD = Filter::drag_profile( meas.x[1]);
  

  // Calculate time delta
  long long dt = cur_time - last_time;
  // Set last time
  last_time = cur_time;

  StateSpace gain;
  gain.x[0] = 1;
  gain.x[1] = 1;
  gain.x[2] = 1;
  gain.rpm = 1;
  gain.fM = 1;
  gain.fD = 1;

  // Apply Constant Gain Filter
  Filter::ConstantGainFilter(state, meas, gain, dt);
   
  // Return state
  std::shared_ptr<StateSpace> new_data = std::make_shared<StateSpace>();
  *new_data = state;

  return new_data;
}

