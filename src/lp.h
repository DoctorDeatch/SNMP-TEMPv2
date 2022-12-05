String HL(String lpip) {
ping_ret = "";
delay(300);
IPAddress ip;
ip.fromString(lpip); 
bool ret = Ping.ping(ip);
if(ret == 1){
     float avg_time_ms = Ping.averageTime();
     ping_ret = lpip + " " + avg_time_ms + " mS";
     } else {
     ping_ret = "Request timed out.";
} 
return ping_ret;                   
    }