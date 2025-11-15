serialmake: serial.c file_reader.c file_reader.h
	gcc -o SerialOutput Serial.c file_reader.c -O3