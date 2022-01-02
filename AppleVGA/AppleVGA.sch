EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 2
Title "Apple II VGA Card"
Date "2021-12-13"
Rev "A"
Comp ""
Comment1 "Licensed under CERN-OHL-P V2.0 (https://ohwr.org/cern_ohl_p_v2.txt)"
Comment2 "Copyright (c) 2021-2022 Mark Aikens"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 74xx:74LS07 U4
U 7 1 6183328A
P 1500 6950
F 0 "U4" H 1730 6996 50  0000 L CNN
F 1 "74LS07" H 1730 6905 50  0000 L CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 1500 6950 50  0001 C CNN
F 3 "" H 1500 6950 50  0001 C CNN
	7    1500 6950
	1    0    0    -1  
$EndComp
$Comp
L MCU_RaspberryPi_and_Boards:Pico U6
U 1 1 61835A0D
P 8300 2550
F 0 "U6" H 8300 3765 50  0000 C CNN
F 1 "Pico" H 8300 3674 50  0000 C CNN
F 2 "MCU_RaspberryPi_and_Boards:RPi_Pico_SMD_TH" V 8300 2550 50  0001 C CNN
F 3 "" H 8300 2550 50  0001 C CNN
	1    8300 2550
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 6183787E
P 2950 5000
F 0 "R2" H 3018 5046 50  0000 L CNN
F 1 "330" H 3018 4955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2990 4990 50  0001 C CNN
F 3 "~" H 2950 5000 50  0001 C CNN
	1    2950 5000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0105
U 1 1 61868BFB
P 4400 1900
F 0 "#PWR0105" H 4400 1750 50  0001 C CNN
F 1 "+3.3V" H 4415 2073 50  0000 C CNN
F 2 "" H 4400 1900 50  0001 C CNN
F 3 "" H 4400 1900 50  0001 C CNN
	1    4400 1900
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0106
U 1 1 6186956D
P 6050 1900
F 0 "#PWR0106" H 6050 1750 50  0001 C CNN
F 1 "+3.3V" H 6065 2073 50  0000 C CNN
F 2 "" H 6050 1900 50  0001 C CNN
F 3 "" H 6050 1900 50  0001 C CNN
	1    6050 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0108
U 1 1 61869A77
P 4400 3500
F 0 "#PWR0108" H 4400 3250 50  0001 C CNN
F 1 "GND" H 4405 3327 50  0000 C CNN
F 2 "" H 4400 3500 50  0001 C CNN
F 3 "" H 4400 3500 50  0001 C CNN
	1    4400 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 61869BA9
P 6050 3500
F 0 "#PWR0109" H 6050 3250 50  0001 C CNN
F 1 "GND" H 6055 3327 50  0000 C CNN
F 2 "" H 6050 3500 50  0001 C CNN
F 3 "" H 6050 3500 50  0001 C CNN
	1    6050 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0110
U 1 1 6186A6F3
P 1150 6450
F 0 "#PWR0110" H 1150 6300 50  0001 C CNN
F 1 "+5V" H 1165 6623 50  0000 C CNN
F 2 "" H 1150 6450 50  0001 C CNN
F 3 "" H 1150 6450 50  0001 C CNN
	1    1150 6450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 6186B2A1
P 1150 7450
F 0 "#PWR0111" H 1150 7200 50  0001 C CNN
F 1 "GND" H 1155 7277 50  0000 C CNN
F 2 "" H 1150 7450 50  0001 C CNN
F 3 "" H 1150 7450 50  0001 C CNN
	1    1150 7450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0112
U 1 1 6186C35E
P 9450 1600
F 0 "#PWR0112" H 9450 1450 50  0001 C CNN
F 1 "+3.3V" H 9465 1773 50  0000 C CNN
F 2 "" H 9450 1600 50  0001 C CNN
F 3 "" H 9450 1600 50  0001 C CNN
	1    9450 1600
	1    0    0    -1  
