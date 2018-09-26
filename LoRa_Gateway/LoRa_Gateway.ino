/*
 * LoRa_Gateway.ino
 *
 *  Created on: 25 Septembre 2018
 *      Author: Bassirou NGOM 
 */

#include "SX1276.h"
#include "mqttsnHeader.h"
#include "PubSubClient.h"
#include <Bridge.h>
#include <Console.h>
#include <BridgeClient.h>

/*************** Declartation Des Variables ****************/
#define MAX_DATA 250
#define MAX_TOPICS 10
#define DEBUG 1

#define LORA_MODE 4
#define LORA_CHANNEL CH_10_868
#define LORA_POWER 'H'
#define LORA_ADDR 8

#define TOPIC_DEFAULT "/test"

typedef struct {
        const char* name;
        uint16_t id;
} topic;
topic topic_table[MAX_TOPICS];
uint8_t topic_count=0;
IPAddress server(10, 130, 1, 200); //adresse Broker
BridgeClient net;
PubSubClient client(net);

/***************FIN Declartation ****************/


/***************Declartation Des Fonctions****************/
/*void callback(char* topic, byte* payload, unsigned int length) {
  Console.print("Message arrived [");
  Console.print(topic);
  Console.print("] ");
  for (int i=0;i<length;i++) {
    Console.print((char)payload[i]);
  }
  Console.println();
}*/
String find_topic_name(uint8_t* index);
//void disconnect();
//void connect();
void initRF();
void reconnect() {
  while (!client.connected()) {
    Console.print("Attempting MQTT connection...");
     if (client.connect("MQTTSN_Gateway_Client")) {
      Console.println("connected");
      // Once connected, publish an announcement...
      client.publish("/outTopic","hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Console.print("failed, rc=");
      Console.print(client.state());
      Console.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
/***************FIN Declartation ****************/



void setup() {
  Bridge.begin(115200);
  Console.begin();
  initRF();
  client.setServer(server, 1883);
  initDefaultTopic();
  Console.print("valeur = "); Console.println(0xffff);
}

void loop() {
if (!client.connected()) {
    reconnect();
  }
 if(receive_message()){
  Console.write(sx1276.packet_received.data, sx1276._payloadlength);
  message_header* response_message;
  response_message = (message_header*)sx1276.packet_received.data;
  Console.println(response_message->type);
  if(response_message->type == PUBLISH){
    msg_publish* msg;
    msg = (msg_publish*)sx1276.packet_received.data;
    char* top;
    top = find_topic_name(msg->topic_id);
    Console.print("topic_id: ");Console.println(msg->topic_id);
    Console.print("topic: ");Console.println(top);
    Console.print("data: ");Console.println(msg->data);
    Console.print("msg_id: ");Console.println(msg->message_id);
    //memcpy(msg->topic, top, strlen(top));
    Console.println(msg->data);
    client.publishFromSN(top, msg);
  }
 /* else if(response_message->type == REGISTER){
    msg_register* msg; 
    msg = (msg_register*)sx1276.packet_received.data;
    register_topic(msg->topic_name, msg->topic_id);
    Console.print("topic_id: ");Console.println(msg->topic_id);
    Console.print("topic_name: ");Console.println(msg->topic_name);
    Console.print("length ");Console.println(msg->length);
    Console.print("msg_id: ");Console.println(msg->message_id);
  }*/
 }
 delay(1000);
}



void initRF(){
  
  sx1276.ON();
  #ifdef DEBUG
  Console.println("LoRa On");
  #endif
  sx1276.setMode(LORA_MODE);
  #ifdef DEBUG
  Console.print("loRa setMode : ");
  Console.println(LORA_MODE);
  #endif
  sx1276.setChannel(CH_10_868);
  #ifdef DEBUG
  Console.print("loRa setChannel : ");
  Console.println(CH_10_868);
  #endif
  sx1276.setPower(LORA_POWER);
  #ifdef DEBUG
  Console.print("loRa setPower : ");
  Console.println(LORA_POWER);
  #endif
  sx1276.setNodeAddress(LORA_ADDR);
  #ifdef DEBUG
  Console.print("loRa set node address : ");
  Console.println(LORA_ADDR);
  Console.println("*********************************\n");
  Console.println("SX1276 successfully configured s\n\n");
  #endif
}

char* find_topic_name(uint16_t topic_id) {
  if(topic_id == 0xffff){
      return topic_table[0].name;
    }
  else{
    return topic_table[topic_id].name;
   /* for (uint8_t i = 0; i < 3; ++i) {
          if (topic_table[i].id == topic_id) {
              return topic_table[i].name;
          }
      }
    */

  }
}



boolean receive_message(){
 sx1276.receivePacketTimeout(10000);
  if(sx1276._payloadlength>0){
    Console.println("reception de Donnees");
    return true;
  }
    false;
    //memcpy(response_buffer, (const void*)sx1276.packet_received.data, sx1276._payloadlength);
}



boolean register_topic(const char* name, uint8_t topic_id) {
    Console.println("REGISTER topic");
    if (topic_count < (MAX_TOPICS - 1)) {
        topic_table[topic_count].name = name;
        topic_table[topic_count++].id = topic_id;
        return true;
    }

    return false;
}

void initDefaultTopic(){
  topic t, t2, t1;
  t.name = "/test", t1.name="/test/mqttsn", t2.name="/test/mqttsn/LoRa";
  t.id = 0, t1.id=1, t2.id=2;
  topic_table[0] = t;
  topic_table[1] = t1;
  topic_table[2] = t2;
}

