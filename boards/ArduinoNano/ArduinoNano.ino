/*
 ROM dumper written by Sven Oliver Moll
 Changes for Arduino Nano version by Dion Olsthoorn
 ==============================================================================

 Distributed under the terms of the GPLv3.

 Sorry for using Arduino IDE, but I didn't know how else to get USB serial.

 Tools -> Board -> Arduino Nano
*/

#define VERSION "0"
#define DEBUG_OUTPUT 0
#define AD_THRESHOLD 512

void setup() {
  Serial.begin(9600);
  
  /* address bus: out */
  DDRD = DDRD | B11111100; // preserve bits 0 and 1 (used for serial comm.)
  DDRB = B00111111; // note: bits 6 and 7 are not usable
  // see https://www.arduino.cc/en/Reference/PortManipulation

  /* data bus: in */
  DDRC = B00000000;
}

static inline void setaddr( uint16_t addr )
{ 
  // Note that he cartidge's A12 signal is wired to VCC, as we don't 
  // have any more free pins on the Arduino Nano. This means that 
  // only bankswitching schemes are supported which use hotspots within 
  // the cartidge ROM address space (i.e. f8, f6, f4, parker, CBS, M-Network)
  PORTB = addr >> 6; 
  PORTD = (PORTD & B00000011) | (addr << 2); // preserve TX/RX bits
  delay(1);
}

void printHex(int num)
{
  char tmp[3];
  sprintf(tmp, "%02X", num);
  Serial.print(tmp);
}

static inline uint8_t peek( uint16_t addr )
{
  setaddr( addr );

  uint8_t data = PINC;
  if (analogRead(A6) > AD_THRESHOLD) data = data | B01000000;
  if (analogRead(A7) > AD_THRESHOLD) data = data | B10000000;
  
  return data;
}

uint16_t inputHex( uint16_t value, int b )
{
  if( (b >= '0') && (b <= '9') )
  {
    return (value << 4) | (b - '0');
  }
  if( (b >= 'a') && (b <= 'f') )
  {
    return (value << 4) | (b - 'a' + 10);
  }
  if( (b >= 'A') && (b <= 'F') )
  {
    return (value << 4) | (b - 'A' + 10);
  }
  return value;
}

void executeCmd( uint8_t cmd, uint16_t arg1, uint16_t arg2 )
{
  uint16_t i;
#if DEBUG_OUTPUT
  //Serial.printf( "%c %04X %04X\n", cmd, arg1, arg2 );
#endif
  switch( cmd )
  {
    case 'V':
      Serial.println( VERSION );
      break;
    case 'R':
#if DEBUG_OUTPUT
      //Serial.printf( "%04X %02X\n", arg1, peek( arg1 ) );
#else      
      //Serial.printf( "%02X", peek( arg1 ) );
      printHex( peek( arg1 ) );
#endif
      break;
    case 'W':
#if DEBUG_OUTPUT
      Serial.println( "poke/write not supported for Arduino Nano dumper" );
#endif
      break;
    case 'D':
      for( i = 0; i < arg2; ++i )
      {
#if DEBUG_OUTPUT
        //Serial.printf( "%04X %02X\n", arg1+i, peek( arg1+i ) );
#else      
        //Serial.printf( "%02X", peek( arg1+i ) );
        printHex( peek( arg1+i ) );
#endif
      }
#if DEBUG_OUTPUT
      Serial.println();
#endif
      break;
  }
}

void loop() {
  int incomingByte;
  uint8_t cmd = 0;
  uint8_t pos = 0;
  uint16_t arg1 = 0x0000;
  uint16_t arg2 = 0x0000;
  bool cmdDone = false;

  while( !cmdDone )
  {
    if (Serial.available() > 0)
    {
      incomingByte = Serial.read();
      if( incomingByte == '\r' )
      {
        pos = 255;
      }
      switch( pos )
      {
        case 0:
          switch( incomingByte )
          {
            case 'v':
            case 'V':
              cmd = 'V';
              pos = 3;
              break;
            case 'r':
            case 'R':
              cmd = 'R';
#if DEBUG_OUTPUT
              Serial.println("READ");
#endif
              pos++;
              break;
            case 'w':
            case 'W':
              cmd = 'W';
#if DEBUG_OUTPUT
              Serial.println("WRITE");
#endif
              pos++;
              break;
            case 'd':
            case 'D':
              cmd = 'D';
#if DEBUG_OUTPUT
              Serial.println("DUMP");
#endif
              pos++;
              break;
            default:
              break;
          }
          break;
        case 1:
          if( incomingByte == ' ' )
          {
            pos++;
          }
          else
          {
            arg1 = inputHex( arg1, incomingByte );
#if DEBUG_OUTPUT
            Serial.println( arg1, 16 );
#endif
          }
          break;
        case 2:
          if( incomingByte == ' ' )
          {
            pos++;
          }
          else
          {
            arg2 = inputHex( arg2, incomingByte );
#if DEBUG_OUTPUT
            Serial.println( arg2, 16 );
#endif
          }
          break;
        case 255:
          executeCmd( cmd, arg1, arg2 );
          cmdDone = true;
          break;
        default:
          break;
      }
    }
  }
}