$EndComp
Text GLabel 1700 5600 0    50   Input ~ 0
PHI0
Text GLabel 1700 5150 0    50   Input ~ 0
R~W
$Comp
L 74xx:74LS07 U4
U 2 1 618CF902
P 2450 6700
F 0 "U4" H 2400 6700 50  0000 C CNN
F 1 "74LS07" H 2450 6900 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 2450 6700 50  0001 C CNN
F 3 "" H 2450 6700 50  0001 C CNN
	2    2450 6700
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS07 U4
U 3 1 618D13AB
P 2450 7100
F 0 "U4" H 2400 7100 50  0000 C CNN
F 1 "74LS07" H 2450 7300 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 2450 7100 50  0001 C CNN
F 3 "" H 2450 7100 50  0001 C CNN
	3    2450 7100
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS07 U4
U 4 1 618D2D9D
P 2100 4700
F 0 "U4" H 2050 4700 50  0000 C CNN
F 1 "74LS07" H 2100 4900 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 2100 4700 50  0001 C CNN
F 3 "" H 2100 4700 50  0001 C CNN
	4    2100 4700
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS07 U4
U 5 1 618D4FE2
P 2100 5600
F 0 "U4" H 2050 5600 50  0000 C CNN
F 1 "74LS07" H 2100 5800 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 2100 5600 50  0001 C CNN
F 3 "" H 2100 5600 50  0001 C CNN
	5    2100 5600
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS07 U4
U 6 1 618D6A69
P 2100 5150
F 0 "U4" H 2050 5150 50  0000 C CNN
F 1 "74LS07" H 2100 5350 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 2100 5150 50  0001 C CNN
F 3 "" H 2100 5150 50  0001 C CNN
	6    2100 5150
	1    0    0    -1  
$EndComp
Entry Wire Line
	1950 2300 2050 2200
Entry Wire Line
	1950 2400 2050 2300
Entry Wire Line
	1950 2500 2050 2400
Entry Wire Line
	1950 2600 2050 2500
Entry Wire Line
	1950 2700 2050 2600
Entry Wire Line
	1950 2800 2050 2700
Entry Wire Line
	1950 2900 2050 2800
Entry Wire Line
	1950 3000 2050 2900
Entry Wire Line
	3250 2300 3350 2200
Entry Wire Line
	3250 2400 3350 2300
Entry Wire Line
	3250 2500 3350 2400
Entry Wire Line
	3250 2600 3350 2500
Entry Wire Line
	3250 2700 3350 2600
Entry Wire Line
	3250 2800 3350 2700
Entry Wire Line
	3250 2900 3350 2800
Entry Wire Line
	3250 2200 3350 2100
Text Label 2050 2400 0    30   ~ 0
A13
Text Label 2050 2500 0    30   ~ 0
A12
Text Label 2050 2600 0    30   ~ 0
A11
Text Label 2050 2700 0    30   ~ 0
A10
Text Label 2050 2800 0    30   ~ 0
A9
Text Label 2050 2900 0    30   ~ 0
A8
Wire Wire Line
	2050 2200 2150 2200
Wire Wire Line
	2050 2300 2150 2300
Wire Wire Line
	2050 2400 2150 2400
Wire Wire Line
	2050 2500 2150 2500
Wire Wire Line
	2050 2600 2150 2600
Wire Wire Line
	2050 2700 2150 2700
Wire Wire Line
	2050 2800 2150 2800
Wire Wire Line
	2050 2900 2150 2900
Text Label 3250 2900 2    30   ~ 0
DA0
Text Label 3250 2800 2    30   ~ 0
DA1
Text Label 3250 2700 2    30   ~ 0
DA2
Text Label 3250 2600 2    30   ~ 0
DA3
Text Label 3250 2500 2    30   ~ 0
DA4
Text Label 3250 2400 2    30   ~ 0
DA5
Text Label 3250 2300 2    30   ~ 0
DA6
Text Label 3250 2200 2    30   ~ 0
DA7
Wire Wire Line
	3250 2200 3150 2200
Wire Wire Line
	3250 2300 3150 2300
Wire Wire Line
	3250 2400 3150 2400
Wire Wire Line
	3250 2500 3150 2500
Wire Wire Line
	3250 2600 3150 2600
Wire Wire Line
	3250 2700 3150 2700
Wire Wire Line
	3150 2800 3250 2800
Wire Wire Line
	3250 2900 3150 2900
Entry Wire Line
	3700 2300 3800 2200
Entry Wire Line
	3700 2400 3800 2300
Entry Wire Line
	3700 2500 3800 2400
Entry Wire Line
	3700 2600 3800 2500
Entry Wire Line
	3700 2700 3800 2600
Entry Wire Line
	3700 2800 3800 2700
Entry Wire Line
	3700 2900 3800 2800
Wire Wire Line
	3900 2200 3800 2200
Wire Wire Line
	3900 2300 3800 2300
Wire Wire Line
	3900 2400 3800 2400
Wire Wire Line
	3900 2500 3800 2500
Wire Wire Line
	3900 2600 3800 2600
Wire Wire Line
	3900 2700 3800 2700
Wire Wire Line
	3900 2800 3800 2800
Entry Wire Line
	3700 3000 3800 2900
Wire Wire Line
	3900 2900 3800 2900
Text Label 3800 2900 0    30   ~ 0
A0
Text Label 3800 2800 0    30   ~ 0
A1
Text Label 3800 2700 0    30   ~ 0
A2
Text Label 3800 2600 0    30   ~ 0
A3
Text Label 3800 2500 0    30   ~ 0
A4
Text Label 3800 2400 0    30   ~ 0
A5
Text Label 3800 2300 0    30   ~ 0
A6
Text Label 3800 2200 0    30   ~ 0
A7
Entry Wire Line
	5350 2300 5450 2200
