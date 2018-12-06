#include "Pod_State.h" 
#include "Pod.h"
#include "SourceManager.hpp"

using namespace Utils;

double MAX_DECCEL = 19.6;
double IDEAL_DECCEL = 9.8;
double LENGTH_OF_TRACK = 1000;
double BUFFER_LENGTH = 20;
double MAX_VELOCITY = 550;

//Pod_State::Pod_State(Brake * brake, Motor * motor, Sensor * sensor)
Pod_State::Pod_State()
  : StateMachine(ST_MAX_STATES),
  motor(), brakes()
    
{
  transition_map[NetworkManager::TRANS_SAFE_MODE] = &Pod_State::move_safe_mode;  
  transition_map[NetworkManager::TRANS_FUNCTIONAL_TEST] = &Pod_State::move_functional_tests;
  transition_map[NetworkManager::TRANS_LOADING] = &Pod_State::move_loading;
  transition_map[NetworkManager::TRANS_LAUNCH_READY] = &Pod_State::move_launch_ready;
  transition_map[NetworkManager::LAUNCH] = &Pod_State::accelerate;
  transition_map[NetworkManager::TRANS_FLIGHT_COAST] = &Pod_State::coast;
  transition_map[NetworkManager::TRANS_FLIGHT_BRAKE] = &Pod_State::brake;
  transition_map[NetworkManager::EMERGENCY_BRAKE] = &Pod_State::emergency_brake;
  transition_map[NetworkManager::TRANS_ERROR_STATE]= &Pod_State::error;
//non state transition commands
  transition_map[NetworkManager::ENABLE_MOTOR] = &Pod_State::no_transition;
  transition_map[NetworkManager::DISABLE_MOTOR] = &Pod_State::no_transition;
  transition_map[NetworkManager::SET_MOTOR_SPEED] = &Pod_State::no_transition;
  transition_map[NetworkManager::ENABLE_BRAKE] = &Pod_State::no_transition;
  transition_map[NetworkManager::DISABLE_BRAKE] = &Pod_State::no_transition;
  steady_state_map[ST_SAFE_MODE] = &Pod_State::steady_safe_mode;
  steady_state_map[ST_FUNCTIONAL_TEST] = &Pod_State::steady_functional;
  steady_state_map[ST_LOADING] = &Pod_State::steady_loading;
  steady_state_map[ST_LAUNCH_READY] = &Pod_State::steady_launch_ready;
  steady_state_map[ST_FLIGHT_ACCEL] = &Pod_State::steady_flight_accelerate;
  steady_state_map[ST_FLIGHT_COAST] = &Pod_State::steady_flight_coast;
  steady_state_map[ST_FLIGHT_BRAKE] = &Pod_State::steady_flight_brake;
  steady_state_map[ST_ERROR] = &Pod_State::steady_error_state;
}
// returns the current state as a E_States enum
Pod_State::E_States Pod_State::get_current_state() {
	return (E_States)StateMachine::getCurrentState();
}


/**
 * User controlled movement events
**/


void Pod_State::move_functional_tests(){
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}

void Pod_State::move_safe_mode() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Loading */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}

void Pod_State::move_loading() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(ST_LOADING)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)	  		/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}
void Pod_State::move_launch_ready() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_LAUNCH_READY)		/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}

// it is important all states should move to braking when this function is called, this is for emergencies
void Pod_State::emergency_brake() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Loading */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Launch ready */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight accel */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight coast */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}

/**
 * Software controlled events
 **/

void Pod_State::accelerate() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_ACCEL)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}
void Pod_State::coast() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_COAST)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}

void Pod_State::brake() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)
}

void Pod_State::error(){
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Loading */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Launch ready */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Flight accel */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Flight coast */
		TRANSITION_MAP_ENTRY(ST_ERROR)			/* Flight brake */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			//Error State
	END_TRANSITION_MAP(NULL)	

}

void Pod_State::no_transition() {
  // do nothing
}

