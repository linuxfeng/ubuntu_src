
#include "net_manager.h"

struct list_head *newClient = NULL;

void client_init(void){
	newClient = kmalloc(sizeof(struct list_head), GFP_ATOMIC);
	INIT_LIST_HEAD(newClient);
	return;
}
CLIENT_LIST * findNode(CLIENT_LIST *target){
	CLIENT_LIST *tmp = NULL;
	if(NULL == target || (list_empty(&(target->list)))){
		return NULL;
	}
	list_for_each_entry(tmp, newClient, list){
		if(tmp->client.port == target->client.port){
			return tmp;
		}
	}
	return NULL;
}
int updateDate(CLIENT_LIST  *linkData, CLIENT_LIST *newData){
	memcpy(linkData->client.mac.mac_char.mac, newData->client.mac.mac_char.mac,sizeof(newData->client.mac.mac_char.mac)); linkData->client.ip.int_ip = newData->client.ip.int_ip;
	linkData->client.time = newData->client.time;
	linkData->clidnt.port = newData->client.port;
	return FUN_OK;
}
int checkNew(CLIENT_LIST *client){
	CLIENT_LIST *tmp = NULL;
	if(NULL == client)
		return 0;
	if(tmp = findNod(client)){
		if(tmp->client.ip.int_ip != client->client.ip.int_ip || \
				0 != strcmp(tmp->client.mac.mac_char.mac, client->client.mac.mac_char.mac)){
			updateDate(tmp, client);
		}
		return 1;

	}
	return 0;
}

int addClient(CLIENT_LIST *client, struct list_head *newClientList){
	if(NULL == newClientList){
		return 0;
	}
	list_add(&(client->list), newClientList);
	return 0;
}

int client_exit(){
	for(){

	}
	return 0;
}