Entry Wire Line
	5350 2400 5450 2300
Entry Wire Line
	5350 2500 5450 2400
Entry Wire Line
	5350 2600 5450 2500
Entry Wire Line
	5350 2700 5450 2600
Entry Wire Line
	5350 2800 5450 2700
Entry Wire Line
	5350 2900 5450 2800
Entry Wire Line
	5350 3000 5450 2900
Wire Wire Line
	5550 2200 5450 2200
Wire Wire Line
	5550 2300 5450 2300
Wire Wire Line
	5550 2400 5450 2400
Wire Wire Line
	5550 2500 5450 2500
Wire Wire Line
	5550 2600 5450 2600
Wire Wire Line
	5550 2700 5450 2700
Wire Wire Line
	5550 2800 5450 2800
Wire Wire Line
	5550 2900 5450 2900
Text Label 5450 2900 0    30   ~ 0
D0
Text Label 5450 2800 0    30   ~ 0
D1
Text Label 5450 2700 0    30   ~ 0
D2
Text Label 5450 2600 0    30   ~ 0
D3
Text Label 5450 2500 0    30   ~ 0
D4
Text Label 5450 2400 0    30   ~ 0
D5
Text Label 5450 2300 0    30   ~ 0
D6
Text Label 5450 2200 0    30   ~ 0
D7
Entry Wire Line
	5000 2300 5100 2200
Entry Wire Line
	5000 2400 5100 2300
Entry Wire Line
	5000 2500 5100 2400
Entry Wire Line
	5000 2600 5100 2500
Entry Wire Line
	5000 2700 5100 2600
Entry Wire Line
	5000 2800 5100 2700
Entry Wire Line
	5000 2900 5100 2800
Entry Wire Line
	5000 2200 5100 2100
Wire Wire Line
	5000 2200 4900 2200
Wire Wire Line
	5000 2300 4900 2300
Wire Wire Line
	5000 2400 4900 2400
Wire Wire Line
	5000 2500 4900 2500
Wire Wire Line
	5000 2600 4900 2600
Wire Wire Line
	5000 2700 4900 2700
Wire Wire Line
	5000 2800 4900 2800
Wire Wire Line
	5000 2900 4900 2900
Text Label 5000 2900 2    30   ~ 0
DA0
Text Label 5000 2800 2    30   ~ 0
DA1
Text Label 5000 2700 2    30   ~ 0
DA2
Text Label 5000 2600 2    30   ~ 0
DA3
Text Label 5000 2500 2    30   ~ 0
DA4
Text Label 5000 2400 2    30   ~ 0
DA5
Text Label 5000 2300 2    30   ~ 0
DA6
Text Label 5000 2200 2    30   ~ 0
DA7
Entry Wire Line
	6650 2400 6750 2300
Entry Wire Line
	6650 2500 6750 2400
Entry Wire Line
	6650 2700 6750 2600
Entry Wire Line
	6650 2800 6750 2700
Entry Wire Line
	6650 2900 6750 2800
Wire Wire Line
	6650 2200 6550 2200
Wire Wire Line
	6650 2300 6550 2300
Wire Wire Line
	6650 2400 6550 2400
Wire Wire Line
	6650 2500 6550 2500
Wire Wire Line
	6650 2600 6550 2600
Wire Wire Line
	6650 2700 6550 2700
Wire Wire Line
	6650 2800 6550 2800
Wire Wire Line
	6550 2900 6650 2900
Entry Wire Line
	6650 2200 6750 2100
Text Label 6650 2900 2    30   ~ 0
DA0
Text Label 6650 2800 2    30   ~ 0
DA1
Text Label 6650 2700 2    30   ~ 0
DA2
Text Label 6650 2600 2    30   ~ 0
DA3
Text Label 6650 2500 2    30   ~ 0
DA4
Text Label 6650 2400 2    30   ~ 0
DA5
Text Label 6650 2300 2    30   ~ 0
DA6
Text Label 6650 2200 2    30   ~ 0
DA7
$Comp
L Device:R_US R1
U 1 1 619CC16A
P 2650 4550
F 0 "R1" H 2718 4596 50  0000 L CNN
F 1 "330" H 2718 4505 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2690 4540 50  0001 C CNN
F 3 "~" H 2650 4550 50  0001 C CNN
	1    2650 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0117
U 1 1 619D3827
P 2950 4400
F 0 "#PWR0117" H 2950 4250 50  0001 C CNN
F 1 "+3.3V" H 2965 4573 50  0000 C CNN
F 2 "" H 2950 4400 50  0001 C CNN
F 3 "" H 2950 4400 50  0001 C CNN
	1    2950 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 6700 2150 7100
