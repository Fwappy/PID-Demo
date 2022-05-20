//constants
const bool debug = 1;
const int iLim = 1023;
//inputs
bool toggle;
int goal;
int pK;
int iK;
int dK;
int pos;

//output
int out;

//vars
int p;
int i;
int d;
int prevP;

//debug
int dt;
int time;
int cycle;


void setup() {
  if (debug == 1) {
    Serial.begin(2000000);
  }
  
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  
  goal = analogRead(A0);
  dK = analogRead(A1)/6; // 0-171
  iK = analogRead(A2)/6; // 0-171
  pK = analogRead(A3)/6; // 0-171
  pos = analogRead(A4);

//proportional
  p = (goal - pos); // up to 1023

//integral
  i = i + p; // up to 1023

  //Integral windup limiter
  if (i > iLim) {
    i = iLim;
  }
  if (i < (iLim* (-1))) {
    i = iLim * (-1);
  }
  
//dirivative
  d = p - prevP; // theoretical max = 1023
  prevP = p;

//Putting it together
  out = ((p/6)*pK + (i/6)*iK + (d/6)*dK)/100; 
  
  //2 wire motor controller - my janky solution
  if (out < 0) {
    analogWrite(10, out);
    analogWrite(9, 0);
  }
  else {
    analogWrite(9, out);
    analogWrite(10, 0);
  }
  
//debug stuff
  if (debug == 1) {
    cycle = cycle + 1;
    Serial.print(cycle);
    Serial.print(": goal: ");
    Serial.print(goal);
    Serial.print(", pK: ");
    Serial.print(pK);
    Serial.print(", iK: ");
    Serial.print(iK);
    Serial.print(", dK: ");
    Serial.print(dK);
    Serial.print(", pos: ");
    Serial.print(pos);
    Serial.print(", out: ");
    Serial.print(out);
    Serial.print(", p: ");
    Serial.print(p);
    Serial.print(", i: ");
    Serial.print(i);
    Serial.print(", d: ");
    Serial.print(d);
    Serial.print(", dt: ");
    Serial.println(dt);
    dt = millis() - time;
    time = millis();
  }
}
