EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "Branch Controller Prototype"
Date ""
Rev ""
Comp "FutureTurtles"
Comment1 "Joel Spolsky"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L teensy:Teensy3.2 U1
U 1 1 5DFF9AF9
P 8450 2700
F 0 "U1" H 8450 4337 60  0000 C CNN
F 1 "Teensy3.2" H 8450 4231 60  0000 C CNN
F 2 "teensy:Teensy30_31_32_LC" H 8450 4231 60  0001 C CNN
F 3 "" H 8450 1950 60  0000 C CNN
	1    8450 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 5E006714
P 9300 4750
F 0 "R1" V 9505 4750 50  0000 C CNN
F 1 "220 Ω" V 9414 4750 50  0000 C CNN
F 2 "" V 9340 4740 50  0001 C CNN
F 3 "~" H 9300 4750 50  0001 C CNN
	1    9300 4750
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E01148E
P 6650 1350
F 0 "#PWR?" H 6650 1100 50  0001 C CNN
F 1 "GND" H 6655 1177 50  0000 C CNN
F 2 "" H 6650 1350 50  0001 C CNN
F 3 "" H 6650 1350 50  0001 C CNN
	1    6650 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 1350 6650 1350
$Comp
L Device:LED D1
U 1 1 5E012C94
P 8750 4750
F 0 "D1" H 8743 4966 50  0000 C CNN
F 1 "Grn Status LED" H 8743 4875 50  0000 C CNN
F 2 "" H 8750 4750 50  0001 C CNN
F 3 "~" H 8750 4750 50  0001 C CNN
	1    8750 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 4750 8900 4750
$Comp
L power:GND #PWR?
U 1 1 5E0150B5
P 8450 4750
F 0 "#PWR?" H 8450 4500 50  0001 C CNN
F 1 "GND" H 8455 4577 50  0000 C CNN
F 2 "" H 8450 4750 50  0001 C CNN
F 3 "" H 8450 4750 50  0001 C CNN
	1    8450 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 4750 8600 4750
Wire Wire Line
	9450 4750 9700 4750
Wire Wire Line
	9700 4750 9700 3950
Wire Wire Line
	9700 3950 9450 3950
$Comp
L 74xx:74HC245 U2
U 1 1 5DFF9A6C
P 1800 2000
F 0 "U2" H 1800 2981 50  0000 C CNN
F 1 "SN74HCT245N Level Shifter" H 1800 2890 50  0000 C CNN
F 2 "" H 1800 2000 50  0001 C CNN
F 3 "http://www.ti.com/lit/gpn/sn74hct245" H 1800 2000 50  0001 C CNN
	1    1800 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5DFFD29C
P 9900 3850
F 0 "#PWR?" H 9900 3700 50  0001 C CNN
F 1 "+3.3V" H 9915 4023 50  0000 C CNN
F 2 "" H 9900 3850 50  0001 C CNN
F 3 "" H 9900 3850 50  0001 C CNN
	1    9900 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 3850 9450 3850
$Comp
L power:+3.3V #PWR?
U 1 1 5DFFEC2F
P 2500 1200
F 0 "#PWR?" H 2500 1050 50  0001 C CNN
F 1 "+3.3V" H 2515 1373 50  0000 C CNN
F 2 "" H 2500 1200 50  0001 C CNN
F 3 "" H 2500 1200 50  0001 C CNN
	1    2500 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5DFFF459
P 650 2400
F 0 "#PWR?" H 650 2250 50  0001 C CNN
F 1 "+3.3V" H 665 2573 50  0000 C CNN
F 2 "" H 650 2400 50  0001 C CNN
F 3 "" H 650 2400 50  0001 C CNN
	1    650  2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5DFFFC77
P 1800 2800
F 0 "#PWR?" H 1800 2550 50  0001 C CNN
F 1 "GND" H 1805 2627 50  0000 C CNN
F 2 "" H 1800 2800 50  0001 C CNN
F 3 "" H 1800 2800 50  0001 C CNN
	1    1800 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2400 650  2400
Wire Wire Line
	1800 1200 2500 1200
$Comp
L power:GND #PWR?
U 1 1 5E000ED7
P 1300 2800
F 0 "#PWR?" H 1300 2550 50  0001 C CNN
F 1 "GND" H 1305 2627 50  0000 C CNN
F 2 "" H 1300 2800 50  0001 C CNN
F 3 "" H 1300 2800 50  0001 C CNN
	1    1300 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2800 1300 2500
Text Label 7050 1650 0    50   ~ 0
STR1
Text Label 7050 1950 0    50   ~ 0
STR8
Text Label 7050 2050 0    50   ~ 0
STR5
Text Label 7050 2150 0    50   ~ 0
STR3
Text Label 7050 2250 0    50   ~ 0
STR4
Text Label 7050 3350 0    50   ~ 0
STR2
Text Label 7050 3950 0    50   ~ 0
STR6
Text Label 7050 4050 0    50   ~ 0
STR7
Wire Wire Line
	7450 1650 7050 1650
Wire Wire Line
	7450 1950 7050 1950
Wire Wire Line
	7450 2050 7050 2050
Wire Wire Line
	7450 2150 7050 2150
Wire Wire Line
	7450 2250 7050 2250
Wire Wire Line
	7450 3350 7050 3350
Wire Wire Line
	7450 3950 7050 3950