Connection ~ 2150 7100
NoConn ~ 2750 6700
NoConn ~ 2750 7100
$Comp
L power:GND #PWR0119
U 1 1 61A22B6C
P 7550 4100
F 0 "#PWR0119" H 7550 3850 50  0001 C CNN
F 1 "GND" H 7555 3927 50  0000 C CNN
F 2 "" H 7550 4100 50  0001 C CNN
F 3 "" H 7550 4100 50  0001 C CNN
	1    7550 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 3300 7600 3300
Wire Wire Line
	7550 3300 7550 2800
Wire Wire Line
	7550 2800 7600 2800
Connection ~ 7550 3300
Wire Wire Line
	7550 2800 7550 2300
Wire Wire Line
	7550 2300 7600 2300
Connection ~ 7550 2800
Wire Wire Line
	7550 2300 7550 1800
Wire Wire Line
	7550 1800 7600 1800
Connection ~ 7550 2300
Wire Wire Line
	9050 3300 9000 3300
Wire Wire Line
	9050 3300 9050 2800
Wire Wire Line
	9050 2800 9000 2800
Connection ~ 9050 3300
Wire Wire Line
	9050 2800 9050 2300
Wire Wire Line
	9050 2300 9000 2300
Connection ~ 9050 2800
Wire Wire Line
	9050 2300 9050 1800
Wire Wire Line
	9050 1800 9000 1800
Connection ~ 9050 2300
$Comp
L Device:C C6
U 1 1 61A8B355
P 850 6900
F 0 "C6" H 965 6946 50  0000 L CNN
F 1 "0.1uF" H 965 6855 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 888 6750 50  0001 C CNN
F 3 "~" H 850 6900 50  0001 C CNN
	1    850  6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  6750 850  6450
Wire Wire Line
	850  6450 1150 6450
Wire Wire Line
	850  7050 850  7450
Wire Wire Line
	850  7450 1150 7450
Connection ~ 1150 7450
Wire Wire Line
	1150 7450 1500 7450
Connection ~ 1150 6450
Wire Wire Line
	1150 6450 1500 6450
Wire Wire Line
	1500 7450 2150 7450
Connection ~ 1500 7450
Wire Wire Line
	2150 7100 2150 7450
Entry Wire Line
	7300 1600 7200 1500
Entry Wire Line
	7300 1700 7200 1600
Entry Wire Line
	7300 1900 7200 1800
Entry Wire Line
	7300 2000 7200 1900
Entry Wire Line
	7300 2100 7200 2000
Entry Wire Line
	7300 2400 7200 2300
Entry Wire Line
	7300 2200 7200 2100
Entry Wire Line
	7300 2500 7200 2400
Wire Wire Line
	7300 1600 7600 1600
Wire Wire Line
	7300 1700 7600 1700
Wire Wire Line
	7300 1900 7600 1900
Wire Wire Line
	7600 2000 7300 2000
Wire Wire Line
	7300 2100 7600 2100
Wire Wire Line
	7300 2200 7600 2200
Wire Wire Line
	7300 2400 7600 2400
Wire Wire Line
	7300 2500 7600 2500
Text Label 7300 1600 0    30   ~ 0
DA0
Text Label 7300 1700 0    30   ~ 0
DA1
Text Label 7300 1900 0    30   ~ 0
DA2
Text Label 7300 2000 0    30   ~ 0
DA3
Text Label 7300 2100 0    30   ~ 0
DA4
Text Label 7300 2200 0    30   ~ 0
DA5
Text Label 7300 2400 0    30   ~ 0
DA6
Text Label 7300 2500 0    30   ~ 0
DA7
Wire Bus Line
	3350 1450 5100 1450
Wire Bus Line
	5100 1450 6750 1450
Connection ~ 5100 1450
Wire Bus Line
	6750 1450 7200 1450
Connection ~ 6750 1450
Text GLabel 1700 4700 0    40   Input ~ 0
~DEVSEL
$Comp
L 74xx:74LS07 U4
U 1 1 61832199
P 3350 7100
F 0 "U4" H 3300 7100 50  0000 C CNN
F 1 "74LS07" H 3350 7300 50  0000 C CNN
F 2 "Package_DIP:DIP-14_W7.62mm_Socket" H 3350 7100 50  0001 C CNN
F 3 "" H 3350 7100 50  0001 C CNN
	1    3350 7100
	1    0    0    -1  
$EndComp
NoConn ~ 9000 2000
NoConn ~ 9000 1900
NoConn ~ 9000 1600
NoConn ~ 9000 2100
Wire Wire Line
	7550 3750 8300 3750
Wire Wire Line
	7550 3750 7550 3300
