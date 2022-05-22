#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "example"                                            //change this
const char AWS_IOT_ENDPOINT[] = "example.iot.us-west-1.amazonaws.com"; // change this
char ssid[] = "yourssid";    // your network SSID (name) 
char pass[] = "yourpwd"; // your network password (use for WPA, or use as key for WEP)

// Amazon Root CA 1                                               //change this
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                             //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";
 
// Device Private Key                                             //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
 
 
)KEY";