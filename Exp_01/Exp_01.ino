
int DLed=11;
int R=10;
int G=9;
int B=8;
int LM35=A0;
float history[10]; 
int count=0; 
boolean first=true;
void setup() 
{ //this sets the output pins
  Serial.begin(9600);
  pinMode(DLed, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(LM35,INPUT);   
}
void loop()
{
  //int x=analogRead(LM35); 
  //x*= 0.48828125;
  //float tempf  =(tempc * 1.8) + 32;

  // Serial.println(" Value:");Serial.print(tempc);
  // Serial.println(" Value:");Serial.print(tempf);

  float reading=analogRead(LM35);
  float voltage=(reading * 5.0)/1024.0;
  float tempc  =(reading*0.0049)*100;

  if(count<10)  
    history[count++]=tempc;
  else         
    { count=0;  first=false; }

  float avg=getavg(); 
  Serial.println(avg);
  if(avg<20)        digitalWrite(B,HIGH);
  else if(avg<30)   digitalWrite(G,HIGH);
  else              digitalWrite(R,HIGH); 
  int y=map((int)avg, 0, 100, 0, 255);
  Serial.println(y);
  digitalWrite(DLed,y); 
  delay(500);
}

float getavg()
{
  float avg=20;
  if(!first)
  {
    avg=0;
    for (int i=0; i<10; i++)
      avg+=history[i];
    avg/=10;
  }
  return avg;
}

