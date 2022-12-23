GyverPortal portal;
String valText;

void build() {
    String chipIde =  String(chipId);
    String hour =  String() + uptame/3600 + " Hour";
    String model = ESP.getChipModel() + ESP.getChipRevision();
    String LocalIP = String() + WiFi.localIP()[0] + "." + WiFi.localIP()[1] + "." + WiFi.localIP()[2] + "." + WiFi.localIP()[3];
    String Localsubnet = String() + WiFi.subnetMask()[0] + "." + WiFi.subnetMask()[1] + "." + WiFi.subnetMask()[2] + "." + WiFi.subnetMask()[3];
    String LocalGW = String() + WiFi.gatewayIP()[0] + "." + WiFi.gatewayIP()[1] + "." + WiFi.gatewayIP()[2] + "." + WiFi.gatewayIP()[3];
    String ip1 = String() + EEPROM.read(10) + "." + EEPROM.read(11) + "." + EEPROM.read(12) + "." + EEPROM.read(13);
    String mask1 = String() + EEPROM.read(15) + "." + EEPROM.read(16) + "." + EEPROM.read(17) + "." + EEPROM.read(18);
    String gw1 = String() + EEPROM.read(20) + "." + EEPROM.read(21) + "." + EEPROM.read(22) + "." + EEPROM.read(23);
    String dns1 = String() + EEPROM.read(25) + "." + EEPROM.read(26) + "." + EEPROM.read(27) + "." + EEPROM.read(28);
    String RSSI = String() + WiFi.RSSI() + "dB";
    String hostname = String(hostname1);

  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  GP.TITLE("SNMP Environmental Monitor");
  GP.NAV_TABS("Info,Network,SNMP,Tools");

  GP.NAV_BLOCK_BEGIN();
    GP_MAKE_BLOCK_TAB("System Info", GP.LABEL("");
    if (dhcp_on==1) dhcpl = "ON"; else dhcpl = "OFF";
    GP_MAKE_BOX(GP.LABEL("Chip model: "); GP.LABEL(model););
    GP_MAKE_BOX(GP.LABEL("Chip ID: "); GP.LABEL(chipIde););
    GP_MAKE_BOX(GP.LABEL("Firmware version: "); GP.LABEL(fw););
    GP_MAKE_BOX(GP.LABEL("Uptime: "); GP.LABEL(hour););
  );
    GP_MAKE_BLOCK_TAB( "Network Info", GP.LABEL("");
    GP_MAKE_BOX(GP.LABEL("MAC:"); GP.LABEL(WiFi.macAddress()););
    GP_MAKE_BOX(GP.LABEL("Host Name:"); GP.LABEL(hostname););
    GP_MAKE_BOX(GP.LABEL("DHCP:"); GP.LABEL(dhcpl););
    GP_MAKE_BOX(GP.LABEL("IP:"); GP.LABEL(LocalIP););
    GP_MAKE_BOX(GP.LABEL("Subnet:"); GP.LABEL(Localsubnet););
    GP_MAKE_BOX(GP.LABEL("GW:"); GP.LABEL(LocalGW););
    GP_MAKE_BOX(GP.LABEL("RSSI:"); GP.LABEL(RSSI););
 );
    GP_MAKE_BLOCK_TAB("Sensors Info", GP.LABEL(power_mode);GP.LABEL(found_sensors);
         float tempc[10]; float tempf[10];
  for (int i = 0; i < countSensors; i++) {
       sensors.requestTemperatures();
  tempc[i] = sensors.getTempCByIndex(i);
  tempf[i] = sensors.getTempFByIndex(i);
 String device = "Device" + String(i);
 String temp1 = String(tempc[i]) + "&degC";
 String temp2 = String(tempf[i]) + "&degF";
 GP_MAKE_BOX(GP.LABEL(device); GP.LABEL(String(temp1)); GP.LABEL(String(temp2)););
  }  );
  GP.NAV_BLOCK_END();

 // GP.NAV_BLOCK_BEGIN();
 //  GP_MAKE_BLOCK_TAB("Device found", GP.LABEL(power_mode);GP.LABEL(found_sensors);
 // for (int i = 0; i < countSensors; i++) {  
 // printAddress(sensorsUnique[i],i);
 // Sensor.toUpperCase();
 // if (sensorname = "") sensorname = "NAME " + String(i);
 //GP_MAKE_BOX(GP.LABEL(Sensor); GP.TEXT("NAME",sensorname,""););
 // }
 // GP.BUTTON_MINI("sensors", "EDIT");
 //  );
 // GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
   GP_MAKE_BLOCK_TAB("Wi-Fi Settings", GP.LABEL("");
    GP_MAKE_BOX(GP.LABEL("AP Mode"); GP.SWITCH("ap", ap_on););
    GP_MAKE_BOX(GP.LABEL("AP name:"); GP.TEXT("ssid2",ssid1,""););
    GP_MAKE_BOX(GP.LABEL("AP pass:"); GP.TEXT("pass2",pass1,""););
    GP_MAKE_BOX(GP.LABEL("DHCP"); GP.SWITCH("dhcp",dhcp_on););
    GP_MAKE_BOX(GP.LABEL("IP address:"); GP.TEXT("ip2",ip1,""););
    GP_MAKE_BOX(GP.LABEL("Mask:"); GP.TEXT("mask2",mask1,""););
    GP_MAKE_BOX(GP.LABEL("Gateway:"); GP.TEXT("gw2",gw1,""););
    GP_MAKE_BOX(GP.LABEL("DNS:"); GP.TEXT("dns2",dns1,""););
    GP_MAKE_BOX(GP.LABEL("Hostname:"); GP.TEXT("hostname2",hostname,""););    
  GP.BUTTON_MINI("wifi", "SAVE & Reboot", "wifi");
   );
  GP.NAV_BLOCK_END();

GP.NAV_BLOCK_BEGIN();
   GP_MAKE_BLOCK_TAB("SNMP Settings", GP.LABEL("");
  //   GP_MAKE_BOX(GP.LABEL("Community RO:"); GP.TEXT("сommunity2",community1,""););
     GP_MAKE_BOX(GP.LABEL("System"); GP.TEXT("system2",system1,""););
     GP_MAKE_BOX(GP.LABEL("location"); GP.TEXT("location2",location1,""););
     GP_MAKE_BOX(GP.LABEL("Contact"); GP.TEXT("contact2",contact1,""););
     GP_MAKE_BOX(GP.LABEL("OID Temp"); GP.TEXT("oid2",oid1,""););
     GP.BUTTON_MINI("snmp", "SAVE");
   );
 GP.NAV_BLOCK_END();

  GP.NAV_BLOCK_BEGIN();
  GP_MAKE_BLOCK_TAB("Ping", GP.LABEL("");
  GP_MAKE_BOX( GP.TEXT("ping", "Destination IP", lpip););
  GP.BUTTON_MINI("ping", "Send Ping" ,"ping");
  GP.AREA_LOG(7);
     );
  GP.NAV_BLOCK_END();
  GP.BUILD_END();
}


