//Main event loop
int main(void) {

	return_normal_operations:
	//Normal event loop
	while (1) {

		//Perform normal operations

		delay(20);
	}
	//Emergency event loop
	while(1) {

		//Perform emergency operations

		delay(600000);
	}
	//restart system
	goto return_normal_operations;
}
