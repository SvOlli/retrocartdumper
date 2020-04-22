
 Wiring
========

Atari Cartridge Slot
--------------------
```
 A-B-C-D-E-F-G-H-I-J-K-L
|= = = = = = = = = = = =|
 X-W-V-U-T-S-R-Q-P-O-N-M
```

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

| Atari 2600 Signal Name | Atari 2600 Pin | Teensy++ 2.0 Pin | Teensy++ 2.0 Name |
|------------------------|----------------|------------------|-------------------|
| L                      | SGND           | 1                | GND               |
| -                      | -              | 2                | PB7               |
| J                      | A8             | 3                | PD0               |
| I                      | A9             | 4                | PD1               |
| G                      | A10            | 5                | PD2               |
| H                      | A11            | 6                | PD3               |
| F                      | A12            | 7                | PD4               |
| -                      | (A13)*         | 8                | PD5               |
| -                      | (A14)*         | 9                | PD6               |
| -                      | (A15)*         | 10               | PD7               |
| -                      | (R/W)*         | 11               | PE0               |
| -                      | -              | 12               | PE1               |
| T                      | A0             | 13               | PC0               |
| S                      | A1             | 14               | PC1               |
| R                      | A2             | 15               | PC2               |
| Q                      | A3             | 16               | PC3               |
| P                      | A4             | 17               | PC4               |
| O                      | A5             | 18               | PC5               |
| N                      | A6             | 19               | PC6               |
| M                      | A7             | 20               | PC7               |
| E                      | D7             | 21               | PF7               |
| D                      | D6             | 22               | PF6               |
| C                      | D5             | 23               | PF5               |
| B                      | D4             | 24               | PF4               |
| A                      | D3             | 25               | PF3               |
| W                      | D2             | 26               | PF2               |
| V                      | D1             | 27               | PF1               |
| U                      | D0             | 28               | PF0               |
| -                      | -              | 29               | AREF              |
| X                      | GND            | 30               | GND               |
| -                      | -              | 31               | PE6               |
| -                      | -              | 32               | PE7               |
| -                      | -              | 33               | PB0               |
| -                      | -              | 34               | PB1               |
| -                      | -              | 35               | PB2               |
| -                      | -              | 36               | PB3               |
| -                      | -              | 37               | PB4               |
| -                      | -              | 38               | PB5               |
| -                      | -              | 39               | PB6               |
| K                      | +5V            | 40               | VCC               |

(* These signals do not exist on the Atari 2600 cartridge slot, but are still handled by
firmware like the would exist. For easier adaptation to other platforms.)
