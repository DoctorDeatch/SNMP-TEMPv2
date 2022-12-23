#if defined (ESP8266)
    #include <ESP8266WiFi.h>        // ESP8266 Core WiFi Library  
    #include <ESP8266WebServer.h>       
#else
    #include <WiFi.h>               // ESP32 Core WiFi Library    
#endif
#include <EEPROM.h>
#include <SNMP_Agent.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Ping.h>
#include <GyverPortal.h>
char* oidSysDescr = ".1.3.6.1.2.1.1.1.0";    // OctetString SysDescr
char* oidSysObjectID = ".1.3.6.1.2.1.1.2.0"; // OctetString SysObjectID
char* oidSysUptime = ".1.3.6.1.2.1.1.3.0";   // TimeTicks sysUptime (hundredths of seconds)
char* oidSysContact = ".1.3.6.1.2.1.1.4.0";  // OctetString SysContact
char* oidSysName = ".1.3.6.1.2.1.1.5.0";     // OctetString SysName
char* oidSysLocation = ".1.3.6.1.2.1.1.6.0"; // OctetString SysLocation
char* oidSysServices = ".1.3.6.1.2.1.1.7.0"; // Integer sysServices
WiFiUDP udp; 
uint32_t uptame, chipId, hour, SysServices;
int n1, n2 , snmp_tc[10], snmp_tf[10],rnd1, countSensors;
byte i, dhcp_on, ap_on, reset_key;
String lpip, name, ping_ret, Sensor, sensorname, power_mode, found_sensors, dhcpl, hostname2, system2, dns2, location2, ip2, oid2, mask2, gw2, ssid2, pass2, contact2, oid_termc2, oid_termf2, community2="public", fw="2.0.3";
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress *sensorsUnique;
char oid_termc1[40], oid_termf1[40], ssid1[16],pass1[16], hostname1[16], community1[16], system1[16], location1[16], contact1[16], oid1[30];
#include "lp.h"
#include "sens.h"
#include "portal.h"
#include "hard_reset.h"
SNMPAgent snmp = SNMPAgent();  
void setup(){
    EEPROM.begin(EEPROM_SIZE);     pinMode(KEY_RESET, INPUT);
    EEPROM.get(1,dhcp_on); EEPROM.get(2,ap_on); EEPROM.get(9,reset_key); 
    if ((dhcp_on==255 && ap_on==255 && reset_key==255) || digitalRead(KEY_RESET)==1) hard_reset();
    EEPROM.get(30,community1); EEPROM.get(90,hostname1); EEPROM.get(150,ssid1); EEPROM.get(210,pass1); EEPROM.get(270,oid1); EEPROM.get(360,contact1); EEPROM.get(390,system1); EEPROM.get(450,location1);
    if (ap_on==0){WiFi.hostname(hostname1); WiFi.begin(ssid1, pass1);}else{WiFi.softAP(ssid1, pass1);}
    if (dhcp_on==0){
IPAddress local_IP(EEPROM.read(10), EEPROM.read(11), EEPROM.read(12), EEPROM.read(13));
IPAddress subnet(EEPROM.read(15), EEPROM.read(16), EEPROM.read(17), EEPROM.read(18));
IPAddress gateway(EEPROM.read(20), EEPROM.read(21), EEPROM.read(22), EEPROM.read(23));
IPAddress primaryDNS(EEPROM.read(25), EEPROM.read(26), EEPROM.read(27), EEPROM.read(28));
IPAddress secondaryDNS(8, 8, 8, 8); 
WiFi.config(local_IP,gateway,subnet,primaryDNS,secondaryDNS);
                    }
    Serial.print("AP IP address: "); delay(1000); Serial.println(WiFi.localIP());
    for(int i=0; i<17; i=i+8) {	  chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i; }
    sensors.begin();
    countSensors = sensors.getDeviceCount();
  found_sensors = "Found sensors: " + String(countSensors);
    sensorsUnique = new DeviceAddress[countSensors];
      if (sensors.isParasitePowerMode()) {
    power_mode = "Mode power is Parasite";
  } else {
    power_mode = "Mode power is Normal";
  }
    portal.attachBuild(build);
    portal.attach(action);
    portal.start();
    portal.log.start(30);
    snmp.setUDP(&udp);
    snmp.begin();
    std::string SysDescr = ESP.getChipModel() + ESP.getChipRevision(); 
    snmp.addReadOnlyStaticStringHandler(oidSysDescr, SysDescr);
    snmp.addReadOnlyStaticStringHandler(oidSysObjectID, "ESP-TERM");
    snmp.addTimestampHandler(oidSysUptime, &uptame);
    snmp.addReadOnlyStaticStringHandler(oidSysContact, contact1);
    snmp.addReadOnlyStaticStringHandler(oidSysName, system1);
    snmp.addReadOnlyStaticStringHandler(oidSysLocation, location1);
  for (int i = 0; i < countSensors; i++) {
    sensors.getAddress(sensorsUnique[i], i);
    sensors.setResolution(sensorsUnique[i], 12);
   oid_termc2= String(oid1)+"1."+String(i); oid_termf2= String(oid1)+"2."+String(i);
   oid_termc2.toCharArray(oid_termc1,40); oid_termf2.toCharArray(oid_termf1,40);
    snmp.addIntegerHandler(oid_termc1,&snmp_tc[i]);
    snmp.addIntegerHandler(oid_termf1,&snmp_tf[i]);
  }
    snmp.sortHandlers();
} 

void loop(){
    snmp.loop(); 
    uptame = millis()/1000;
  portal.tick();
  float temperaturec[10], temperaturef[10];
  sensors.requestTemperatures();
  for (int i = 0; i < countSensors; i++) {
    temperaturec[i] = sensors.getTempCByIndex(i);
    temperaturef[i] = sensors.getTempFByIndex(i);
    snmp_tc[i]=temperaturec[i]*100;
    snmp_tf[i]=temperaturef[i]*100;
  }
}
