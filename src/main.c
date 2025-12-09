//Main event loop
int main(void) {

	return_normal_operations:
	//Normal event loop
	while (1) {

		update_time();
		delay(20);
	}

//=================================================================================//

	//Emergency event loop
	while(1) {

		//Perform emergency operations

		delay(600000);
	}
	//restart system
	goto return_normal_operations;
}
