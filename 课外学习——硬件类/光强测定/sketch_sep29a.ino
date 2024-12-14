void setup() 
{
  Serial.begin(115200);
  pinMode(A0, INPUT);  
  
}
 
void loop() 
{
  int F1 = analogRead(A0);
 
  Serial.println("++++++");
  Serial.println(F1);
  delay(2000);
 
}
