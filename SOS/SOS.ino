/*
  Morse SOS
 */

void lang() {
  digitalWrite(13, HIGH);
  delay(500);           
  digitalWrite(13, LOW);
  delay(250); 
}

void kurz() {
  digitalWrite(13, HIGH);
  delay(200); 
  digitalWrite(13, LOW);
  delay(250); 
}

void s() {
  lang();
  lang();
  lang();
  delay(250);
}

void o() {
  kurz();
  kurz();
  kurz();
  delay(250);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  s();
  o();
  s();
  delay(1000); 
}

