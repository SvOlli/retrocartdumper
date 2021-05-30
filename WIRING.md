
 Wiring
========

Atari Cartridge Slot
--------------------
```
  24 23 22 21 20 19 18 17 16 15 14 13
| == == == == == == == == == == == == |
| == == == == == == == == == == == == |
   1  2  3  4  5  6  7  8  9 10 11 12
```
![image info](./pictures/atari_2600_cart_port_pinout.jpg)

Pinout Teensy++ 2.0
-------------------
```
     _ _
  1-| v |-40
  2-|   |-39
  3-|   |-38
  4-|   |-37
  5-|   |-36
  6-|   |-35
  7-|   |-34
  8-|   |-33
  9-|   |-32
 10-|   |-31
 11-|   |-30
 12-|   |-29
 13-|   |-28
 14-|   |-27
 15-|   |-26
 16-|   |-25
 17-|   |-24
 18-|   |-23
 19-|   |-22
 20-|___|-21

```
Pinout Arduino Nano
-------------------
```
     _ _
 16-| v |-15
 17-|   |-14
 18-|   |-13
 19-|   |-12
 20-|   |-11
 21-|   |-10
 22-|   |-9
 23-|   |-8
 24-|   |-7
 25-|   |-6
 26-|   |-5
 27-|   |-4
 28-|   |-3
 29-|   |-2
 30-|___|-1

```

| Atari 2600 Pin  | Teensy++ 2.0 Pin | Arduino Nano Pin |
|-----------------|------------------|------------------|
| A7  (1)         | PC7  (20)        | D9  (12)         |
| A6  (2)         | PC6  (19)        | D8  (11)         |
| A5  (3)         | PC5  (18)        | D7  (10)         |
| A4  (4)         | PC4  (17)        | D6  (9)          |
| A3  (5)         | PC3  (16)        | D5  (8)          |
| A2  (6)         | PC2  (15)        | D4  (7)          |
| A1  (7)         | PC1  (14)        | D3  (6)          |
| A0  (8)         | PC0  (13)        | D2  (5)          |
| D0  (9)         | PF0  (28)        | A0  (19) ***     |
| D1  (10)        | PF1  (27)        | A1  (20)         |
| D2  (11)        | PF2  (26)        | A2  (21)         |
| GND (12)        | GND  (30)        | GND (29)         |
| D3  (13)        | PF3  (25)        | A3  (22)         |
| D4  (14)        | PF4  (24)        | A4  (23)         |
| D5  (15)        | PF5  (23)        | A5  (24)         |
| D6  (16)        | PF6  (22)        | A6  (25)         |
| D7  (17)        | PF7  (21)        | A7  (26)         |
| R/W *           | PE0  (11)        | --               |
| A15 *           | PD7  (10)        | --               |
| A14 *           | PD6  (9)         | --               |
| A13 *           | PD5  (8)         | --               |
| A12 (18)        | PD4  (7)         | ***5V*** (27) ** |
| A11 (***20***)  | PD3  (6)         | D13 (16)         |
| A10 (***19***)  | PD2  (5)         | D12 (15)         |
| A9  (21)        | PD1  (4)         | D11 (14)         |
| A8  (22)        | PD0  (3)         | D10 (13)         |
| +5V (23)        | +5V  (40)        | +5V (27)         |
| GND (24)        | GND  (1)         | GND (4)          |
---------------------------------------------------------

\* [Teensy++ 2.0] Signals A13, A14, A15 and R/W do not exist on the Atari 2600 cartridge slot, but are still handled by the Teensy dumper like they would exist. 
This is done for easier adaptation to other platforms.

\*\* [Arduino Nano] Because GPIO pins are limited on the Nano, the cartridge A12 signal is wired to +5V, which basically always selects the ROM address space $1000-$1fff.
This means that when using the Nano dumper, only bankswitching schemes are supported which use hotspots *within* the cartridge ROM address space.
For most cartridges brands (e.g. Atari, Activision, Imagic, CBS, M-Network and Parker Brothers) this is sufficient.
If you need support for dumping TigerVision's 3F or Superbanking cartridges, please use the Teensy++ 2.0 microcontroller instead.

\*\*\* [Arduino Nano] Since the A6 and A7 pins on the Nano are analog-in only, writing to the the data bus is not
supported on the Nano. This means that poke/write commands are ignored. 
If you need to do poke/write to the cartridge, please use the Teensy++ 2.0 microcontroller instead.

