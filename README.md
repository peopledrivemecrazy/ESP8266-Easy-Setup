# ESP8266-Easy-Setup
Easy and simple barebones (hardcoded) ESP8266 setup for mDNS, LLMNR and Pubsub mqtt client. Just insert the ssid, wifi creds and mqtt broker ip/hostname 

# Libraries Used
This code is using these libraries
<br>
`#include <PubSubClient.h>`<br>
`#include <ESP8266LLMNR.h>`<br>
`#include <ESP8266mDNS.h>`<br>

# LLMNR and mDNS
LLMNR and mDNS is used for address resolution computers can resolve the ip address for the esp8266 just by navigating to the hostname

`Mac/Apple devices` can go to http://esp8266.local<br>
`Windows` can go to http://esp8266 users can also choose to install [bonjour](https://support.apple.com/kb/DL999?viewlocale=en_US&locale=en_US) or iTunes and can visit http://esp8266.local<br>
`Linux` users have to install [avahi](http://avahi.org) daemon and will be able to load http://esp8266.local


# Initial setup and instructions
**Edit only this code block**
```
const char* ssid = "..."; // Enter your wifi ssid
const char* password = "..."; // Enter your wifi password
const char* mqtt_server = "..."; // Enter your mqtt broker ip or hostname
const char* host_name = "esp8266"; // Dedicated hostname if need be
```
**You can use this code as a template and if you are using mqtt _AVOID_ using delays**

# Special options
You can initiate multiple servers at different port

```c
ESP8266WebServer secret(81);
 ```
 Anything that is inside ```secret.on ``` will be only visible only at http://esp8266.local:81 and will return `Pssst..!, you are on a secret page`
 
# mDNS and LLMNR
```c
MDNS.begin(host_name) && LLMNR.begin(host_name)
```
You can entirely use different hostname to visit pages on different ports, for example the normal webserver at 80 can be visited at myesp8266.local and webserver at port 81 can be visited at icefrog.local change code to 

```c
MDNS.begin("myesp8266") && LLMNR.begin(icefrog)
```


# Future of this repo

- [x] Clean the code
- [ ] Make repo cleaner :feelsgood:
- [ ] Add [EasyDDNS](https://github.com/ayushsharma82/EasyDDNS)
- [ ] Add extra functionalities
