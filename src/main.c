//Main event loop
int main(void) {
	
	return_normal_operations:
	//Normal event loop
	while (1) {
		read_sensors(void)
		cmp_sensor_data(void);
		emerg_override(void);
		time_update(void);
		delay(20);
	}

//=================================================================================//

	//Emergency event loop
	while(1) {

		//Perform emergency operations
		read_sensors(void);
		cmp_sensor_data(void);
		delay(600000);
	}
	//restart system
	goto return_normal_operations;
}
