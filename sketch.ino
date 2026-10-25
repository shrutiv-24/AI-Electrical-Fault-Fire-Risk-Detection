const int PIN_I = 34;   // current knob
const int PIN_V = 35;   // voltage knob
const int PIN_T = 32;   // temperature knob
const int GREEN = 25;
const int YELLOW = 26;
const int RED = 27;
const int BUZZ = 14;
const int RELAY = 33;   // cuts the low-voltage test load

String shown = "NORMAL";
String lastRisk = "";
int same = 0;

void setup() {
  Serial.begin(115200);
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(RELAY, OUTPUT);
}

void loop() {
  float current = analogRead(PIN_I) * 30.0 / 4095;
  float voltage = 180 + analogRead(PIN_V) * 80.0 / 4095;
  float temp = 20 + analogRead(PIN_T) * 80.0 / 4095;
  float power = current * voltage;

  String risk = "NORMAL";
  if (voltage < 200 || voltage > 250) risk = "VOLTAGE ABNORMAL";
  if (current >= 9) risk = "OVERLOAD RISK";
  if (temp >= 48) risk = "ABNORMAL HEATING";
  if ((current >= 11 && temp >= 48) || temp >= 62) risk = "HIGH FIRE RISK";

  if (risk == lastRisk) same++; else { same = 0; lastRisk = risk; }
  if (same >= 2) shown = risk;

  digitalWrite(GREEN, shown == "NORMAL");
  digitalWrite(YELLOW, shown == "OVERLOAD RISK" || shown == "ABNORMAL HEATING" || shown == "VOLTAGE ABNORMAL");
  digitalWrite(RED, shown == "HIGH FIRE RISK");
  digitalWrite(RELAY, shown == "HIGH FIRE RISK");
  if (shown == "HIGH FIRE RISK") tone(BUZZ, 2000, 150);
  else if (shown != "NORMAL") tone(BUZZ, 1000, 100);

  Serial.print("Current A: "); Serial.print(current);
  Serial.print("  Voltage V: "); Serial.print(voltage);
  Serial.print("  Temp C: "); Serial.print(temp);
  Serial.print("  Power W: "); Serial.print(power);
  Serial.print("  Status: "); Serial.println(shown);
  delay(300);
}
