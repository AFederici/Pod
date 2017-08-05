#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Battery::Battery(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	data = vector<double>(count);		
}

Battery::~Battery(){
	//do nothing
}

void Battery::reset() {
	//TODO: implement resetting
}

void Battery::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Battery::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	cout << difference << endl;
	
	//TODO calc some actual value
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}