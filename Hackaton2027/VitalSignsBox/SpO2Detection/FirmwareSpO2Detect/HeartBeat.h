/*
    HeartBeat.h

    Non-blocking LED heartbeat indicator

    Johan Korten
    for HAN ESE / WKZ Hackaton Challenge 2026

    V1.0 Jan 2026
*/

#ifndef HeartBeat_h
#define HeartBeat_h

class HeartBeat {
  private:
    unsigned int _ledPin;
    long _interval;
    long _previousMillis;
  public:
    HeartBeat();
    HeartBeat(unsigned int ledPin, long interval);
    void begin();
    void blink();
};

#endif
