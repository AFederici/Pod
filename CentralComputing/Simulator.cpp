#include "Simulator.hpp"

using namespace Utils;
Simulator::Simulator(const char * logfile) : logpath(logfile) {
  //TODO set up internal variables
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
