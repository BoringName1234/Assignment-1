serialmake: serial.c file_reader.c
	gcc -o SerialOutput Serial.c file_reader.c -O3 -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info -fopt-info-missed

versionmake: Serial_Version_2.c file_reader.c
	gcc -o SerialOutput_Version2 Serial_Version_2.c file_reader.c -O3 

testmake: testSerial.c file_reader.c
	gcc -o TestOutput testSerial.c file_reader.c -O3 

testmake2: testSerial_Version2.c file_reader.c
	gcc -o TestOutputV2 testSerial_Version2.c file_reader.c -O3 

speed: SerialVersion3.c file_reader.c
	gcc -o SpeedTest SerialVersion3.c file_reader.c -O3 