/*
 * wifi_demo.c
 *
 * Base on the official sample by Intel at https://github.com/zephyrproject-rtos/zephyr/tree/master/samples/net/wifi
 *
 * Created on: Dec 27, 2020
 * Author: Joao Carreira
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#define CONFIG_NET_NATIVE_IPV4 1

#include <stdio.h>

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#include <net/wifi.h>
#include <net/net_if.h>
#include <net/net_event.h>
#include <net/net_mgmt.h>
#include <net/wifi_mgmt.h>

#include <string.h>

#include "config.h"


#if AUTO_CONNECT
#ifndef DT_N_NODELABEL_wifi0
#error "Unsupported board: wifi0 is not defined in the devicetree"
#endif

#define perror(fmt, args...)						\
do {									\
	printf("Error: " fmt "(): %d\n" ## args, errno );	\
	while(1); \
} while (0)								\


#define WIFI_MGMT_EVENTS (NET_EVENT_WIFI_SCAN_RESULT |		\
				NET_EVENT_WIFI_SCAN_DONE |		\
				NET_EVENT_WIFI_CONNECT_RESULT |		\
				NET_EVENT_WIFI_DISCONNECT_RESULT)


static struct net_mgmt_event_callback cb;
static struct k_sem net_cb_sem;


static void Wifi_check_connect_result( struct net_if *iface, struct net_mgmt_event_callback *cb)
{
	const struct wifi_status *status = (const struct wifi_status *) cb->info;
	if (!status->status) {
		// Connected
		k_sem_give(&net_cb_sem);
	}
}


void Wifi_event_listener( struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface ) {
	switch( mgmt_event ) {
		case NET_EVENT_WIFI_CONNECT_RESULT:
			Wifi_check_connect_result( iface, cb );
			break;
	}
}

int Wifi_autoconnect() {
	struct wifi_connect_req_params wifi_args;
	wifi_args.security = WIFI_SECURITY_TYPE_PSK;
	wifi_args.channel = WIFI_CHANNEL_ANY;
	wifi_args.psk = AUTO_CONNECT_SSID_PSK;
	wifi_args.psk_length = strlen(AUTO_CONNECT_SSID_PSK);
	wifi_args.ssid = AUTO_CONNECT_SSID;
	wifi_args.ssid_length = strlen(AUTO_CONNECT_SSID);

	// Init semaphore
	k_sem_init(&net_cb_sem, 0, 1);

	// Configure Callback
	net_mgmt_init_event_callback(&cb, Wifi_event_listener, WIFI_MGMT_EVENTS );
	net_mgmt_add_event_callback(&cb);

	// Connect interface to network
	struct net_if *iface = net_if_get_default();
	if( net_mgmt( NET_REQUEST_WIFI_CONNECT, iface, &wifi_args, sizeof(wifi_args) ) ) {
		perror("Failed to request connection to SSID "AUTO_CONNECT_SSID);
	}

	// Wait for connection.....
	k_sem_take(&net_cb_sem, K_FOREVER );

	struct net_if_ipv4 *ipv4 = iface->config.ip.ipv4;
	static char buf[NET_IPV4_ADDR_LEN];
	net_addr_ntop( AF_INET, (const char *)&ipv4->unicast[0].address.in_addr, buf, NET_IPV4_ADDR_LEN);
	printk("\r\nSucessfull connected to SSID:["AUTO_CONNECT_SSID"]\r\nAssigned IP address [%s] \r\n", buf );

	return 0;
}
#endif

void Wifi_demo(void)
{

#if AUTO_CONNECT
	Wifi_init();
#endif

}


