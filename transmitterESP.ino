/*
  by Yaser Ali Husen

  This code for sending data temperature and humidity to postresql database
  code based on pgconsole in SimplePgSQL example code.

*/

#include <WiFi.h>
#include <SimplePgSQL.h>

#define RXp2 16
#define TXp2 17

const int fioSize = 20;
const int timeeSize = 9;

// current data, updated in loop()
int id = 0;
char fio[fioSize] = "sample\0";
int price = 0;
char timee[timeeSize] = "00:00:00";
int left = 5;
// and the data buffer
// size: 4+20+4+9+4
const int databufSize = 41;
char databuf[databufSize] = "";

//Setup connection and Database=====================
const char* ssid = "ssid";
const char* pass = "password";
WiFiClient client;

IPAddress PGIP(xxx, xxx, xxx, xxx);     // your PostgreSQL server IP
const char user[] = "user";       // your database user
const char password[] = "password";   // your database password
const char dbname[] = "databaseName";         // your database name
char buffer[1024];
PGconnection conn(&client, 0, 1024, buffer);
char inbuf[128];
int pg_status = 0;
//===================================================


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 60 seconds (timerDelay variable), it will take 60 seconds before publishing the first reading.");
}

void doPg(void)
{
  char *msg;
  int rc;
  if (!pg_status) {
    conn.setDbLogin(PGIP,
                    user,
                    password,
                    dbname,
                    "utf8");
    pg_status = 1;
    return;
  }

  if (pg_status == 1) {
    rc = conn.status();
    if (rc == CONNECTION_BAD || rc == CONNECTION_NEEDED) {
      char *c = conn.getMessage();
      if (c) Serial.println(c);
      pg_status = -1;
    }
    else if (rc == CONNECTION_OK) {
      pg_status = 2;
      Serial.println("Starting query");
    }
    return;
  }
  
  if (pg_status == 2 && strlen(inbuf) > 0) {
    if (conn.execute(inbuf)) goto error;
    Serial.println("Working...");
    pg_status = 3;
    memset(inbuf, 0, sizeof(inbuf));
  }
  
  if (pg_status == 3) {
    rc = conn.getData();
    int i;
    if (rc < 0) goto error;
    if (!rc) return;
    if (rc & PG_RSTAT_HAVE_COLUMNS) {
      for (i = 0; i < conn.nfields(); i++) {
        if (i) Serial.print(" | ");
        Serial.print(conn.getColumn(i));
      }
      Serial.println("\n==========");
    }
    else if (rc & PG_RSTAT_HAVE_ROW) {
      for (i = 0; i < conn.nfields(); i++) {
        if (i) Serial.print(" | ");
        msg = conn.getValue(i);
        if (!msg) msg = (char *)"NULL";
        Serial.print(msg);
      }
      Serial.println();
    }
    else if (rc & PG_RSTAT_HAVE_SUMMARY) {
      Serial.print("Rows affected: ");
      Serial.println(conn.ntuples());
    }
    else if (rc & PG_RSTAT_HAVE_MESSAGE) {
      msg = conn.getMessage();
      if (msg) Serial.println(msg);
    }
    if (rc & PG_RSTAT_READY) {
      pg_status = 2;
      Serial.println("Waiting query");
    }
  }
  return;
error:
  msg = conn.getMessage();
  if (msg) Serial.println(msg);
  else Serial.println("UNKNOWN ERROR");
  if (conn.status() == CONNECTION_BAD) {
    Serial.println("Connection is bad");
    pg_status = -1;
  }
}


//read Serial2---------------------------------------
// the sequence is the following:
// <int>;<chars>;<int>;<chars>;<int>
void readMessageFromMega() {
  // ";" is the separator
  int i = 0;
  int j = 0;
  int bytesRead;
  char eh[1];

  // reading id
  id = Serial2.parseInt();
  Serial2.readBytes(eh, 1);

  // reading fio
  bytesRead = Serial2.readBytesUntil(';', fio, fioSize);
  fio[bytesRead] = '\0';

  // reading price
  price = Serial2.parseInt();
  Serial2.readBytes(eh, 1);

  // reading time
  bytesRead = Serial2.readBytesUntil(';', timee, timeeSize);
  timee[bytesRead] = '\0';

  // reading how much is left
  left = Serial2.parseInt();
}
//--------------------------------------------------


void loop() {
  delay(50);
  doPg();
  // Checks whether it is time to run the task
  if (Serial2.available()) {
    if (WiFi.status() == WL_CONNECTED) {
      readMessageFromMega();
      //Send data to PostgreSQL
      // here we write to inbuf, then (in the very next iteration) read and execute a query from it
      sprintf(inbuf, "insert into datas (id,fio,price,time,left) values(%d,%s,%d,%s,%d)", id, fio, price, timee, left);      
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    
  }
}
