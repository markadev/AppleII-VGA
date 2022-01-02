EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L Device:C C1
U 1 1 618FB312
P 1100 6700
F 0 "C1" H 1215 6746 50  0000 L CNN
F 1 "0.1uF" H 1215 6655 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 1138 6550 50  0001 C CNN
F 3 "~" H 1100 6700 50  0001 C CNN
	1    1100 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 618FB318
P 1550 6700
F 0 "C3" H 1665 6746 50  0000 L CNN
F 1 "0.1uF" H 1665 6655 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 1588 6550 50  0001 C CNN
F 3 "~" H 1550 6700 50  0001 C CNN
	1    1550 6700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 618FB31E
P 2000 6700
F 0 "C5" H 2115 6746 50  0000 L CNN
F 1 "0.1uF" H 2115 6655 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.3mm_W1.9mm_P5.00mm" H 2038 6550 50  0001 C CNN
F 3 "~" H 2000 6700 50  0001 C CNN
	1    2000 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 6550 1550 6550
Wire Wire Line
	1550 6550 2000 6550
Connection ~ 1550 6550
Wire Wire Line
	2000 6850 1550 6850
Wire Wire Line
	1550 6850 1100 6850
Connection ~ 1550 6850
$Comp
L power:+3.3V #PWR0115
U 1 1 618FB32A
P 1550 6550
F 0 "#PWR0115" H 1550 6400 50  0001 C CNN
F 1 "+3.3V" H 1565 6723 50  0000 C CNN
F 2 "" H 1550 6550 50  0001 C CNN
F 3 "" H 1550 6550 50  0001 C CNN
	1    1550 6550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0116
U 1 1 618FB330
P 1550 6850
F 0 "#PWR0116" H 1550 6600 50  0001 C CNN
F 1 "GND" H 1555 6677 50  0000 C CNN
F 2 "" H 1550 6850 50  0001 C CNN
F 3 "" H 1550 6850 50  0001 C CNN
	1    1550 6850
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 61914AC4
P 9600 1900
F 0 "SW1" H 9600 2185 50  0000 C CNN
F 1 "SW_Push" H 9600 2094 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 9600 2100 50  0001 C CNN
F 3 "~" H 9600 2100 50  0001 C CNN
	1    9600 1900
	1    0    0    -1  
$EndComp
Text GLabel 9400 1900 0    50   Output ~ 0
RUN
$Comp
L power:GND #PWR0118
U 1 1 61914ACB
P 9800 1900
F 0 "#PWR0118" H 9800 1650 50  0001 C CNN
F 1 "GND" H 9805 1727 50  0000 C CNN
F 2 "" H 9800 1900 50  0001 C CNN
F 3 "" H 9800 1900 50  0001 C CNN
	1    9800 1900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 6194823B
P 6150 3500
AR Path="/6194823B" Ref="J?"  Part="1" 
AR Path="/619081B0/6194823B" Ref="J2"  Part="1" 
F 0 "J2" H 6230 3492 50  0000 L CNN
F 1 "VGA" H 6230 3401 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 6150 3500 50  0001 C CNN
F 3 "~" H 6150 3500 50  0001 C CNN
	1    6150 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61948241
P 5900 4100
AR Path="/61948241" Ref="#PWR?"  Part="1" 
AR Path="/619081B0/61948241" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 5900 3850 50  0001 C CNN
F 1 "GND" H 5905 3927 50  0000 C CNN
F 2 "" H 5900 4100 50  0001 C CNN
F 3 "" H 5900 4100 50  0001 C CNN
	1    5900 4100
	1    0    0    -1  
$EndComp
Text GLabel 5800 3200 0    50   Input ~ 0
VGA_R
Text GLabel 5800 3400 0    50   Input ~ 0
VGA_G
Text GLabel 5800 3600 0    50   Input ~ 0
VGA_B
Text GLabel 5850 3900 0    50   Input ~ 0
VGA_HSYNC
Text GLabel 5850 3800 0    50   Input ~ 0
VGA_VSYNC
$Comp
L power:GND #PWR?
U 1 1 6194824C
P 7850 4350
AR Path="/6194824C" Ref="#PWR?"  Part="1" 
AR Path="/619081B0/6194824C" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 7850 4100 50  0001 C CNN
F 1 "GND" H 7855 4177 50  0000 C CNN
F 2 "" H 7850 4350 50  0001 C CNN
F 3 "" H 7850 4350 50  0001 C CNN
	1    7850 4350
	1    0    0    -1  