Wire Wire Line
	9050 3750 9050 3300
Connection ~ 8300 3750
Wire Wire Line
	8300 3750 9050 3750
NoConn ~ 8200 3700
NoConn ~ 8400 3700
$Comp
L Device:R_US R11
U 1 1 61A0E583
P 9700 3850
F 0 "R11" V 9750 3950 50  0000 L CNN
F 1 "1K" V 9750 3650 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 3840 50  0001 C CNN
F 3 "~" H 9700 3850 50  0001 C CNN
	1    9700 3850
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R8
U 1 1 61A30150
P 9700 3300
F 0 "R8" V 9750 3400 50  0000 L CNN
F 1 "1K" V 9750 3100 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 3290 50  0001 C CNN
F 3 "~" H 9700 3300 50  0001 C CNN
	1    9700 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9550 3200 9350 3200
$Comp
L Device:R_US R5
U 1 1 61A52471
P 9700 2900
F 0 "R5" V 9750 3000 50  0000 L CNN
F 1 "1K" V 9750 2700 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 2890 50  0001 C CNN
F 3 "~" H 9700 2900 50  0001 C CNN
	1    9700 2900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9950 2900 9850 2900
Wire Wire Line
	9850 3000 9950 3000
Wire Wire Line
	9950 3000 9950 2900
Wire Wire Line
	9850 3200 9950 3200
Wire Wire Line
	9950 3200 9950 3300
Wire Wire Line
	9950 3300 9850 3300
Wire Wire Line
	9850 3400 9950 3400
Wire Wire Line
	9950 3400 9950 3300
Connection ~ 9950 3300
Wire Wire Line
	9500 3750 9550 3750
Wire Wire Line
	9850 3750 9950 3750
Wire Wire Line
	9950 3750 9950 3850
Wire Wire Line
	9950 3850 9850 3850
Wire Wire Line
	9850 3950 9950 3950
Wire Wire Line
	9950 3950 9950 3850
Connection ~ 9950 3850
Wire Wire Line
	9850 2800 9950 2800
Wire Wire Line
	9950 2800 9950 2900
Connection ~ 9950 2900
Wire Wire Line
	2150 7450 3050 7450
Wire Wire Line
	3050 7450 3050 7100
Connection ~ 2150 7450
NoConn ~ 3650 7100
Entry Wire Line
	6650 2300 6750 2200
Text GLabel 10050 2900 2    50   Output ~ 0
VGA_R
Text GLabel 10050 3300 2    50   Output ~ 0
VGA_G
Text GLabel 10050 3850 2    50   Output ~ 0
VGA_B
Text GLabel 9850 2200 2    50   Output ~ 0
VGA_HSYNC
Text GLabel 9850 2400 2    50   Output ~ 0
VGA_VSYNC
$Comp
L Device:R_US R14
U 1 1 61B46B5A
P 9550 2400
F 0 "R14" V 9600 2500 50  0000 L CNN
F 1 "47" V 9600 2200 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9590 2390 50  0001 C CNN
F 3 "~" H 9550 2400 50  0001 C CNN
	1    9550 2400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R13
U 1 1 61B546DE
P 9550 2200
F 0 "R13" V 9600 2300 50  0000 L CNN
F 1 "47" V 9600 2000 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9590 2190 50  0001 C CNN
F 3 "~" H 9550 2200 50  0001 C CNN
	1    9550 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10050 2900 9950 2900
Wire Wire Line
	10050 3300 9950 3300
Wire Wire Line
	10050 3850 9950 3850
Wire Wire Line
	9400 2400 9000 2400
Wire Wire Line
	9400 2200 9000 2200
Wire Wire Line
	9850 2200 9700 2200
Wire Wire Line
	9850 2400 9700 2400
Wire Wire Line
	9000 3000 9550 3000
Wire Wire Line
	9000 2900 9550 2900
Wire Wire Line
	9350 2700 9000 2700
Wire Wire Line
	9350 2800 9550 2800
Wire Wire Line
	9350 3200 9350 3100
Wire Wire Line
	9350 3100 9000 3100
Wire Wire Line
	9250 3300 9250 3200
Wire Wire Line
	9250 3200 9000 3200
Wire Wire Line
	9250 3300 9550 3300
Wire Wire Line
	9000 3400 9550 3400
Wire Wire Line
	9500 3500 9000 3500
Wire Wire Line
	9500 3500 9500 3750
Wire Wire Line
	7500 3850 7500 3500
Wire Wire Line
	7500 3500 7600 3500
Wire Wire Line
	7500 3850 9550 3850
Wire Wire Line
	7400 3950 7400 3400
Text GLabel 10050 2600 2    50   Input ~ 0
RUN
Wire Wire Line
	9150 2500 9000 2500
