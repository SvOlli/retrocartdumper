/*
 ROM dumper written by Sven Oliver Moll
 Adapted for Arduino Nano by Dion Olsthoorn
 ==============================================================================

 Distributed under the terms of the GPLv3.

 Tools -> Board -> Arduino Nano
*/

#define VERSION "0"
#define AD_THRESHOLD 512

void setup() {
  Serial.begin(38400);
  
  /* address bus: out */
  // The cartridge address signals A0-A11 are mapped across port registers D and B.
  // A12 is hard wired to VCC, as there are no more free GPIOs on the Nano
  DDRD = DDRD | B11111100; // preserve bits 0 and 1 (TX/RX)
  DDRB = B00111111; // note: bits 6 and 7 are not usable
  // see https://www.arduino.cc/en/Reference/PortManipulation
  
  /* data bus: in */
  DDRC = B00000000;
}

static inline void setaddr( uint16_t addr )
{ 
  // Because the cartidge A12 signal is wired to VCC, only backswitching schemes
  // are supported which use hotspots within the cartridge ROM address space. 
  // So f8, f6, f4, parker, CBS, M-Network are supported, but e.g. not TigerVision's 3f.
  PORTB = addr >> 6; 
  PORTD = (PORTD & B00000011) | (addr << 2); // also preserving TX/RX bits
  delay(1);
}

static inline uint8_t peek( uint16_t addr )
{
  setaddr( addr );

  uint8_t data = PINC;
  if (analogRead(A6) > AD_THRESHOLD) data |= B01000000;
  if (analogRead(A7) > AD_THRESHOLD) data |= B10000000;
  
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

void printHex(int num)
{
  char tmp[4];
  sprintf(tmp, "%02X", num);
  Serial.print(tmp);
}

void executeCmd( uint8_t cmd, uint16_t arg1, uint16_t arg2 )
{
  uint16_t i;
  switch( cmd )
  {
    case 'V':
      Serial.println( VERSION );
      break;
    case 'R':
      printHex( peek( arg1 ) );
      break;
    case 'W':
      /* poke/write not supported for Arduino Nano dumper, so just ignore */
      break;
    case 'D':
      for( i = 0; i < arg2; ++i )
      {
        printHex( peek( arg1+i ) );
      }
      break;
  }
}

void loop() 
{  
  int incomingByte;
  uint8_t cmd = 0;
  uint8_t pos = 0;
  uint16_t arg1 = 0x0000;
  uint16_t arg2 = 0x0000;
  bool cmdDone = false;

  while( !cmdDone )
  {
    if ( Serial.available() )
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
              pos++;
              break;
            case 'w':
            case 'W':
              cmd = 'W';
              pos++;
              break;
            case 'd':
            case 'D':
              cmd = 'D';
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
