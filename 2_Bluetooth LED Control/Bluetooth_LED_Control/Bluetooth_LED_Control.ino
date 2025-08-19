char Incomming_Value = 0;
int LED_PIN = 8;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {
  if(Serial.available()>0){
    Incoming_value = Serial.read();
    Serial.print(Incoming_Value);
    Serial.print("\n");

    if(Incoming_Value == '1')
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else if(Incoming_Value == '0')
    {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