Wire Wire Line
	3150 3100 3200 3100
Wire Wire Line
	3200 3100 3200 3500
Wire Wire Line
	3200 3500 2650 3500
Connection ~ 2650 3500
Wire Wire Line
	4900 3100 4950 3100
Wire Wire Line
	4950 3100 4950 3500
Wire Wire Line
	4950 3500 4400 3500
Connection ~ 4400 3500
Wire Wire Line
	3150 3200 3150 3850
Wire Wire Line
	4900 3200 4900 3750
$Comp
L 74xx:74LS245 U3
U 1 1 61835873
P 6050 2700
F 0 "U3" H 6050 2500 50  0000 C CNN
F 1 "74LVC245" H 6050 2400 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm_Socket" H 6050 2700 50  0001 C CNN
F 3 "" H 6050 2700 50  0001 C CNN
	1    6050 2700
	-1   0    0    -1  
$EndComp
$Comp
L 74xx:74LS245 U2
U 1 1 6183524A
P 4400 2700
F 0 "U2" H 4400 2500 50  0000 C CNN
F 1 "74LVC245" H 4400 2400 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm_Socket" H 4400 2700 50  0001 C CNN
F 3 "" H 4400 2700 50  0001 C CNN
	1    4400 2700
	-1   0    0    -1  
$EndComp
Text Label 2050 2300 0    30   ~ 0
A14
Text Label 2050 2200 0    30   ~ 0
A15
$Comp
L power:GND #PWR0107
U 1 1 618697C0
P 2650 3500
F 0 "#PWR0107" H 2650 3250 50  0001 C CNN
F 1 "GND" H 2655 3327 50  0000 C CNN
F 2 "" H 2650 3500 50  0001 C CNN
F 3 "" H 2650 3500 50  0001 C CNN
	1    2650 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0104
U 1 1 61865500
P 2650 1900
F 0 "#PWR0104" H 2650 1750 50  0001 C CNN
F 1 "+3.3V" H 2665 2073 50  0000 C CNN
F 2 "" H 2650 1900 50  0001 C CNN
F 3 "" H 2650 1900 50  0001 C CNN
	1    2650 1900
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74LS245 U1
U 1 1 61830C5A
P 2650 2700
F 0 "U1" H 2650 2500 50  0000 C CNN
F 1 "74LVC245" H 2650 2400 50  0000 C CNN
F 2 "Package_DIP:DIP-20_W7.62mm_Socket" H 2650 2700 50  0001 C CNN
F 3 "" H 2650 2700 50  0001 C CNN
	1    2650 2700
	-1   0    0    -1  
$EndComp
Entry Wire Line
	6650 2600 6750 2500
Wire Wire Line
	6800 2900 7600 2900
Wire Wire Line
	6550 3100 6800 3100
Wire Wire Line
	7100 3850 7100 3200
Wire Wire Line
	7100 3200 7600 3200
Wire Wire Line
	3150 3850 3450 3850
$Comp
L Device:D_Schottky D1
U 1 1 618C6FC7
P 9250 1700
F 0 "D1" H 9250 1825 50  0000 C CNN
F 1 "D_Schottky" H 9250 1826 50  0001 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 9250 1700 50  0001 C CNN
F 3 "~" H 9250 1700 50  0001 C CNN
	1    9250 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 1700 9000 1700
Wire Wire Line
	9450 1600 9450 1700
Wire Wire Line
	9450 1700 9400 1700
$Sheet
S 12050 500  1350 650 
U 619081B0
F0 "Connections" 50
F1 "AppleVGA_Connections.sch" 50
$EndSheet
Wire Bus Line
	1950 3950 3700 3950
Text GLabel 1700 4100 0    50   BiDi ~ 0
D[0..7]
Text GLabel 1700 3950 0    50   Input ~ 0
A[0..15]
Wire Bus Line
	1700 3950 1950 3950
Connection ~ 1950 3950
Wire Bus Line
	1700 4100 5350 4100
$Comp
L power:GND #PWR?
U 1 1 61BA29FC
P 5250 7300
AR Path="/619081B0/61BA29FC" Ref="#PWR?"  Part="1" 
AR Path="/61BA29FC" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 5250 7050 50  0001 C CNN
F 1 "GND" H 5255 7127 50  0000 C CNN
F 2 "" H 5250 7300 50  0001 C CNN
F 3 "" H 5250 7300 50  0001 C CNN
	1    5250 7300
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61BA2A02
P 4700 6700
AR Path="/619081B0/61BA2A02" Ref="#PWR?"  Part="1" 
AR Path="/61BA2A02" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 4700 6550 50  0001 C CNN
F 1 "+5V" H 4715 6873 50  0000 C CNN
F 2 "" H 4700 6700 50  0001 C CNN
F 3 "" H 4700 6700 50  0001 C CNN
	1    4700 6700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61BA2A08
