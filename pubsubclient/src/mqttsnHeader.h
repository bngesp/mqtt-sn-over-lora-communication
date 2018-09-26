/*
mqttsn.h

The MIT License (MIT)


*/

#ifndef __MQTTSN_H__
#define __MQTTSN_H__



enum return_code_t {
    ACCEPTED,
    REJECTED_CONGESTION,
    REJECTED_INVALID_TOPIC_ID,
    REJECTED_NOT_SUPPORTED
};

enum message_type {
    ADVERTISE,
    SEARCHGW,
    GWINFO,
    CONNECT = 0x04,
    CONNACK,
    WILLTOPICREQ,
    WILLTOPIC,
    WILLMSGREQ,
    WILLMSG,
    REGISTER,
    REGACK,
    PUBLISH,
    PUBACK,
    PUBCOMP,
    PUBREC,
    PUBREL,
    SUBSCRIBE = 0x12,
    SUBACK,
    UNSUBSCRIBE,
    UNSUBACK,
    PINGREQ,
    PINGRESP,
    DISCONNECT,
    WILLTOPICUPD = 0x1a,
    WILLTOPICRESP,
    WILLMSGUPD,
    WILLMSGRESP
};

struct __attribute__ ((packed)) message_header {
    uint8_t length;
    uint8_t type;
};

/*struct __attribute__ ((packed)) msg_advertise : public message_header {
    uint8_t gw_id;
    uint16_t duration;
};

struct __attribute__ ((packed)) msg_searchgw : public message_header {
    uint8_t radius;
};

struct __attribute__ ((packed)) msg_gwinfo : public message_header {
    uint8_t gw_id;
    char gw_add[0];
};

struct __attribute__ ((packed)) msg_connect : public message_header {
    uint8_t flags;
    uint8_t protocol_id;
    uint16_t duration;
    char client_id[0];
};

struct __attribute__ ((packed)) msg_connack : public message_header {
    return_code_t return_code;
};

struct __attribute__ ((packed)) msg_willtopic : public message_header {
    uint8_t flags;
    char will_topic[0];
};

struct __attribute__ ((packed)) msg_willmsg : public message_header {
    char willmsg[0];
};*/

struct __attribute__ ((packed)) msg_register : public message_header {
    uint16_t topic_id;
    uint16_t message_id;
    char topic_name[0];
};

/*struct __attribute__ ((packed)) msg_regack : public message_header {
    uint16_t topic_id;
    uint16_t message_id;
    uint8_t return_code;
};*/

struct __attribute__ ((packed)) msg_publish : public message_header {
    uint8_t flags;
    uint16_t topic_id;
    uint16_t message_id;
    char data[0];
    char topic[0];
};

struct __attribute__ ((packed)) msg_puback : public message_header {
    uint16_t topic_id;
    uint16_t message_id;
    uint8_t return_code;
};

/*struct __attribute__ ((packed)) msg_pubqos2 : public message_header {
    uint16_t message_id;
};

struct __attribute__ ((packed)) msg_subscribe : public message_header {
    uint8_t flags;
    uint16_t message_id;
    union {
        char topic_name[0];
        uint16_t topic_id;
    };
};

struct __attribute__ ((packed)) msg_suback : public message_header {
    uint8_t flags;
    uint16_t topic_id;
    uint16_t message_id;
    uint8_t return_code;
};

struct __attribute__ ((packed)) msg_unsubscribe : public message_header {
    uint8_t flags;
    uint16_t message_id;
    union {
        char topic_name[0];
        uint16_t topic_id;
    };
};

struct __attribute__ ((packed)) msg_unsuback : public message_header {
    uint16_t message_id;
};

struct __attribute__ ((packed)) msg_pingreq : public message_header {
    char client_id[0];
};

struct __attribute__ ((packed)) msg_disconnect : public message_header {
    uint16_t duration;
};

struct __attribute__ ((packed)) msg_willtopicresp : public message_header {
    uint8_t return_code;
};

struct __attribute__ ((packed)) msg_willmsgresp : public message_header {
    uint8_t return_code;
};*/

#endif
