/*
 ROM dumper written by Sven Oliver Moll
 ======================================

 Distributed under the terms of the GPLv3.

 Sorry for using Arduino IDE, but I didn't know how else to get USB serial.

 Tools -> Board -> Teensy++ 2.0
 Tools -> USB Type -> Serial
*/

#define DEBUG_OUTPUT 0

void setup() {
	Serial.begin(38400);
  /* address bus: out */
  DDRD = 0xFF;
  DDRC = 0xFF;
}

static inline void setaddr( uint16_t addr )
{
  PORTD = addr >> 8;
  PORTC = addr & 0xFF;
  delay(1);
}

static inline uint8_t peek( uint16_t addr )
{
  /* data bus: in */
  DDRF = 0x00;

  setaddr( addr );
  PORTE |= 0x01;
  return PINF;
}

static inline void poke( uint16_t addr, uint8_t data )
{
  /* data bus: out */
  DDRF = 0xFF;

  setaddr( addr );
  PORTE &= 0xFE;
  PORTF = data;
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
  char outbuffer[16];
  snprintf( &outbuffer[0], sizeof(outbuffer)-1, "%c %04X %04X", cmd, arg1, arg2 );
#if DEBUG_OUTPUT
  Serial.println( &outbuffer[0] );
#endif
  switch( cmd )
  {
    case 'R':
#if DEBUG_OUTPUT
      snprintf( &outbuffer[0], sizeof(outbuffer)-1, "%04X %02X\n", arg1, peek( arg1 ) );
#else      
      snprintf( &outbuffer[0], sizeof(outbuffer)-1, "%02X", peek( arg1 ) );
#endif
      Serial.print( &outbuffer[0] );
      break;
    case 'W':
      poke( arg1, arg2 );
      break;
    case 'D':
      for( i = 0; i < arg2; ++i )
      {
#if DEBUG_OUTPUT
        snprintf( &outbuffer[0], sizeof(outbuffer)-1, "%04X %02X\n", arg1+i, peek( arg1+i ) );
#else      
        snprintf( &outbuffer[0], sizeof(outbuffer)-1, "%02X", peek( arg1+i ) );
#endif
        Serial.print( &outbuffer[0] );
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
      if( incomingByte == 0x0d )
      {
        pos = 255;
      }
      switch( pos )
      {
        case 0:
          switch( incomingByte )
          {
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
          if( incomingByte == ' ')
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
          arg2 = inputHex( arg2, incomingByte );
#if DEBUG_OUTPUT
          Serial.println( arg2, 16 );
#endif
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
