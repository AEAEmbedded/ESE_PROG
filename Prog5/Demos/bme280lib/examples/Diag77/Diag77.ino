// Step-by-step I2C diagnosis on the ATP Primary bus. Every call is announced
// before it runs and flushed, so the last line on the monitor names the call
// that does not return (or that resets the board).

#include <Wire.h>

TwoWire bus(PB11, PB10);
const uint8_t kBme = 0x77;   // change if the scanner found 0x76
const uint8_t kSht = 0x44;

void step(const char* what) { Serial.print(what); Serial.print(" ... "); Serial.flush(); delay(20); }
void done(long v)          { Serial.println(v); Serial.flush(); delay(20); }

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}
    Serial.println("\n--- Diag77: boot ---");
    bus.begin();
    bus.setClock(100000);
}

void loop()
{
    // 1. address probe (this worked in the scanner)
    step("probe 0x77: endTransmission"); bus.beginTransmission(kBme); done(bus.endTransmission());

    // 2. plain read without selecting a register (BME280 answers from its current pointer)
    step("requestFrom(0x77, 1) without write"); done(bus.requestFrom(kBme, (uint8_t)1));
    while (bus.available()) bus.read();

    // 3. write the register address only, with STOP
    step("write 0xD0 + STOP: endTransmission"); bus.beginTransmission(kBme); bus.write(0xD0); done(bus.endTransmission());

    // 4. now read one byte
    step("requestFrom(0x77, 1)"); done(bus.requestFrom(kBme, (uint8_t)1));
    if (bus.available()) { step("chip id"); done(bus.read()); }

    // 5. same on the SHT45 (known good from the research rig): serial-number command 0x89
    step("SHT45 write 0x89: endTransmission"); bus.beginTransmission(kSht); bus.write(0x89); done(bus.endTransmission());
    delay(10);
    step("SHT45 requestFrom(6)"); done(bus.requestFrom(kSht, (uint8_t)6));
    while (bus.available()) bus.read();

    Serial.println("--- cycle complete ---\n");
    delay(3000);
}