P 5850 6800
AR Path="/619081B0/61BA2A08" Ref="#PWR?"  Part="1" 
AR Path="/61BA2A08" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 5850 6650 50  0001 C CNN
F 1 "+3.3V" H 5865 6973 50  0000 C CNN
F 2 "" H 5850 6800 50  0001 C CNN
F 3 "" H 5850 6800 50  0001 C CNN
	1    5850 6800
	0    1    1    0   
$EndComp
$Comp
L Regulator_Linear:LF33_TO220 U?
U 1 1 61BA2A0E
P 5250 6800
AR Path="/619081B0/61BA2A0E" Ref="U?"  Part="1" 
AR Path="/61BA2A0E" Ref="U5"  Part="1" 
F 0 "U5" H 5250 7042 50  0000 C CNN
F 1 "UA78M33CKCS" H 5250 6951 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 5250 7025 50  0001 C CIN
F 3 "" H 5250 6750 50  0001 C CNN
	1    5250 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61BA2A14
P 4900 7050
AR Path="/619081B0/61BA2A14" Ref="C?"  Part="1" 
AR Path="/61BA2A14" Ref="C2"  Part="1" 
F 0 "C2" H 4786 7004 50  0000 R CNN
F 1 "0.33uF" H 4786 7095 50  0000 R CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 4938 6900 50  0001 C CNN
F 3 "~" H 4900 7050 50  0001 C CNN
	1    4900 7050
	1    0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 61BA2A1A
P 5600 7050
AR Path="/619081B0/61BA2A1A" Ref="C?"  Part="1" 
AR Path="/61BA2A1A" Ref="C4"  Part="1" 
F 0 "C4" H 5715 7096 50  0000 L CNN
F 1 "0.1uF" H 5715 7005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 5638 6900 50  0001 C CNN
F 3 "~" H 5600 7050 50  0001 C CNN
	1    5600 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 6800 4900 6800
Connection ~ 4900 6800
Wire Wire Line
	4900 6800 4700 6800
Wire Wire Line
	4700 6800 4700 6700
Wire Wire Line
	4900 6800 4900 6900
Wire Wire Line
	5550 6800 5600 6800
Wire Wire Line
	5600 6800 5600 6900
Connection ~ 5600 6800
Wire Wire Line
	5600 6800 5850 6800
Wire Wire Line
	4900 7200 5250 7200
Wire Wire Line
	5250 7200 5250 7100
Wire Wire Line
	5600 7200 5250 7200
Connection ~ 5250 7200
Wire Wire Line
	5250 7200 5250 7300
Wire Wire Line
	1800 5150 1700 5150
Wire Wire Line
	1800 5600 1700 5600
Wire Wire Line
	1800 4700 1700 4700
Wire Wire Line
	2400 4700 2650 4700
Wire Wire Line
	2400 5150 2950 5150
Wire Wire Line
	8300 3750 8300 3700
Wire Wire Line
	7550 4100 7550 3750
Connection ~ 7550 3750
Text Notes 1700 6000 0    50   ~ 0
TODO: AND together ~IOSEL~ and ~DEVSEL~ with an LVC gate\nso both can be used.  The extra gates can then replace\nthese inefficient OC buffers.
$Comp
L Device:R_US R4
U 1 1 61A5E084
P 9700 2800
F 0 "R4" V 9750 2900 50  0000 L CNN
F 1 "500" V 9750 2600 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 2790 50  0001 C CNN
F 3 "~" H 9700 2800 50  0001 C CNN
	1    9700 2800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R6
U 1 1 61A46E2A
P 9700 3000
F 0 "R6" V 9750 3100 50  0000 L CNN
F 1 "2K" V 9750 2800 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 2990 50  0001 C CNN
F 3 "~" H 9700 3000 50  0001 C CNN
	1    9700 3000
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R10
U 1 1 61A19870
P 9700 3750
F 0 "R10" V 9750 3850 50  0000 L CNN
F 1 "500" V 9750 3550 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 3740 50  0001 C CNN
F 3 "~" H 9700 3750 50  0001 C CNN
	1    9700 3750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R12
U 1 1 619C780B
P 9700 3950
F 0 "R12" V 9750 4050 50  0000 L CNN
F 1 "2K" V 9750 3750 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 3940 50  0001 C CNN
F 3 "~" H 9700 3950 50  0001 C CNN
	1    9700 3950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R7
