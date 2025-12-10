# ElectrSwitch
Electricity Switch for automatic electricity managment

LIBRARIES:
stdint.h
stdbool.h
stddef.h

FILESYSTEM:
SRC
 ├─ main.c		//main event loop
 ├─ logic		//all func
 │   ├─ logic.c   	
 │   └─ logic.h
 └─ registers		//simulated and fake registers
     ├─ registers.c 	
     └─ registers.h

FLOW:
[------------------------------------Main]
main
reboot: 
[------------------------------------Loop1]
main event loop (infinite): 

read sensors();
compare sensors data();
emergency();
- IF data isnt compatible?
-- yes BREAK
-- no skip
time();
- IF is the time between 6:00 and 22:00? [Y/N]
-- yes set elec to ON
-- no set elec to OFF
how many days passed since "date saved"();
- IF have 14 days passed [Y/N]
-- yes substract 7.5s from clock
-- no skip
DELAY wait 20 miliseconds
[------------------------------------Loop1]
[------------------------------------Loop2]
emergency event loop (infinite):
read sensors();
compare sensors data();
- IF decide if return to the main event loop
-- yes data is normal BREAK
-- no data is irrational skip
DELAY wait 600000 miliseconds (10 minutes)
[------------------------------------Loop2]
goto reboot
[------------------------------------Main]