/*
 * wifi_demo_shell.c
 *
 *  Created on: Dec 27, 2020
 *      Author: Joao Carreira
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <logging/log.h>
LOG_MODULE_REGISTER(net_demo_shell, LOG_LEVEL_INF);

#include <zephyr.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell/shell.h>
#include <sys/printk.h>
#include <init.h>
#include <fcntl.h>

#include <net/net_if.h>
#include <net/wifi_mgmt.h>
#include <net/net_event.h>
#include <net/socket.h>

#define DEMO_SHELL_MODULE "demo"

#define print(shell, level, fmt, ...)					\
	do {								\
		if (shell) {						\
			shell_fprintf(shell, level, fmt, ##__VA_ARGS__); \
		} else {						\
			printk(fmt, ##__VA_ARGS__);			\
		}							\
	} while (false)


struct cmd_connect_args {
	char * proto;
	char * address;
	uint16_t port;
};

static int __connect_args_to_params(size_t argc, char *argv[],
				struct cmd_connect_args *params)
{
	if (argc < 3) {
		return -EINVAL;
	}

	/* SSID */
	params->proto 		= argv[1];
	params->address 	= argv[2];
	params->port 		= atoi(argv[3]);

	return 0;
}

static int cmd_conect( const struct shell *shell, size_t argc, char *argv[] ) {
	int ret = 0;
	int count;
	char c;
	struct k_poll_signal *signal;
	int set;
	int res;
	int fd;

	struct sockaddr_in addr4;
	struct cmd_connect_args params;
	__connect_args_to_params(argc, argv, &params );

	addr4.sin_family = AF_INET;
	addr4.sin_port = htons(params.port);
	inet_pton(AF_INET, INADDR_ANY, &addr4.sin_addr);

	if( strcmp(params.proto, "udp") == 0 ) {
		 fd = socket(addr4.sin_family, SOCK_DGRAM, IPPROTO_UDP);
		 if (fd < 0) {
			shell_fprintf(shell, SHELL_ERROR, "Failed to create socket");
		 }
		 ret = bind( fd, (const struct sockaddr *)&addr4, sizeof(addr4) );
		 if (fd < 0) {
			shell_fprintf(shell, SHELL_ERROR, "Failed bind socket address");
		 }
	}
	else if ( strcmp(params.proto, "tcp") == 0 ) {
		 fd = socket(addr4.sin_family, SOCK_STREAM, IPPROTO_TCP);
	}
	else {
		shell_fprintf(shell, SHELL_ERROR, "Invalid protocol %s \n", params.proto );
		return -1;
	}

	if (fd < 0) {
		shell_fprintf(shell, SHELL_ERROR, "Failed to create socket");
	}

	// Ignore errors... probably not supported...
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK );

	struct sockaddr_in svr4;
	svr4.sin_family = AF_INET;
	svr4.sin_port = htons(params.port);
	inet_pton(AF_INET, params.address, &svr4.sin_addr );
	ret = connect(fd, (const struct sockaddr*)&svr4, sizeof(svr4));
	if (ret < 0) {
		shell_fprintf(shell, SHELL_ERROR, "Failed to connect to %s://%s:%d \n", params.proto, params.address, params.port);
		return -1;
	}
	shell_info(shell, "CONNECTED. Use CTRL+D to disconnect.\n");
	// Wait for shell events.. like keyboard press
	const char lf = '\n';
	while( 1 ) {
		// Wait for shell events...
		ret = k_poll(shell->ctx->events, SHELL_SIGNAL_TXDONE, K_MSEC(10) );

		// Flush socket buffer...
		do {
			// Clear errno
			errno = 0;
			ret = recv(fd, &c, 1, 0  ) != -1;
			if( ret < 0 || errno == EAGAIN ) {
				break;
			}
			shell_fprintf( shell, SHELL_NORMAL, "%c", c );
		} while( 1 );

		signal = &shell->ctx->signals[ SHELL_SIGNAL_TXDONE ];
		k_poll_signal_check( signal, &set, &res );
		if (set) {
			k_poll_signal_reset(signal);
			if( ret < 0 ) {
				// Some error occurred.....
				break;
			}

			// Read chars...
			(void)shell->iface->api->read(shell->iface, &c,
						      sizeof(c), &count);
			if( count == 0 ) {
				continue;
			}
			switch( c ) {
				case 0x04:
					// Disconnect
					goto end;
				case '\r':
					send(fd, &lf, 1, 0 );
					shell_fprintf( shell, SHELL_NORMAL, "%c", lf );
				default:
					send(fd, &c, 1, 0 );
					shell_fprintf( shell, SHELL_NORMAL, "%c", c );
			}


		}
	}
end:
	close(fd);
	shell_info(shell, "DISCONECTED\n");
	return 0;
}


//SHELL_STATIC_SUBCMD_SET_CREATE(net_commands,
//	SHELL_CMD(tcp, NULL,
//		  "Creates a connection [-p proto]  <IP> <Port>",
//		  cmd_conect),
//	SHELL_SUBCMD_SET_END
//);

SHELL_CMD_REGISTER(nc, NULL, "Connection TCP/UDP commands", cmd_conect );

static int demo_shell_init(const struct device *unused)
{
	ARG_UNUSED(unused);
	return 0;
}

SYS_INIT(demo_shell_init, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