void action() {
if (portal.click()){
  if (portal.click("ping"))  {
    portal.log.println(HL(portal.getString("ping")));  }
  if (portal.clickInt("dhcp", dhcp_on)) {
    EEPROM.put(1,dhcp_on);  n2=n2+1;   }
  if (portal.clickInt("ap",ap_on))  {
    EEPROM.put(2,ap_on);  n2=n2+1;  }
  if (portal.clickString("сommunity2",community2))  {
    community2.toCharArray(community1, 16); EEPROM.put(30,community1);   }
  if (portal.clickString("hostname2",hostname2))  {
    hostname2.toCharArray(hostname1, 16); EEPROM.put(90,hostname1);  n2=n2+1;  }
  if (portal.clickString("ssid2",ssid2))  {
    ssid2.toCharArray(ssid1, 16); EEPROM.put(150,ssid1); n2=n2+1;  }
  if (portal.clickString("pass2",pass2))  {
    pass2.toCharArray(pass1, 16); EEPROM.put(210,pass1); n2=n2+1;  }
  if (portal.clickString("ip2",ip2))  {
    ip2 = portal.getString("ip2");  IPAddress ip;  ip.fromString(ip2);  for (int i = 0; i < 4; ++i) { EEPROM.put(10+i,ip[i]); }  n2=n2+1;  }
  if (portal.clickString("mask2",mask2))  {
    IPAddress mask; mask.fromString(mask2); for (int i = 0; i < 4; ++i) { EEPROM.put(15+i,mask[i]); } n2=n2+1;  }
  if (portal.clickString("gw2",gw2))  {
    IPAddress gw; gw.fromString(gw2); for (int i = 0; i < 4; ++i) { EEPROM.put(20+i,gw[i]); }  n2=n2+1;    }
  if (portal.clickString("dns2",gw2))  {
    IPAddress dns; dns.fromString(dns2); for (int i = 0; i < 4; ++i) { EEPROM.put(25+i,dns[i]); } n2=n2+1;   }
  if (portal.clickString("oid2",oid2))  {
    oid2.toCharArray(oid1, 30); EEPROM.put(270,oid1);     }
  if (portal.clickString("contact2",contact2))  {
    contact2.toCharArray(contact1, 16); EEPROM.put(360,contact1);   }
  if (portal.clickString("system2",system2))  {
    system2.toCharArray(system1, 16); EEPROM.put(390,system1);    }
  if (portal.clickString("location2",location2))  {
    location2.toCharArray(location1, 16); EEPROM.put(450,location1);    }
EEPROM.commit();
EEPROM.get(270,oid1); EEPROM.get(360,contact1); EEPROM.get(390,system1); EEPROM.get(450,location1);
if (n2>0) {delay(300); ESP.restart();}
}
}
