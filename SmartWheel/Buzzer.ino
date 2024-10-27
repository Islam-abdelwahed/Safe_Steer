void Buzzer_Init() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void Buzzer_Short_Beeb() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
}

void Buzzer_Long_Beeb() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}