//Main event loop
int main(void) {

	while (1) {
		read_sensors()
		cmp_sensor_data();
		time_update();
		if (emergency_flag) { electricity_flow(); }
		else (!emergency_flag) { electricity_flow(); }
		delay(20);
	}
}