void Pod_State::ST_Safe_Mode() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}

void Pod_State::ST_Functional_Test() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}
void Pod_State::ST_Loading() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}
void Pod_State::ST_Launch_Ready() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
}

void Pod_State::ST_Flight_Accel() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  brakes.disable_brakes();
  motor.enable_motors();
}

void Pod_State::ST_Flight_Coast() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  motor.disable_motors();
}

void Pod_State::ST_Flight_Brake() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  motor.disable_motors();
  brakes.enable_brakes();
}
void Pod_State::ST_Error() {
  print(LogLevel::LOG_EDEBUG, "STATE : %s\n", get_current_state_string().c_str());
  motor.disable_motors();
	//enable brakes;
}

/////////////////////////////
// STEADY STATE FUNCTIONS //
///////////////////////////
void Pod_State::steady_safe_mode(std::shared_ptr<NetworkManager::Network_Command> command) {
  //not much special stuff to do here  
}

void Pod_State::steady_functional(std::shared_ptr<NetworkManager::Network_Command> command) {
  //process command, let manual commands go through
	switch (command->id) {
		case NetworkManager::ENABLE_MOTOR: 
      motor.enable_motors();
			break;
		case NetworkManager::DISABLE_MOTOR:
      motor.disable_motors();
			break;
		case NetworkManager::SET_MOTOR_SPEED:
      motor.set_throttle(command->value * 4); //value is 0-250, set_throttle expects 0-1000
			break;
  	case NetworkManager::ENABLE_BRAKE:
      //activate brakes
      brakes.enable_brakes();
			break;
		case NetworkManager::DISABLE_BRAKE:
      //deactivate brakes
      brakes.disable_brakes();
			break;
    default:
      break;
	}
}

void Pod_State::steady_loading(std::shared_ptr<NetworkManager::Network_Command> command) {

}

void Pod_State::steady_launch_ready(std::shared_ptr<NetworkManager::Network_Command> command) {

}

void Pod_State::steady_flight_accelerate(std::shared_ptr<NetworkManager::Network_Command> command) {
	// Access Pos, Vel, and Accel from Motion Model
	std::shared_ptr<StateSpace> state = SourceManager::MM.Get();
	double pos = state->x[0];
	double vel = state->x[1];
	double acc = state->x[2];
	
	if (shouldBrake(vel, pos)) {
		
		auto newCommand = std::make_shared<NetworkManager::Network_Command>();
		newCommand->id = NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST;
		newCommand->value = 0;
		NetworkManager::command_queue.enqueue(newCommand);
	}

	if (vel > MAX_VELOCITY) {
		// Switch to coast
		auto newCommand = std::make_shared<NetworkManager::Network_Command>();
		newCommand->id = NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST;
		newCommand->value = 0;
		NetworkManager::command_queue.enqueue(newCommand);
	}
}

void Pod_State::steady_flight_coast(std::shared_ptr<NetworkManager::Network_Command> command) {
	std::shared_ptr<StateSpace> state = SourceManager::MM.Get();
	double pos = state->x[0];
	double vel = state->x[1];
	double acc = state->x[2];
	
	if (shouldBrake(vel, pos)) {
		auto newCommand = std::make_shared<NetworkManager::Network_Command>();
		newCommand->id = NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST;
		newCommand->value = 0;
		NetworkManager::command_queue.enqueue(newCommand);
	}
}

void Pod_State::steady_flight_brake(std::shared_ptr<NetworkManager::Network_Command> command) {
	// Brakes are applied
}

bool Pod_State::shouldBrake(double vel, double pos) {
	double stoppingDistance = -0.5*vel*vel/IDEAL_DECCEL;
	double remainingDistance = LENGTH_OF_TRACK - BUFFER_LENGTH - pos;

	if (stoppingDistance >= remainingDistance) {
		return true;
	} else {
		return false;
	}
}

void Pod_State::steady_error_state(std::shared_ptr<NetworkManager::Network_Command> command){
	}
