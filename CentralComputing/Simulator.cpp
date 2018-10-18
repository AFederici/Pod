#include "Simulator.hpp"
#include "Motor.cpp"

using namespace Utils;
//assuming Motor is in Test/Sim Mode

Simulator SimulatorManager::sim;

Simulator::Simulator() {
  //TODO set up internal variables
}
bool Simulator::sim_motor_enable(){
    motorON = 1;// currently a dummy
    return motorON;
}
bool Simulator::sim_motor_disable(){
    motorON = 0;// currently a dummy
    return motorON;
}
void Simulator::sim_motor_set_throttle(int16_t throtNeeded){
    //int16_t currThrot == throtNeeded;
    //Motor::set_throttle(currThrot); //sets the sim value to wanted value
}
bool Simulator::sim_brake_enable(){
    brakeON = 1; //turns brake on, currently a dummy
    return brakeON;
}

bool Simulator::sim_brake_disable(){
    brakeON = 0; //turns brake off, currently a dummy
    return brakeON;
}
void Simulator::sim_set_pressure( int16_t pressNeeded){
    //currPress == pressNeeded;
   // Motor::set_pressure(currPress); //sets the sim value to wanted value, but can't cuz set pressure doesn't exist yet
}
void Simulator::sim_get_position(){
 //currently empty but 
 // but in the future : TODO it should actually refer to current inputs from the accelerometer and also the encoders
 //print out each value
  //and also a combined/synthesized output that verifies/gives a more accurate output for current position.
}
bool Simulator::sim_connect(const char * hostname, const char * port) {
  //TODO connect to a Pod instance
  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int rv;
  if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error get addrinfo\n");
    return false;
  }

  if((socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error getting socket\n");
    return false;
  }

  if(connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    close(socketfd);
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error connecting\n");
    return false;
  }


  active_connection.store(true);

  read_thread = std::thread([&]() {
    read_loop();
  });

  freeaddrinfo(servinfo);
  return true;
}

bool Simulator::send_command(shared_ptr<NetworkManager::Network_Command> command) {
  int bytes_written = write(socketfd, command.get(), sizeof(NetworkManager::Network_Command));
  //print(LogLevel::LOG_EDEBUG, "Bytes written : %d, ID : %d, Value : %d\n", bytes_written, command->id, command->value);
  int size = sizeof(NetworkManager::Network_Command);
  return bytes_written == size;

}

void Simulator::read_loop() {
  while(active_connection.load()){
    // dump the data because we don't need it or do anything with it.
    // TODO if we want to we can keep the data and use it for error checking purposes
    // but that seems redundant and like a lot of work
    
    char buf[100]; 
    read(socketfd, buf, 100);

  }
  closed.invoke();

}

void Simulator::disconnect() {
  active_connection.store(false);
  close(socketfd);
  closed.wait();
  read_thread.join();
}
