//Main event loop
int main(void) {
	
	return_normal_operations:

	//Normal event loop
	while (1) {
		read_sensors()
		cmp_sensor_data();
		emerg_override();
		time_update();
		if (emergency_flag) { break; }
		delay(20);
	}

//=================================================================================//

	//Emergency event loop
	while(1) {

		//Perform emergency operations
		read_sensors();
		cmp_sensor_data();
		if (!emergency_flag) { break; }
		delay(600000);
	}
	//restart system
	goto return_normal_operations;
}
