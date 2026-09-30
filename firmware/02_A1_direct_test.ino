const int COL1 = 4;
const int ROW_A = 5;

void setup() {
  Serial.begin(115200);

  pinMode(ROW_A, INPUT_PULLUP);

  pinMode(COL1, OUTPUT);
  digitalWrite(COL1, LOW);

  Serial.println("VORTEX AX-128 - TEST A1");
}

void loop() {
  static bool oldState = false;

  bool pressed = digitalRead(ROW_A) == LOW;

  if (pressed != oldState) {
    oldState = pressed;

    if (pressed) {
      Serial.println("A1 PRESSED");
    } else {
      Serial.println("A1 RELEASED");
    }
  }

  delay(5);
}
