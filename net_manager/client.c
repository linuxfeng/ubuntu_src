
#include "net_manager.h"
#include <linux/skbuff.h>
struct list_head *newClient = NULL;

void client_init(void){
	newClient = kmalloc(sizeof(struct list_head), GFP_ATOMIC);
	INIT_LIST_HEAD(newClient);
	return;
}
EXPORT_SYMBOL(client_init);

CLIENT_LIST * findNode(CLIENT_LIST *target){
	CLIENT_LIST *tmp = NULL;
	if(NULL == target || (list_empty(&(target->list)))){
		return NULL;
	}
	list_for_each_entry(tmp, newClient, list){
		if(tmp->client.port == target->client.port){
			printk("tmp-port=[%d],target-port=[%d]\n",tmp->client.port,target->client.port);
			return tmp;
		}
	}
	return NULL;
}

int updateDate(CLIENT_LIST  *linkData, CLIENT_LIST *newData){
	memcpy(linkData->client.mac.mac_char.mac, newData->client.mac.mac_char.mac,sizeof(newData->client.mac.mac_char.mac)); linkData->client.ip.int_ip = newData->client.ip.int_ip;
	linkData->client.time = newData->client.time;
	linkData->client.port = newData->client.port;
	return FUN_OK;
}
//EXPORT_SYMBOL(updateDate);
/* return 1 : this is a old 
 * return 0 : this is a new 
 * */
int checkNew(CLIENT_LIST *client){
	CLIENT_LIST *tmp = NULL;
	if(NULL == client)
		return 0;
	tmp = findNode(client);
	if(NULL == tmp){
		return 0;
	}
	else{
		if(tmp->client.ip.int_ip != client->client.ip.int_ip || \
				0 != strcmp(tmp->client.mac.mac_char.mac, client->client.mac.mac_char.mac)){
			updateDate(tmp, client);
		}
		return 1;

	}
}
EXPORT_SYMBOL(checkNew);

int addClient(CLIENT_LIST *client, struct list_head *newClientList){
	if(NULL == newClientList){
		return 0;
	}
	list_add(&(client->list), newClientList);
	return 0;
}
EXPORT_SYMBOL(addClient);
void  client_exit(void){
	struct list_head *tmp = NULL;
	struct list_head *tmp_node = NULL;
	if(list_empty(newClient)){
		printk("I am null\n");
		return ;
	}
	tmp = newClient->next;
	while(tmp != newClient){
		printk("client=[%d]\n",list_entry(tmp,CLIENT_LIST, list)->client.port);
		tmp_node = tmp;
		tmp = tmp->next;
		list_del(tmp_node);
		kfree(tmp_node);
	}
	return ;
}
EXPORT_SYMBOL(client_exit);
