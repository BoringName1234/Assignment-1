serialmake: serial.c filereader.c filereader.h
	gcc -o SerialOutput Serial.c filereader.c