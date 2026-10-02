// Which bus is the sensor on, and at which address?
// Scans both I2C buses of the MicroMod ATP carrier every 2 s and reads the
// chip-id register of anything found at 0x76 / 0x77 (0x60 = BME280, 0x58 = BMP280).

#include <Wire.h>

// Both buses with explicit pins: which board you selected in the IDE decides what
// `Wire` means (generic F405: PB7/PB6, SparkFun MicroMod: PB11/PB10), so `Wire` is
// not used here at all.
TwoWire primaryI2c(PB11, PB10);    // ATP "I2C" header + Qwiic, STM32 I2C2
TwoWire secondaryI2c(PB7, PB6);    // ATP SDA1 / SCL1 header, STM32 I2C1

struct NamedBus { const char* name; TwoWire& wire; };
NamedBus buses[] = { { "Primary  (PB11/PB10)", primaryI2c },
                     { "Secondary (PB7/PB6) ", secondaryI2c } };

// Reads the chip-id register two ways: with a STOP between address and read
// (what the library uses) and, second, with a repeated start.
uint8_t chipId(TwoWire& w, uint8_t addr, bool repeatedStart)
{
    w.beginTransmission(addr);
    w.write(0xD0);
    if (w.endTransmission(!repeatedStart) != 0) return 0;
    if (w.requestFrom(addr, (uint8_t)1) != 1) return 0;
    return (uint8_t)w.read();
}

void printId(uint8_t id)
{
    Serial.print(id == 0x60 ? "BME280" : id == 0x58 ? "BMP280" : id == 0 ? "no answer" : "id ?");
}

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000) {}
    for (auto& b : buses) { b.wire.begin(); b.wire.setClock(100000); }   // slow and safe for a scan
}

void loop()
{
    for (auto& b : buses) {
        Serial.print(b.name); Serial.print(": ");
        int found = 0;
        for (uint8_t addr = 0x08; addr < 0x78; ++addr) {
            b.wire.beginTransmission(addr);
            if (b.wire.endTransmission() == 0) {
                Serial.print("0x"); Serial.print(addr, HEX);
                if (addr == 0x76 || addr == 0x77) {
                    Serial.print(" [stop: "); printId(chipId(b.wire, addr, false));
                    Serial.print(", repeated start: "); Serial.flush();
                    printId(chipId(b.wire, addr, true)); Serial.print("]");
                }
                Serial.print("  ");
                ++found;
            }
        }
        Serial.println(found ? "" : "nothing");
    }
    Serial.println();
    delay(2000);
}
