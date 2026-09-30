// TESTED: A1/A2 scanning worked, including overlapping presses.

const int ROW_A = 5;

const int COL1 = 4;
const int COL2 = 6;

bool oldA1 = false;
bool oldA2 = false;

bool readKey(int columnPin) {
  pinMode(COL1, INPUT);
  pinMode(COL2, INPUT);

  pinMode(columnPin, OUTPUT);
  digitalWrite(columnPin, LOW);

  delayMicroseconds(10);

  bool pressed = (digitalRead(ROW_A) == LOW);

  pinMode(columnPin, INPUT);

  return pressed;
}

void setup() {
  Serial.begin(115200);

  pinMode(ROW_A, INPUT_PULLUP);

  pinMode(COL1, INPUT);
  pinMode(COL2, INPUT);

  Serial.println("VORTEX AX-128 - MATRIX TEST A1/A2");
}

void loop() {
  bool A1 = readKey(COL1);
  bool A2 = readKey(COL2);

  if (A1 != oldA1) {
    oldA1 = A1;
    Serial.println(A1 ? "A1 PRESSED" : "A1 RELEASED");
  }

  if (A2 != oldA2) {
    oldA2 = A2;
    Serial.println(A2 ? "A2 PRESSED" : "A2 RELEASED");
  }

  delay(2);
}