Wire Wire Line
	7450 4050 7050 4050
Text Label 850  1500 0    50   ~ 0
STR1
Text Label 850  1600 0    50   ~ 0
STR2
Text Label 850  1700 0    50   ~ 0
STR3
Text Label 850  1800 0    50   ~ 0
STR4
Text Label 850  1900 0    50   ~ 0
STR5
Text Label 850  2000 0    50   ~ 0
STR6
Text Label 850  2100 0    50   ~ 0
STR7
Text Label 850  2200 0    50   ~ 0
STR8
Wire Wire Line
	1300 1500 850  1500
Wire Wire Line
	1300 1600 850  1600
Wire Wire Line
	1300 1700 850  1700
Wire Wire Line
	1300 1800 850  1800
Wire Wire Line
	1300 1900 850  1900
Wire Wire Line
	1300 2000 850  2000
Wire Wire Line
	1300 2100 850  2100
Wire Wire Line
	1300 2200 850  2200
$Comp
L Device:R_Pack08 RN1
U 1 1 5E00BC14
P 3050 1900
F 0 "RN1" V 2433 1900 50  0000 C CNN
F 1 "4116R-1-101LF-ND 100Ω RES ARRAY" V 2524 1900 50  0000 C CNN
F 2 "" V 3525 1900 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/4100R.pdf" H 3050 1900 50  0001 C CNN
	1    3050 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 1500 2850 1500
Wire Wire Line
	2300 1600 2850 1600
Wire Wire Line
	2300 1700 2850 1700
Wire Wire Line
	2300 1800 2850 1800
Wire Wire Line
	2300 1900 2850 1900
Wire Wire Line
	2300 2000 2850 2000
Wire Wire Line
	2300 2100 2850 2100
Wire Wire Line
	2300 2200 2850 2200
$Comp
L Connector:RJ45 J1
U 1 1 5E015D10
P 4750 1900
F 0 "J1" H 4420 1904 50  0000 R CNN
F 1 "RJ45" H 4420 1995 50  0000 R CNN
F 2 "" V 4750 1925 50  0001 C CNN
F 3 "~" V 4750 1925 50  0001 C CNN
	1    4750 1900
	-1   0    0    1   
$EndComp
$Comp
L Connector:RJ45 J2
U 1 1 5E0167E8
P 4750 2950
F 0 "J2" H 4420 2954 50  0000 R CNN
F 1 "RJ45" H 4420 3045 50  0000 R CNN
F 2 "" V 4750 2975 50  0001 C CNN
F 3 "~" V 4750 2975 50  0001 C CNN
	1    4750 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	3250 1500 3900 1500
Wire Wire Line
	3900 1500 3900 1700
Wire Wire Line
	3900 1700 4350 1700
Wire Wire Line
	3250 1600 3850 1600
Wire Wire Line
	3850 1600 3850 1900
Wire Wire Line
	3850 1900 4350 1900
Wire Wire Line
	3250 1700 3800 1700
Wire Wire Line
	3800 1700 3800 2100
Wire Wire Line
	3800 2100 4350 2100
Wire Wire Line
	3250 1800 3750 1800
Wire Wire Line
	3750 1800 3750 2300
Wire Wire Line
	3750 2300 4350 2300
Wire Wire Line
	3250 1900 3700 1900
Wire Wire Line
	3700 1900 3700 2750
Wire Wire Line
	3700 2750 4350 2750
Wire Wire Line
	3250 2000 3650 2000
Wire Wire Line
	3650 2000 3650 2950
Wire Wire Line
	3650 2950 4350 2950
Wire Wire Line
	3250 2100 3600 2100
Wire Wire Line
	3600 2100 3600 3150
Wire Wire Line
	3600 3150 4350 3150
Wire Wire Line
	3250 2200 3550 2200
Wire Wire Line
	3550 2200 3550 3350
Wire Wire Line
	3550 3350 4350 3350
$Comp
L power:GND #PWR?
U 1 1 5E035EDD
P 4200 3500
F 0 "#PWR?" H 4200 3250 50  0001 C CNN
F 1 "GND" H 4205 3327 50  0000 C CNN
F 2 "" H 4200 3500 50  0001 C CNN
F 3 "" H 4200 3500 50  0001 C CNN
	1    4200 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 3500 4200 3250
Wire Wire Line
	4200 1600 4350 1600
Wire Wire Line
	4200 1800 4350 1800
Connection ~ 4200 1800
Wire Wire Line
	4200 1800 4200 1600
Wire Wire Line
	4200 2000 4350 2000
Connection ~ 4200 2000
Wire Wire Line
	4200 2000 4200 1800
Wire Wire Line
	4200 2200 4350 2200
Connection ~ 4200 2200
Wire Wire Line
	4200 2200 4200 2000
Wire Wire Line
	4200 2650 4350 2650
Connection ~ 4200 2650
Wire Wire Line
	4200 2650 4200 2200
Wire Wire Line
	4200 2850 4350 2850
Connection ~ 4200 2850
Wire Wire Line
	4200 2850 4200 2650
Wire Wire Line
	4200 3050 4350 3050
Connection ~ 4200 3050
Wire Wire Line
	4200 3050 4200 2850
Wire Wire Line
	4200 3250 4350 3250
Connection ~ 4200 3250
Wire Wire Line
	4200 3250 4200 3050
$EndSCHEMATC
