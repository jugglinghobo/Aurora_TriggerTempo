#ifndef Aurora_TriggerTempo_h
#define Aurora_TriggerTempo_h

#include "Arduino.h"

class Aurora_TriggerTempo {
  private:
    static const int DEBOUNCE_DELAY = 50;
    byte triggerPin;

    unsigned long lastDebounceMillis;

    int inputState;             
    int lastInputState;

    int beatDivision;

    unsigned long lastTriggerMillis;
    unsigned long currentTriggerMillis;
    unsigned long lastTempoGateTrigger;

    int tempo;
    bool gate;
    bool _debug;

    void triggerGate();
    void setGateFromTempo();
    int calculateAbsoluteTempo(); 
    int getDivisionBasedTempo();
    bool tempoTriggersGate();


  public:
    Aurora_TriggerTempo(byte pin, int initialBeatDivision = DIVISION_QUARTER, int initialTempo = 1000);
    bool getGate(); 
    void setBeatDivision(int division);
    void debug(bool enabled = true);

    static const int DIVISION_HALF = 2;
    static const int DIVISION_QUARTER = 4;
    static const int DIVISION_EIGHT = 8;
};
#endif
