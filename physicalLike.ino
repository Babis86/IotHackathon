// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

// This #include statement was automatically added by the Particle IDE.
#include "InternetButton/InternetButton.h"

//This Script based on the MQTT-Example in the Photon-Libary of the Photon-WEB-IDE
//Particle-WEB-IDE: https://login.particle.io/build

#define LEDGR0_1 11 //LED #11
#define LEDGR0_2 1 //LED #1
#define LEDGR1_1 2 //LED #2
#define LEDGR1_2 4 //LED #4
#define LEDGR2_1 5 //LED #5
#define LEDGR2_2 7 //LED #7

void callback(char* topic, byte* payload, unsigned int length);

char *IOT_CLIENT = "d:<yourOrgUnit>:<yourDeviceType>:<yourDeviceName>"; //Device2
char *IOT_HOST = "<yourOrgUnit>.messaging.internetofthings.ibmcloud.com";  
char *IOT_PASSWORD = "<yourDeviceAPIKey>"; //Device2
char *IOT_PUBLISH = "iot-2/evt/input/fmt/json";  
char *IOT_USERNAME = "use-token-auth";
InternetButton b = InternetButton();
InternetButton c = InternetButton(); 
int state=0;
String msgBuffer = NULL;

//MQTT-Connection
MQTT client(IOT_HOST, 1883, callback);

//Connection-Method
void iotConnection(){
    //Connect Photon to Bluemix
    client.connect(IOT_CLIENT, IOT_USERNAME, IOT_PASSWORD);
    if (client.isConnected()) {
        //client.publish(IOT_PUBLISH, "{\"device\”: \"pauseState\"}"); //Device1
        client.publish(IOT_PUBLISH, "{\"device\”: \"physicalLike\"}"); //Device2
        if(msgBuffer != NULL){
            client.publish(IOT_PUBLISH, msgBuffer);
        }
        plog("Connection established...");
    }
}

//For debugging -> https://console.particle.io/logs
void plog (String msg){
    Particle.publish(msg);
}

//Recieve message -> if your want to handle incoming messages
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);
}

//Start-LED-Effect
void initLED(int duration){
    RGB.control(true);
    for(int i=1; i<=12;i++){
        b.ledOn(i, i+i, i*2-i, i*2-2+i*3);
        delay(i*10);
    }
    delay(1000);
    for(int i=1; i<=255;i++){
        delay(2);
        b.setBrightness(i);
    }
    delay(1000);
    stateLEDOff();
}

//Setup-Method
void setup() {
    b.begin();
    //Set Pins
    pinMode(D0, OUTPUT);
    //LEDs
    initLED(500);
    // connect to the server
    iotConnection();
    plog("Init passed!");
    b.setBrightness(10);
}

//Msg-Publish-Method
void publish(String msg, int ledgroup, int wait){
    if (client.isConnected()){
        if(state==0){
            digitalWrite(D0, HIGH);
            msgBuffer = "{\"button\": \""+msg+"\"}";
            client.publish(IOT_PUBLISH, msgBuffer);
            switch(ledgroup){
                case 0:
                    b.ledOn(LEDGR0_1, 255, 0, 0);
                    b.ledOn(LEDGR0_2, 255, 0, 0);
                    RGB.color(255, 0, 0);
                    plog(msg+"-Button pressed!");
                    break;
                case 1:
                    b.ledOn(LEDGR1_1, 0, 0, 255);
                    b.ledOn(LEDGR1_2, 0, 0, 255);
                    RGB.color(0, 255, 0);
                    plog(msg+"-Button pressed!");
                    break;    
                case 2:
                    b.setBrightness(200);
                    b.ledOn(LEDGR2_1, 0, 255, 0);
                    b.ledOn(LEDGR2_2, 0, 255, 0);
                    RGB.color(0, 0, 255);
                    plog(msg+"-Button pressed!");
                    break;
                default:
                    plog(msg+"-Button pressed!");
                    plog("undefined buttonAction!");
                    RGB.color(255, 255, 255);
                    delay(100);
                    RGB.color(0, 0, 0);
                    delay(100);
                    RGB.color(255, 255, 255);
                    delay(100);
                    RGB.color(0, 0, 0);
                    delay(100);
                    RGB.color(255, 255, 255);
                    delay(100);
                    break;
            }
            state = 1;
        }
        client.loop();
    }
    delay(wait);
}
//LED-Blinking in IDLE-Mode
void stateLED(){
    b.ledOn(6, 255,255,255);
    delay(200);
    b.ledOff(6);
    delay(200);
}

//Switch-Off all LEDs
void stateLEDOff(){
    b.ledOff(LEDGR0_1);
    b.ledOff(LEDGR0_2);
    b.ledOff(LEDGR1_1);
    b.ledOff(LEDGR1_2);
    b.ledOff(LEDGR2_1);
    b.ledOff(LEDGR2_2);
    RGB.color(0, 0, 0);
    digitalWrite(D0, LOW);
}

//Waittime for Buttons -> Delay between 2 clicks
void stateDelayer(int wait){
    delay(wait);
    state = 0;
    delay(wait);
}

//Main-Loop
void loop() {
    if (client.isConnected()==false){
        //Hint: The Connection to IBM Bluemix will beclosed after few iterations (Default MQTT-Library-Settings)
        //This Methods keeps the Connection "alive"
        iotConnection();
        //Sending last Msg, if Connection in past iteration was lost _> handling in iotConnection()
        msgBuffer=NULL;
    }
    stateDelayer(200);
    if (c.buttonOn(1)){
        //publish("wc", 0, 200);//Device1
        publish("dislike", 0, 200);//Device2

    /*Unused in Device2
    }else if(c.buttonOn(2)){
        publish("coffee", 1, 200);//Device1
    */
    }else if(c.buttonOn(3)){
        //publish("smoke", 2, 200);//Device1
        publish("like", 2, 200);//Device2

    /*}else if(c.buttonOn(4)){
        publish("Tada", 2, 200);*/
    }

    else{
        stateLEDOff();
    }
    stateLED();

}
