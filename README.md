# SNMP Environmental Monitor V2.1
Server Room SNMP Temperature Meter
This project was created as a cheaper alternative to climate meters for server rooms.
The system is aimed at using the snmp protocol for temperature control in server racks.
Suitable for interaction with monitoring systems supporting the SNMP protocol such as PRTG network monitor, Zabbix and the like

The button is used to reset the settings. Pressing this button when the device is turned on allows you to reset the settings to default mode.
The default mode is set automatically after the firmware if there was no data in the EEPROM memory.
Default mode means launching the APK of the access point with the address 192.168.4.1 SSID:TERM-SNMP

SNMP Community RO: Public

The SNMP OID consists of a prefix .1.3.6.1.4.1.2020.1. , which can be changed in the settings, next .1 - degrees Celsius .2 - degrees fahrenheit, which is the temperature sensor number. the resulting temperature value must be divided by 100
example: .1.3.6.1.4.1.2020.1.1.2 - displays the temperature in degrees Celsius of the second sensor
The numbering of the sensors is displayed on the first page of the setup 

Supports up to 10 sensors 
