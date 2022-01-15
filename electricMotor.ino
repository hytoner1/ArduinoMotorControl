const int ledPin = 13;

const int in1 = A0;
const int in2 = A1;

const int out1 = 8;
const int out2 = 9;

bool prevPin1Val;
bool prevPin2Val;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);

  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);

  pinMode(in1, INPUT);
  pinMode(in2, INPUT);

  prevPin1Val = digitalRead(in1);
  prevPin2Val = digitalRead(in2);

  digitalWrite(out1, HIGH);
  digitalWrite(out2, LOW);

  Serial.begin(9600);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long t = millis();
  bool pin1Val = digitalRead(in1);
  bool pin2Val = digitalRead(in2);

  if((pin1Val ^ pin2Val) != (prevPin1Val ^ prevPin2Val)){
    digitalWrite(out1, LOW);
    
    prevPin1Val = pin1Val;
    prevPin2Val = pin2Val;

    delay(1000);
    digitalWrite(out1, HIGH);
  }

  Serial.print(t);
  Serial.print(", ");
  Serial.print(pin1Val);
  Serial.print(", ");
  Serial.print(pin2Val);
  Serial.println("-");
}