$EndComp
Text GLabel 7400 3200 0    50   Input ~ 0
VGA_R
Text GLabel 7400 3400 0    50   Input ~ 0
VGA_G
Text GLabel 7400 3600 0    50   Input ~ 0
VGA_B
Text GLabel 8150 3600 2    50   Input ~ 0
VGA_HSYNC
Text GLabel 8150 3800 2    50   Input ~ 0
VGA_VSYNC
Wire Wire Line
	5950 3300 5900 3300
Wire Wire Line
	5900 3300 5900 3500
Wire Wire Line
	5900 3500 5950 3500
Wire Wire Line
	5900 3500 5900 3700
Wire Wire Line
	5900 3700 5950 3700
Connection ~ 5900 3500
Wire Wire Line
	5900 3700 5900 4100
Connection ~ 5900 3700
Wire Wire Line
	5950 3900 5850 3900
Wire Wire Line
	5950 3800 5850 3800
Wire Wire Line
	5950 3600 5800 3600
Wire Wire Line
	5950 3400 5800 3400
Wire Wire Line
	5950 3200 5800 3200
Wire Wire Line
	7550 3100 7500 3100
Wire Wire Line
	7500 3100 7500 3300
Wire Wire Line
	7500 3300 7550 3300
Wire Wire Line
	7500 3300 7500 3500
Wire Wire Line
	7500 3500 7550 3500
Connection ~ 7500 3300
Wire Wire Line
	7500 3500 7500 3900
Wire Wire Line
	7500 3900 7550 3900
Connection ~ 7500 3500
Wire Wire Line
	7500 3900 7500 4000
Wire Wire Line
	7500 4000 7550 4000
Connection ~ 7500 3900
Wire Wire Line
	7850 4300 7500 4300
Wire Wire Line
	7500 4300 7500 4000
Connection ~ 7500 4000
Wire Wire Line
	7850 4300 7850 4350
Wire Wire Line
	7550 3200 7400 3200
Wire Wire Line
	7550 3400 7400 3400
Wire Wire Line
	7550 3600 7400 3600
NoConn ~ 7550 3700
NoConn ~ 7550 3800
Connection ~ 7850 4300
$Comp
L Connector:DB15_Female_HighDensity_MountingHoles J?
U 1 1 6194827A
P 7850 3600
AR Path="/6194827A" Ref="J?"  Part="1" 
AR Path="/619081B0/6194827A" Ref="J3"  Part="1" 
F 0 "J3" H 7850 4467 50  0000 C CNN
F 1 "VGA" H 7850 4376 50  0000 C CNN
F 2 "Connector_Dsub:DSUB-15-HD_Female_Horizontal_P2.29x1.98mm_EdgePinOffset3.03mm_Housed_MountingHolesOffset4.94mm" H 6900 4000 50  0001 C CNN
F 3 " ~" H 6900 4000 50  0001 C CNN
	1    7850 3600
	1    0    0    -1  
$EndComp
NoConn ~ 8150 4000
NoConn ~ 8150 3400
NoConn ~ 8150 3200
$Comp
L Connector_Generic:Conn_02x25_Counter_Clockwise J?
U 1 1 61B13981
P 2700 3750
AR Path="/61B13981" Ref="J?"  Part="1" 
AR Path="/619081B0/61B13981" Ref="J1"  Part="1" 
F 0 "J1" H 2750 2325 50  0000 C CNN
F 1 "AppleIIBus" H 2750 2416 50  0000 C CNN
F 2 "AppleVGA:AppleIIBus_Edge" H 2700 3750 50  0001 C CNN
F 3 "~" H 2700 3750 50  0001 C CNN
	1    2700 3750
	-1   0    0    1   
