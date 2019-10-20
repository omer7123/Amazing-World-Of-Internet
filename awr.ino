#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <GSM.h>

#define PINNUMBER ""

#define GPRS_APN       "GPRS_APN" 
#define GPRS_LOGIN     "login"    
#define GPRS_PASSWORD  "password" 


GPRS gprs;
GSM gsmAccess;    
GSMServer server(80); 

const unsigned long __TIMEOUT__ = 10 * 1000;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  while (!Serial) {
    ;  }

  bool notConnected = true;

  while (notConnected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("Connected to GPRS network");

  server.begin();

  IPAddress LocalIP = gprs.getIPAddress();
  Serial.println("Server IP address=");
  Serial.println(LocalIP);
}

void loop() {


  GSMClient client = server.available();



  if (client) {
    while (client.connected()) {
      if (client.available()) {
        Serial.println("Receiving request!");
        bool sendResponse = false;
        while (char c = client.read()) {
          if (c == '\n') {
            sendResponse = true;
          }
        }

        if (sendResponse) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(analogRead(analogChannel));
            client.println("<br />");
          }
          client.println("</html>");
          delay(1000);
          client.stop();
        }
      }
    }
  }
}


