
int LM35=A0;
int photocellReading;
int led = 8;

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 
  192, 168, 1, 177 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 
  192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 
  255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;

void setup() 
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
//-------------------------------------------------------------------
void loop() 
{
  EthernetClient client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {   
      if (client.available()) 
      {
        char c = client.read();  //read char by char HTTP request
        if (readString.length() < 100) //store characters to string          
          readString += c;

        if (c == '\n'){ //if HTTP request has ended
          Serial.println(readString); //print to serial monitor for debuging
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println("Content-Type: text/html");
          client.println();     
          client.println("<HTML>");           
          client.println("<HEAD>");
          client.println("<meta name='WoT Tutorial' content='yes' />");
          client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
          client.println("<TITLE> IoT Control and Monitor Services </TITLE>");
          client.println("</HEAD>");          
          client.println("<BODY>");
          client.println("<H1>    The IoT Layer   </H1><hr />");
          client.println("<H2>Arduino acts as a Web Server</H2>");
          client.println("<br ><a href=\"/?button1on\"\"> LED:ON </a> </br> ");
          client.println("<br ><a href=\"/?button1off\"\"> LED:OFF </a></br><br></br>");
          client.println("<hr />");

          float tempf =(analogRead(LM35) * 100.0) / 1024; 
          Serial.println(tempf);
          float tempc= ((tempf-32)*5)/9;                  
          Serial.println(tempc);
          client.print("<br ><p>Temperature Value: ");
          client.print(int(tempc));              
          client.println(" C = ");
          client.print(int(tempf)); 
          client.println(" F</p></br><hr />"); 
          client.println("</BODY></HTML>");

          delay(1);           
          client.stop(); //stopping client
          //controls the Arduino if you press the buttons
          if (readString.indexOf("?button1on") >0)  digitalWrite(led, HIGH);
          if (readString.indexOf("?button1off") >0) digitalWrite(led, LOW);

          //clearing string for next read
          readString="";  

        }
      }
    }
  }
}


