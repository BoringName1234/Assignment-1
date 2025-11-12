serialmake: serial.c file_reader.c
	gcc -o SerialOutput Serial.c file_reader.c -O3 -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info -fopt-info-missed

versionmake: serial.c file_reader.c
	gcc -o SerialOutput_Version2 Serial_Version_2.c file_reader.c -O3 -fopt-info-vec-optimized -fopt-info-vec-missed -fopt-info -fopt-info-missed