$EndComp
NoConn ~ 2400 4950
Text Label 2400 4850 2    30   ~ 0
D0
Text Label 2400 4750 2    30   ~ 0
D1
Text Label 2400 4650 2    30   ~ 0
D2
Text Label 2400 4550 2    30   ~ 0
D3
Text Label 2400 4450 2    30   ~ 0
D4
Text Label 2400 4350 2    30   ~ 0
D5
Text Label 2400 4250 2    30   ~ 0
D6
Text Label 2400 4150 2    30   ~ 0
D7
Text Label 2400 3950 2    30   ~ 0
PHI0
Text Label 2400 3750 2    30   ~ 0
PHI1
Text Label 2400 3650 2    30   ~ 0
Q3
Text Label 2400 3550 2    30   ~ 0
7M
Text Label 2400 3050 2    30   ~ 0
~RES
Text Label 2400 2950 2    30   ~ 0
~IRQ
Text Label 2400 2850 2    30   ~ 0
~NMI
Text Label 2400 2750 2    30   ~ 0
~INTIN
Text Label 2400 2650 2    30   ~ 0
~DMAIN
$Comp
L power:GND #PWR?
U 1 1 61B1399A
P 1650 2600
AR Path="/61B1399A" Ref="#PWR?"  Part="1" 
AR Path="/619081B0/61B1399A" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 1650 2350 50  0001 C CNN
F 1 "GND" H 1655 2427 50  0000 C CNN
F 2 "" H 1650 2600 50  0001 C CNN
F 3 "" H 1650 2600 50  0001 C CNN
	1    1650 2600
	1    0    0    -1  
$EndComp
NoConn ~ 2400 3150
NoConn ~ 2400 3250
NoConn ~ 2400 3350
NoConn ~ 2400 3450
$Comp
L power:+5V #PWR?
U 1 1 61B139A4
P 2900 2250
AR Path="/61B139A4" Ref="#PWR?"  Part="1" 
AR Path="/619081B0/61B139A4" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 2900 2100 50  0001 C CNN
F 1 "+5V" H 2915 2423 50  0000 C CNN
F 2 "" H 2900 2250 50  0001 C CNN
F 3 "" H 2900 2250 50  0001 C CNN
	1    2900 2250
	1    0    0    -1  
$EndComp
Text Label 2900 2650 0    30   ~ 0
~DMAOUT
Text Label 2900 2750 0    30   ~ 0
~INTOUT
Text Label 2900 2850 0    30   ~ 0
~DMA
Text Label 2900 2950 0    30   ~ 0
~RDY
Text Label 2900 3050 0    30   ~ 0
~IOSTRB
Text Label 2900 3250 0    30   ~ 0
R~W
Text Label 2900 3350 0    30   ~ 0
A15
Text Label 2900 3450 0    30   ~ 0
A14
Text Label 2900 3550 0    30   ~ 0
A13
Text Label 2900 3650 0    30   ~ 0
A12
Text Label 2900 3750 0    30   ~ 0
A11
Text Label 2900 3850 0    30   ~ 0
A10
Text Label 2900 3950 0    30   ~ 0
A9
Text Label 2900 4050 0    30   ~ 0
A8
Text Label 2900 4150 0    30   ~ 0
A7
Text Label 2900 4250 0    30   ~ 0
A6
Text Label 2900 4350 0    30   ~ 0
A5
Text Label 2900 4450 0    30   ~ 0
A4
Text Label 2900 4550 0    30   ~ 0
A3
Text Label 2900 4650 0    30   ~ 0
A2
Text Label 2900 4750 0    30   ~ 0
A1
Text Label 2900 4850 0    30   ~ 0
A0
Text Label 2400 3150 2    30   ~ 0
~INH
Text Label 2900 3150 0    30   ~ 0
SYNC
Text GLabel 2150 2750 0    30   Output ~ 0
~INTPASS
Text GLabel 3150 2750 2    30   Input ~ 0
~INTPASS
Text GLabel 3150 2650 2    30   Input ~ 0
~DMAPASS
Text GLabel 2150 2650 0    30   Output ~ 0
~DMAPASS
Wire Wire Line
	2400 2650 2150 2650
Wire Wire Line
	2150 2750 2400 2750
Wire Wire Line
	2900 2750 3150 2750
Wire Wire Line
	2900 2650 3150 2650
NoConn ~ 2400 2850
NoConn ~ 2400 2950
NoConn ~ 2900 2850
NoConn ~ 2900 2950
NoConn ~ 2900 3050
NoConn ~ 2900 3150
NoConn ~ 2400 3050
NoConn ~ 2400 3550
NoConn ~ 2400 3650
NoConn ~ 2400 3850
Entry Wire Line
	2200 4850 2300 4750
Entry Wire Line
	2200 4750 2300 4650
Entry Wire Line
	2200 4650 2300 4550
Entry Wire Line
	2200 4550 2300 4450
Entry Wire Line
	2200 4450 2300 4350
Entry Wire Line
	2200 4350 2300 4250
Entry Wire Line
	2200 4950 2300 4850
Entry Wire Line
	3000 4850 3100 4950
Entry Wire Line
	3000 4750 3100 4850
Entry Wire Line
	3000 4650 3100 4750
