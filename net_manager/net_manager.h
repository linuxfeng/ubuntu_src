#ifndef __NET_MANAGER_H__
#define __NET_MANAGER_H__ 

#include <linux/time.h>

#define WLANDEVICE ("wl0.1")
#define FUN_OK 0   //
#define FUN_ERR -1
#define FUN_ORTHER 1
#define NIPQUAD(addr) \
  ((unsigned char *)&addr)[0], \
  ((unsigned char *)&addr)[1], \
  ((unsigned char *)&addr)[2], \
  ((unsigned char *)&addr)[3]

typedef union {
	int int_ip;
    char char_ip[4];
} IPADR;

struct mac_int {
	int mac1;
	short mac2;
};
struct mac_char {
	char mac[6];
};
typedef union {
	struct mac_int mac_int;
    struct mac_char mac_char;
} MACADR;

typedef struct {
	IPADR ip;
    MACADR mac;
    time_t time;
	int port;
} CLIENT;

typedef struct client_list{
	struct list_head list;
	CLIENT client;
} CLIENT_LIST;

extern struct list_head *newClient;



#endif 
