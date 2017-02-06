
const int w = 20;
int horst[w];
int index = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(7, INPUT);
}



void loop() {
  // put your main code here, to run repeatedly:
  index = (index + 1) % w;
  horst[index] = digitalRead(7); //analogRead(0);
  int sum = 0;
  for (int i = 0; i<w; i++) {
    sum += horst[i];
  }
  Serial.println(sum);
}
