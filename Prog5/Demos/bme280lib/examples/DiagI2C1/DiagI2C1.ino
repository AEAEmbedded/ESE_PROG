// Why does the I2C1 header (SDA1 = PB7, SCL1 = PB6) find nothing?
// 1. Before touching the I2C peripheral, read both lines as plain inputs, five
//    times, with the internal pull-up off and on. An I2C line with an external
//    pull-up reads HIGH in both cases; a floating line changes between the two;
//    a line held low by a slave (or shorted) reads LOW in both.
// 2. Then start I2C1 and scan, so the two observations sit next to each other.

#include <Wire.h>

void sample(const char* label, uint32_t mode)
{
    pinMode(PB7, mode); pinMode(PB6, mode);
    delay(5);
    Serial.print(label); Serial.print("  SDA1(PB7)=");
    for (int i = 0; i < 5; ++i) Serial.print(digitalRead(PB7));
    Serial.print("  SCL1(PB6)=");
    for (int i = 0; i < 5; ++i) Serial.print(digitalRead(PB6));
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}
    Serial.println("\n--- DiagI2C1 ---");
    sample("no pull-up  ", INPUT);
    sample("int. pull-up", INPUT_PULLUP);
    sample("no pull-up  ", INPUT);
    Serial.println("HIGH in all rows = external pull-ups present. Changing = floating (no pull-ups). LOW = held low.");

    Wire.setSDA(PB7); Wire.setSCL(PB6);   // explicit, independent of the selected board variant
    Wire.begin();
    Wire.setClock(100000);
}

void loop()
{
    Serial.print("I2C1 scan: ");
    int found = 0;
    for (uint8_t addr = 0x08; addr < 0x78; ++addr) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) { Serial.print("0x"); Serial.print(addr, HEX); Serial.print(' '); ++found; }
    }
    Serial.println(found ? "" : "nothing");
    delay(2000);
}