Entry Wire Line
	3000 4550 3100 4650
Entry Wire Line
	3000 4450 3100 4550
Entry Wire Line
	3000 4350 3100 4450
Entry Wire Line
	3000 4250 3100 4350
Entry Wire Line
	3000 4150 3100 4250
Entry Wire Line
	3000 4050 3100 4150
Entry Wire Line
	3000 3950 3100 4050
Entry Wire Line
	3000 3850 3100 3950
Entry Wire Line
	3000 3750 3100 3850
Entry Wire Line
	3000 3650 3100 3750
Entry Wire Line
	3000 3550 3100 3650
Entry Wire Line
	3000 3450 3100 3550
Entry Wire Line
	3000 3350 3100 3450
Wire Wire Line
	3000 4850 2900 4850
Wire Wire Line
	3000 4750 2900 4750
Wire Wire Line
	3000 4650 2900 4650
Wire Wire Line
	3000 4550 2900 4550
Wire Wire Line
	3000 4450 2900 4450
Wire Wire Line
	3000 4350 2900 4350
Wire Wire Line
	3000 4250 2900 4250
Wire Wire Line
	3000 4150 2900 4150
Wire Wire Line
	3000 4050 2900 4050
Wire Wire Line
	3000 3950 2900 3950
Wire Wire Line
	3000 3850 2900 3850
Wire Wire Line
	3000 3750 2900 3750
Wire Wire Line
	3000 3650 2900 3650
Wire Wire Line
	3000 3550 2900 3550
Wire Wire Line
	3000 3450 2900 3450
Wire Wire Line
	3000 3350 2900 3350
Wire Wire Line
	2300 4250 2400 4250
Wire Wire Line
	2300 4350 2400 4350
Wire Wire Line
	2300 4450 2400 4450
Wire Wire Line
	2300 4550 2400 4550
Wire Wire Line
	2300 4650 2400 4650
Wire Wire Line
	2300 4750 2400 4750
Wire Wire Line
	2300 4850 2400 4850
Text GLabel 3150 3250 2    30   Output ~ 0
R~W
Wire Wire Line
	3150 3250 2900 3250
Wire Wire Line
	2400 3950 2150 3950
NoConn ~ 2400 3750
Wire Wire Line
	2300 4150 2400 4150
Entry Wire Line
	2200 4250 2300 4150
Wire Bus Line
	3100 5000 3750 5000
Wire Bus Line
	2200 5100 3750 5100
Text GLabel 3750 5000 2    50   Output ~ 0
A[0..15]
Text GLabel 3750 5100 2    50   BiDi ~ 0
D[0..7]
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 61C4590B
P 3000 2250
F 0 "#FLG0101" H 3000 2325 50  0001 C CNN
F 1 "PWR_FLAG" H 3000 2423 50  0001 C CNN
F 2 "" H 3000 2250 50  0001 C CNN
F 3 "~" H 3000 2250 50  0001 C CNN
	1    3000 2250
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 61C467CC
P 1550 2600
F 0 "#FLG0102" H 1550 2675 50  0001 C CNN
F 1 "PWR_FLAG" H 1550 2773 50  0001 C CNN
F 2 "" H 1550 2600 50  0001 C CNN
F 3 "~" H 1550 2600 50  0001 C CNN
	1    1550 2600
	-1   0    0    1   
$EndComp
Wire Wire Line
	1650 2600 1650 2550
Wire Wire Line
	1650 2550 2400 2550
Wire Wire Line
	1650 2550 1550 2550
Wire Wire Line
	1550 2550 1550 2600
Connection ~ 1650 2550
Wire Wire Line
	2900 2550 2950 2550
Wire Wire Line
	2950 2550 2950 2300
Wire Wire Line
	2950 2300 2900 2300
Wire Wire Line
	2900 2300 2900 2250
Wire Wire Line
	2950 2300 3000 2300
Wire Wire Line
	3000 2300 3000 2250
Connection ~ 2950 2300
Text GLabel 2150 4050 0    30   Output ~ 0
~DEVSEL
Text Label 2400 4050 2    30   ~ 0
~DEVSEL
Wire Wire Line
	2150 4050 2400 4050
Text Label 2900 4950 0    30   ~ 0
~IOSEL
NoConn ~ 2900 4950
Text GLabel 2150 3950 0    40   Output ~ 0
PHI0
Wire Bus Line
	2200 4200 2200 5100
Wire Bus Line
	3100 3400 3100 5000
$EndSCHEMATC