U 1 1 61A3B76D
P 9700 3200
F 0 "R7" V 9750 3300 50  0000 L CNN
F 1 "500" V 9750 3000 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 3190 50  0001 C CNN
F 3 "~" H 9700 3200 50  0001 C CNN
	1    9700 3200
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R9
U 1 1 61A24CCF
P 9700 3400
F 0 "R9" V 9750 3500 50  0000 L CNN
F 1 "2K" V 9750 3200 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9740 3390 50  0001 C CNN
F 3 "~" H 9700 3400 50  0001 C CNN
	1    9700 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6800 2900 6800 3100
Wire Wire Line
	7000 3100 7600 3100
Wire Wire Line
	4900 3750 5100 3750
Wire Wire Line
	6550 3200 6900 3200
Wire Wire Line
	6900 3000 6950 3000
Wire Wire Line
	6900 3000 6900 3200
$Comp
L Device:R_US R3
U 1 1 61B4C935
P 3250 5450
F 0 "R3" H 3318 5496 50  0000 L CNN
F 1 "330" H 3318 5405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3290 5440 50  0001 C CNN
F 3 "~" H 3250 5450 50  0001 C CNN
	1    3250 5450
	1    0    0    -1  
$EndComp
Connection ~ 2650 4700
Connection ~ 2950 5150
Wire Wire Line
	2950 4400 2650 4400
Wire Wire Line
	2950 4400 2950 4850
Connection ~ 2950 4400
Wire Wire Line
	2400 5600 3250 5600
Wire Wire Line
	2950 4400 3250 4400
Wire Wire Line
	3250 4400 3250 5300
Connection ~ 3250 5600
Wire Wire Line
	7200 2600 7200 4700
Wire Wire Line
	2650 4700 7200 4700
Wire Wire Line
	2950 5150 7300 5150
Wire Wire Line
	7300 2700 7300 5150
$Comp
L Device:R_US R15
U 1 1 61AD7057
P 3450 3700
F 0 "R15" H 3518 3746 50  0000 L CNN
F 1 "10K" H 3518 3655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3490 3690 50  0001 C CNN
F 3 "~" H 3450 3700 50  0001 C CNN
	1    3450 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2600 10050 2600
Wire Wire Line
	9350 2800 9350 2700
Wire Wire Line
	9150 5600 9150 2500
Wire Wire Line
	3250 5600 9150 5600
$Comp
L Device:R_US R16
U 1 1 61B6D7A0
P 5100 3600
F 0 "R16" H 5168 3646 50  0000 L CNN
F 1 "10K" H 5168 3555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5140 3590 50  0001 C CNN
F 3 "~" H 5100 3600 50  0001 C CNN
	1    5100 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R17
U 1 1 61B79500
P 6950 2650
F 0 "R17" H 7018 2696 50  0000 L CNN
F 1 "10K" H 7018 2605 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 6990 2640 50  0001 C CNN
F 3 "~" H 6950 2650 50  0001 C CNN
	1    6950 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 2600 7600 2600
Wire Wire Line
	7300 2700 7600 2700
Wire Wire Line
	7400 3400 7600 3400
Wire Wire Line
	7400 3950 9550 3950
Wire Wire Line
	7000 3750 7000 3100
Connection ~ 3450 3850
Wire Wire Line
	3450 3850 7100 3850
Connection ~ 5100 3750
Wire Wire Line
	5100 3750 7000 3750
Wire Wire Line
	6950 2800 6950 3000
Connection ~ 6950 3000
Wire Wire Line
	6950 3000 7600 3000
$Comp
L power:+3.3V #PWR?
U 1 1 61C3A0F1
P 6950 2500
F 0 "#PWR?" H 6950 2350 50  0001 C CNN
F 1 "+3.3V" H 6965 2673 50  0000 C CNN
F 2 "" H 6950 2500 50  0001 C CNN
F 3 "" H 6950 2500 50  0001 C CNN
	1    6950 2500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61C46875
P 5100 3450
F 0 "#PWR?" H 5100 3300 50  0001 C CNN
F 1 "+3.3V" H 5115 3623 50  0000 C CNN
F 2 "" H 5100 3450 50  0001 C CNN
F 3 "" H 5100 3450 50  0001 C CNN
	1    5100 3450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61C52D87
P 3450 3550
F 0 "#PWR?" H 3450 3400 50  0001 C CNN
F 1 "+3.3V" H 3465 3723 50  0000 C CNN
F 2 "" H 3450 3550 50  0001 C CNN
F 3 "" H 3450 3550 50  0001 C CNN
	1    3450 3550
	1    0    0    -1  
$EndComp
Wire Bus Line
	6750 1450 6750 2850
Wire Bus Line
	7200 1450 7200 2450
Wire Bus Line
	5100 1450 5100 2850
Wire Bus Line
	5350 2250 5350 4100
Wire Bus Line
	3700 2250 3700 3950
Wire Bus Line
	3350 1450 3350 2850
Wire Bus Line
	1950 2250 1950 3950
$EndSCHEMATC
