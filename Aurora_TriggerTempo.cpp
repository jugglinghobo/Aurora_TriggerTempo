#include "Arduino.h"
#include "Aurora_TriggerTempo.h"

Aurora_TriggerTempo::Aurora_TriggerTempo(byte pin, int initialBeatDivision, int initialTempo) {
  triggerPin = pin;
  pinMode(triggerPin, INPUT);

  lastDebounceMillis = 0;
  inputState = LOW;
  lastInputState = LOW;
  gate = LOW;
  lastTriggerMillis = 0;
  currentTriggerMillis = 0;
  lastTempoGateTrigger = 0;

  beatDivision = initialBeatDivision;
  tempo = initialTempo;
}

void Aurora_TriggerTempo::debug(bool enabled) {
  _debug = enabled;
}

bool Aurora_TriggerTempo::getGate() {
  setGateFromTempo();

  bool touchTempoButtonReading = digitalRead(triggerPin);

  if (touchTempoButtonReading != lastInputState) {
    lastDebounceMillis = millis();
  }

  if ((millis() - lastDebounceMillis) >= DEBOUNCE_DELAY) {

    if (touchTempoButtonReading != inputState) {
      inputState = touchTempoButtonReading;

      if (inputState) {
        tempo = calculateAbsoluteTempo();
        triggerGate();
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastInputState = touchTempoButtonReading;

  return gate;
}

void Aurora_TriggerTempo::setBeatDivision(int division) {
  beatDivision = division;
}

void Aurora_TriggerTempo::triggerGate() {
  gate = HIGH;
  lastTempoGateTrigger = millis();
}

void Aurora_TriggerTempo::setGateFromTempo() {
  if (tempoTriggersGate()) {
    triggerGate();
  } else {
    gate = LOW;
  }
}

int Aurora_TriggerTempo::calculateAbsoluteTempo() {
  lastTriggerMillis = currentTriggerMillis;
  currentTriggerMillis = millis();
  int absoluteTempo = currentTriggerMillis - lastTriggerMillis;
  return absoluteTempo;
}

bool Aurora_TriggerTempo::tempoTriggersGate() {
  return ((millis() - lastTempoGateTrigger) >= getDivisionBasedTempo());
}

int Aurora_TriggerTempo::getDivisionBasedTempo() {
  switch (beatDivision) {
    case DIVISION_EIGHT:
      return tempo / 2;
      break;
    case DIVISION_HALF:
      return tempo * 2;
      break;
    case DIVISION_QUARTER:
    default:
      return tempo;
  }
}
