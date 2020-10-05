
 #include <ArduinoHttpClient.h>

#define TINY_GSM_DEBUG Serial
#define CAYENNE_PRINT Serial
#define TINY_GSM_MODEM_SIM7000

#define USE_GSM //! Uncomment will use SIM7000 for GSM communication

#ifdef USE_GSM
#include <CayenneMQTTGSM.h>
#else
#include <CayenneMQTTESP32.h>
#endif
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define TEMPERATURE_VIRTUAL_CHANNEL 1
#define BAROMETER_VIRTUAL_CHANNEL 2
#define ALTITUDE_VIRTUAL_CHANNEL 3
#define BATTERY_VIRTUAL_CHANNEL 4
#define SOLAR_VIRTUAL_CHANNEL 5
#define LIGHTSENSOR_VIRTUAL_CHANNEL 6

#define PIN_TX 27
#define PIN_RX 26
#define UART_BAUD 115200
#define PWR_PIN 4
#define BAT_ADC 35
#define SOLAR_ADC 36


//for google script Things to change
//const char * ssid = "";
//const char * password = "";

//https://script.google.com/macros/s/AKfycbylrRpSX7SUPJzZfElazwG_BRp7yafTgLRy5vwXbtqaCNFSXGg/exec


String GOOGLE_SCRIPT_ID = "AKfycbylrRpSX7SUPJzZfElazwG_BRp7yafTgLRy5vwXbtqaCNFSXGg"; // Replace by your GAS service id

const int sendInterval = 996 *5; // in millis, 996 instead of 1000 is adjustment, with 1000 it jumps ahead a minute every 3-4 hours

//------------- 



//updated 04.12.2019
const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
"-----END CERTIFICATE-----\n";


WiFiClientSecure client;




Adafruit_BMP085 bmp;
HardwareSerial gsmSerial(1);
  
#ifdef USE_GSM
// GSM connection info.
char apn[] = "";          // Access point name. Leave empty if it is not needed.
char gprsLogin[] = "";    // GPRS username. Leave empty if it is not needed.
char gprsPassword[] = ""; // GPRS password. Leave empty if it is not needed.
char pin[] = "";          // SIM pin number. Leave empty if it is not needed.
#else
// WiFi network info.
char ssid[] = "your wifi ssid";
char wifiPassword[] = "your wifi password";
#endif

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "dbd718c0-0afd-11e7-a044-09cb7c7beef1";
char password[] = "b311142229b601732d6cda7fdef1f2e9be49c185";
char clientID[] = "b313b040-fbf6-11ea-93bf-d33a96695544";

bool bmpSensorDetected = true;

void setup()
{
    Serial.begin(UART_BAUD);
    gsmSerial.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

    pinMode(PWR_PIN, OUTPUT);

    //Launch SIM7000
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);

    // Launch BMP085
    // if (!bmp.begin())
    // {
    //     bmpSensorDetected = false;
    //     Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    //     while (1)
    //     {
    //     }
    // }

    //Wait for the SIM7000 communication to be normal, and will quit when receiving any byte
    int i = 6;
    delay(200);
    while (i)
    {
        Serial.println("Send AT");
        gsmSerial.println("AT");
        if (gsmSerial.available())
        {
            String r = gsmSerial.readString();
            Serial.println(r);
            break;
        }
        delay(1000);
        i--;
    }

#ifdef USE_GSM
    Cayenne.begin(username, password, clientID, gsmSerial, apn, gprsLogin, gprsPassword, pin);
#else
    Cayenne.begin(username, password, clientID, ssid, wifiPassword);
#endif


Serial.println("Ready to go");

}



float getFakeTemperature() {
return micros()%20;
}

String fakeFunc1()
{
  return "somedata";
}
float fakeFunc2()
{
  return millis()%100;
}







void loop()
{
    Cayenne.loop();
 
 
 delay(2000);
 sendData("info1=" + fakeFunc1()+"&info2="+String(fakeFunc2())+"&temp="+String(getFakeTemperature()));
   

}



void sendData(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   Serial.print(url);
    Serial.print("Making a request");
    http.begin(url, root_ca); //Specify the URL and certificate
    int httpCode = http.GET();  
    http.end();
    Serial.println(": done "+httpCode);
}







// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
    CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
    //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

CAYENNE_IN(1)
{
    CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
}

// This function is called at intervals to send temperature sensor data to Cayenne.
CAYENNE_OUT(TEMPERATURE_VIRTUAL_CHANNEL)
{

    if (bmpSensorDetected)
    {
        float temperature = random(100) ;// bmp.readTemperature();

        Serial.print("Temperature = ");
        Serial.print(temperature);
        Serial.println(" *C");

        Cayenne.celsiusWrite(TEMPERATURE_VIRTUAL_CHANNEL, temperature);
    }

 //delay(2000);
// sendData("info1=" + fakeFunc1()+"&info2="+String(fakeFunc2())+"&temp="+String(getFakeTemperature()));
   

}

// This function is called at intervals to send barometer sensor data to Cayenne.
CAYENNE_OUT(BAROMETER_VIRTUAL_CHANNEL)
{
    if (bmpSensorDetected)
    {
        float pressure = random(100) ;// bmp.readPressure() / 1000;

        Serial.print("Pressure = ");
        Serial.print(pressure);
        Serial.println(" hPa");

        Cayenne.hectoPascalWrite(BAROMETER_VIRTUAL_CHANNEL, pressure);
    }
}

CAYENNE_OUT(ALTITUDE_VIRTUAL_CHANNEL)
{
    if (bmpSensorDetected)
    {
        float altitude = random(100) ;// bmp.readAltitude();

        Serial.print("Altitude = ");
        Serial.print(altitude);
        Serial.println(" meters");

        Cayenne.virtualWrite(ALTITUDE_VIRTUAL_CHANNEL, altitude, "meters", UNIT_METER);
    }
}

float readBattery(uint8_t pin)
{
    int vref = 1100;
    uint16_t volt = analogRead(pin);
    float battery_voltage = ((float)volt / 4095.0) * 2.0 * 3.3 * (vref);
    return battery_voltage;
}

CAYENNE_OUT(BATTERY_VIRTUAL_CHANNEL)
{
    float mv = readBattery(BAT_ADC);
    Serial.printf("batter : %f\n", mv);
    Cayenne.virtualWrite(BATTERY_VIRTUAL_CHANNEL, mv, TYPE_VOLTAGE, UNIT_MILLIVOLTS);
}

CAYENNE_OUT(SOLAR_VIRTUAL_CHANNEL)
{
    float mv = readBattery(SOLAR_ADC);
    Serial.printf("solar : %f\n", mv);
    Cayenne.virtualWrite(SOLAR_VIRTUAL_CHANNEL, mv, TYPE_VOLTAGE, UNIT_MILLIVOLTS);
}
