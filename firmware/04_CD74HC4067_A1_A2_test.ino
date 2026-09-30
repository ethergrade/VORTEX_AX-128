// NEXT TEST - NOT YET VALIDATED ON HARDWARE.
// CD74HC4067:
// S0 GPIO4
// S1 GPIO6
// S2 GPIO7
// S3 GPIO15
// SIG -> 1k -> GND
// EN -> GND
// C0 -> J1-2
// C1 -> J1-4
// J1-3 -> GPIO5

const int S0 = 4;
const int S1 = 6;
const int S2 = 7;
const int S3 = 15;

const int ROW_A = 5;

bool oldA1 = false;
bool oldA2 = false;

void selectChannel(byte ch) {
  digitalWrite(S0, ch & 1);
  digitalWrite(S1, (ch >> 1) & 1);
  digitalWrite(S2, (ch >> 2) & 1);
  digitalWrite(S3, (ch >> 3) & 1);

  delayMicroseconds(10);
}

bool readKey(byte column) {
  selectChannel(column);
  return digitalRead(ROW_A) == LOW;
}

void setup() {
  Serial.begin(115200);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(ROW_A, INPUT_PULLUP);

  Serial.println("VORTEX AX-128 - CD74HC4067 TEST");
}

void loop() {
  bool A1 = readKey(0);
  bool A2 = readKey(1);

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
