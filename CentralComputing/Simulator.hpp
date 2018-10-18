#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP


#include "NetworkManager.hpp"
#include "Pod_State.h"
using namespace std;
class Simulator {
 
  public:
    /**
     * Creates a command simulator object
     */
    Simulator();


    /**
     * Connects to the pod server at hostname/port
     * @param hostname the hostname to connect to
     * @param port the port to connect to
     */
    bool sim_connect(const char * hostname, const char * port);

    /**
     * Sends the given command to the connected pod
     * @param command the command to send
     */
    bool send_command(shared_ptr<NetworkManager::Network_Command> command); 

    /**
     * Thread function, reads continually and updates the internal simulate state variables
     */
    void read_loop();


    /**
     * Close the active connection and free any owned variables
     */
    void disconnect();


    bool sim_motor_enable();
    bool sim_motor_disable();
    void sim_motor_set_throttle(int16_t throtNeeded);
    bool sim_brake_enable();
    bool sim_brake_disable();
    void sim_set_pressure(int16_t pressNeeded);
    void sim_get_position();
    Pod_State::E_States current_state; //current state of the pod as read by the network controller

    atomic<bool> active_connection;
    Event closed;
    thread read_thread;

    int socketfd;

    bool motorON;
    bool brakeON;
    int16_t currPress; //which is what is currently being used by the set_throttle function in Motor.cpp
    int16_t currThrot; // which is what is currently being used by the set_pressure function somewhere else

    //TODO:
    //Add brake status, motor status
};
namespace SimulatorManager {
  extern Simulator sim;
}

#endif //SIMULATOR_HPP
