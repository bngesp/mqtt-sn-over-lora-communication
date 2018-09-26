/*
 * LoRa_Gateway.ino
 *
 *  Created on: 25 Septembre 2018
 *      Author: Bassirou NGOM 
 */
#include "SX1276.h"
#include "mqttsnHeader.h"

/*************** Declartation Des Variables ****************/
#define MAX_DATA 250
#define MAX_TOPICS 10
#define LORA_GATEWAY 8

#define LORA_MODE 4
#define LORA_CHANNEL CH_10_868
#define LORA_POWER 'H'
#define LORA_ADDR 3
#define DEBUG 1

#define TOPIC "/bng"


typedef struct {
        const char* name;
        uint16_t id;
} topic;

topic topic_table[MAX_TOPICS];
uint8_t response_buffer[MAX_DATA]; 
uint8_t message_buffer[MAX_DATA];
uint16_t _message_id=0;
uint8_t topic_count=0;

/***************FIN Declartation ****************/


/***************Declartation Des Fonctions****************/

void publish(const uint8_t flags, const uint16_t topic_id, const void* data, const uint8_t data_len);
//void subscribe();
boolean register_topic(const char* name) ;
uint16_t find_topic_id(const char* name, uint8_t* index);
//void disconnect();
//void connect();
void initRF();
void send_message();
void receive_message();
/***************FIN Declartation ****************/



void setup() {
  Serial.begin(115200);
  initRF();
  memset(topic_table, 0, sizeof(topic) * MAX_TOPICS);
  memset(message_buffer, 0, MAX_DATA);
  memset(response_buffer, 0, MAX_DATA);
  

}

void loop() {
  uint16_t topic_id = find_topic_id(TOPIC);
    if(topic_id == 0xffff){
      if(register_topic(TOPIC))
        topic_id = topic_count++;
    }
 char msg[] = "ceci est un message de test";
  Serial.println(topic_id);
 publish(0, topic_id, msg, strlen(msg));


 delay(10000);
}



void initRF(){
  
  sx1276.ON();
  #ifdef DEBUG
  Serial.println("LoRa On");
  #endif
  sx1276.setMode(LORA_MODE);
  #ifdef DEBUG
  Serial.print("loRa setMode : ");
  Serial.println(LORA_MODE);
  #endif
  sx1276.setChannel(CH_10_868);
  #ifdef DEBUG
  Serial.print("loRa setChannel : ");
  Serial.println(CH_10_868);
  #endif
  sx1276.setPower(LORA_POWER);
  #ifdef DEBUG
  Serial.print("loRa setPower : ");
  Serial.println(LORA_POWER);
  #endif
  sx1276.setNodeAddress(LORA_ADDR);
  #ifdef DEBUG
  Serial.print("loRa set node address : ");
  Serial.println(LORA_ADDR);
  Serial.println("*********************************\n");
  Serial.println("SX1276 successfully configured s\n\n");
  #endif
}



void publish(const uint8_t flags, const uint16_t topic_id, const void* data, const uint8_t data_len) {
    ++_message_id;

    msg_publish* msg;
    msg = reinterpret_cast<msg_publish*>(message_buffer);
    msg->length = sizeof(msg_publish) + data_len;
    msg->type = PUBLISH;
    msg->flags = flags;
    msg->topic_id = topic_id;
    msg->message_id = _message_id;
    memcpy(msg->data, data, data_len);
    
    send_message();
    
}

void send_message(){
    message_header* hdr;
    hdr = reinterpret_cast<message_header*>(message_buffer);
    Serial.println("envoi en cours");
    sx1276.sendPacketTimeout(LORA_GATEWAY, message_buffer, hdr->length);
    Serial.println("data envoyés avec succes");
}

uint16_t find_topic_id(const char* name) {
    for (uint8_t i = 0; i < topic_count; ++i) {
        if (strcmp(topic_table[i].name, name) == 0) {
            return topic_table[i].id;
        }
    }

    return 0xffff;
}

boolean register_topic(const char* name) {
    Serial.println("REGISTER topic");
    if (topic_count < (MAX_TOPICS - 1)) {
        topic_table[topic_count].name = name;
        topic_table[topic_count].id = topic_count;
       
       
       /* Envoi du packet REGISTER */
        msg_register* msg; 
        msg = reinterpret_cast<msg_register*>(message_buffer);
        msg->length = sizeof(msg_register) + strlen(name);
        msg->type = REGISTER;
        msg->topic_id = topic_count;
        msg->message_id = _message_id;
        strcpy(msg->topic_name, name);
        //
        send_message();
        return true;
    }

    return false;
}


void receive_message(){
 sx1276.receivePacketTimeout(10000);
  if(sx1276._payloadlength>0)
    Serial.println("reception de Donnees");
    memcpy(response_buffer, (const void*)sx1276.packet_received.data, sx1276._payloadlength);
    
}
