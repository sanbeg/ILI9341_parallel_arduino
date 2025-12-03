#include "ILI9341.h"

bool go_fast = 0;


void write_fast(uint8_t d) {
#ifdef ARDUINO_ARCH_AVR
  PORTD = d & B11111100;
  PORTB = d & B00000011;
#endif
}

void write_slow(uint8_t d) {
  digitalWrite(8, (d >> 0) & B1);
  digitalWrite(9, (d >> 1) & B1);
  digitalWrite(2, (d >> 2) & B1);
  digitalWrite(3, (d >> 3) & B1);
  digitalWrite(4, (d >> 4) & B1);
  digitalWrite(5, (d >> 5) & B1);
  digitalWrite(6, (d >> 6) & B1);
  digitalWrite(7, (d >> 7) & B1);
}

void emit_data(uint8_t data) {
  if (go_fast) {
    write_fast(data);
  } else {
    write_slow(data);
  }
}

void LCD_write(uint8_t d) {
  // ILI9341 reads data pins when WR rises from LOW to HIGH
  digitalWrite(LCD_WR, LOW);
  //write_fast(d);
  emit_data(d);
  digitalWrite(LCD_WR, HIGH);
}

void LCD_command_write(uint8_t d) {
  digitalWrite(LCD_RS, LOW);
  // write data pins
  LCD_write(d);
}

void LCD_data_write(uint8_t d) {
  digitalWrite(LCD_RS, HIGH);
  // write data pins
  LCD_write(d);
}

void LCD_clear(byte color) {

  LCD_command_write(0x2a);
  LCD_data_write(0);
  LCD_data_write(0);
  LCD_data_write(0);
  LCD_data_write(0xEC);
  LCD_command_write(0x2b);
  LCD_data_write(0);
  LCD_data_write(0);
  LCD_data_write(1);
  LCD_data_write(0x3F);
  LCD_command_write(0x2c);

  digitalWrite(LCD_RS, HIGH);
  //PORTD = color & B11111100;
  //PORTB = color & B00000011;
  emit_data(color);

  uint16_t x;
  x = 38400;  // 240*320/2

  if (go_fast) {
  #ifdef ARDUINO_ARCH_AVR
    byte wr0 = PORTC & B11111101;  // set WR 0
    byte wr1 = PORTC | B00000010;  // set WR 1

    for (uint16_t y = 0; y < x; y++) {
      PORTC = wr0;
      PORTC = wr1;
      PORTC = wr0;
      PORTC = wr1;

      PORTC = wr0;
      PORTC = wr1;
      PORTC = wr0;
      PORTC = wr1;
    }
  #endif
  } else {
    for (uint16_t y = 0; y < x; y++) {
      for (int z = 0; z < 4; ++z) {
        digitalWrite(LCD_WR, 0);
        digitalWrite(LCD_WR, 1);
      }
    }
  }
}

void init_fast() {
#ifdef ARDUINO_ARCH_AVR
  DDRD |= B11111100;
  DDRB |= B00000011;
#endif
}

void init_slow() {
  for (int i = 2; i <= 9; ++i) {
    pinMode(i, OUTPUT);
  }
}

void init_pins() {
  if (go_fast) {
    init_fast();
  } else {
    init_slow();
  }
}

void LCD_init(void) {

  //set data bus as output
  //DDRD |= B11111100;
  //DDRB |= B00000011;
  init_pins();

  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);

  // LCD_RESET 1 - 0 - 1, arduino pin A4
  digitalWrite(LCD_RST, HIGH);
  delay(10);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(20);

  // CS HIGH, WR HIGH, RD HIGH, CS LOW
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_CS, LOW);

  LCD_command_write(0xF7);  // Pump ratio control
  LCD_data_write(0x20);     //

  LCD_command_write(0x3A);  // COLMOD: Pixel Format Set
  LCD_data_write(0x55);

  LCD_command_write(0x36);  // Memory Access Control
  // MY  - Row Address Order (bit7)
  // MX  - Column Address Order
  // MV  - Row / Column Exchange
  // ML  - Vertical Refresh Order
  // BGR - RGB-BGR Order
  // MH  - Horizontal Refresh ORDER(bit2)
  LCD_data_write(B00001000);

  LCD_command_write(0x11);  // Sleep OUT
  LCD_command_write(0x29);  // Display ON

  delay(50);
}

void draw_rect(uint16_t col, uint16_t row, uint16_t width, uint16_t height, uint16_t color) {

  LCD_command_write(0x2a);  // Column Address Set
  LCD_data_write(row >> 8);
  LCD_data_write(row);
  LCD_data_write((row + height - 1) >> 8);
  LCD_data_write(row + height - 1);
  LCD_command_write(0x2b);  // Page Address Set
  LCD_data_write(col >> 8);
  LCD_data_write(col);
  LCD_data_write((col + width - 1) >> 8);
  LCD_data_write(col + width - 1);
  LCD_command_write(0x2c);  // Memory Write

  byte chigh = color >> 8;
  byte clow = color;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      LCD_data_write(chigh);
      LCD_data_write(clow);
    }
}