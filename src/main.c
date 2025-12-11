//Main event loop
int main(void) {

	while (1) {
		read_sensors(&voltage, &temperature);
		cmp_sensor_data();
		time_update();
		electricity_flow();
		delay(20);
	}
}
