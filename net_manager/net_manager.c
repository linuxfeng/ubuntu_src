/* ************************************************************************
 *       Filename:  net_manager.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年12月09日 17时08分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  fhw, 
 *        Company:  tyxm
 * ************************************************************************/
#include <linux/errno.h>

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/time.h>
#include <linux/ieee80211.h>

#include "net_manager.h"

int totalPackets = 0;

int time(void) {
	struct timeval t1;
    do_gettimeofday(&t1);
    return t1.tv_sec;
}
static unsigned int preRouting(
	unsigned int hooknum,
    struct sk_buff * skb,
    const struct net_device *in,
    const struct net_device *out,
    int (*okfn) (struct sk_buff *))
{
	__be32 sip,dip;
    time_t cur;
	struct iphdr *iph;
    CLIENT_LIST *cli = NULL;
	struct sk_buff *sb = NULL;
	struct tcphdr *thd = NULL;
	struct ieee80211s_hdr *whd;

	sb = skb;
	if(NULL == skb){
		return NF_ACCEPT;
	}
	if(in && (0 == strcmp(in->name, "lo"))){
		return NF_ACCEPT;
	}
	if(!(iph = ip_hdr(sb))){
		return NF_ACCEPT;
	}
	
	sip = iph->saddr;
    dip = iph->daddr;
    cur = time();
	whd = (struct ieee80211s_hdr *)(skb_mac_header(sb));
    totalPackets ++;
	if(0 == sip){
		return NF_ACCEPT;
	}
	if(iph->protocol != IPPROTO_TCP && iph->protocol != IPPROTO_UDP){
		return NF_ACCEPT;
	}
	if(((int*)skb_mac_header(sb))&& iph->protocol == IPPROTO_TCP){
		thd = (struct tcphdr *)((int *) iph + iph->ihl);
		printk("int gate  %d.%d.%d.%d source %d dest %d  %d.%d.%d.%d\n",NIPQUAD(sip),thd->source, thd->dest,NIPQUAD(dip));
		cli = kmalloc(sizeof(CLIENT), GFP_ATOMIC);
		if(NULL == cli){
			return NF_ACCEPT;
		}
		memcpy(cli->client.mac.mac_char.mac, whd->eaddr1,sizeof(cli->client.mac));
		cli->client.ip.int_ip = sip;
		cli->client.time = cur;
		cli->client.port = thd->dest;
		INIT_LIST_HEAD(&(cli->list));
		if(checkNew(cli)){
			kfree(cli);
		}
		else{
			addClient(cli, newClient);
		}
		return NF_ACCEPT;
	}
	printk("I am in preRouting\n");
	return 0;
}
static unsigned int postRouting(
	unsigned int hooknum,
    struct sk_buff * skb,
    const struct net_device *in,
    const struct net_device *out,
    int (*okfn) (struct sk_buff *))
{
	printk("I am in posRouting\n");
	return 0;

}
struct nf_hook_ops preroute_ops = {
    .list =  {NULL,NULL},
    .hook = preRouting,
    .pf = PF_INET,
    .hooknum = NF_INET_POST_ROUTING,
    .priority = NF_IP_PRI_FILTER+2
};

struct nf_hook_ops  postroute_ops = {
    .list =  {NULL,NULL},
    .hook = postRouting,
    .pf = PF_INET,
    .hooknum = NF_INET_LOCAL_IN,
    .priority = NF_IP_PRI_FILTER+2
};

void register_rbmaster_hook(void)
{
	nf_register_hook(&preroute_ops);
	nf_register_hook(&postroute_ops);
	printk("I am in the register\n");
	return ;
}
void unregister_rbmaster_hook(void)
{
	printk("I am in ther unregister\n");
	nf_unregister_hook(&preroute_ops);
    nf_unregister_hook(&postroute_ops);
	return ;
}

int rbmaster_init(void)
{
	printk("hello, you are in the rbmaster_init");
	client_init();
	register_rbmaster_hook();
	return 0;
}

void rbmaster_exit(void)
{
	printk("Hello, this is the exit of rbmaster test module\n");
    unregister_rbmaster_hook();
	client_exit();
	return ;
}

module_init(rbmaster_init);
module_exit(rbmaster_exit);

MODULE_AUTHOR("fhw");
MODULE_DESCRIPTION("netfilter manager");
MODULE_LICENSE("GPL");


