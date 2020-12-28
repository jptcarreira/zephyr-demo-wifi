/*
 * config.h
 *
 *  Created on: Dec 27, 2020
 *      Author: Joao Carreira
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

// Auto join 0 - Disabled, 1 - Enable
#define AUTO_CONNECT  0

// SSID
#define AUTO_CONNECT_SSID 		"<<SSID>>"

// Password
#define AUTO_CONNECT_SSID_PSK	"<<PSK>>"

//Security type WIFI_SECURITY_TYPE_NONE = 0, WIFI_SECURITY_TYPE_PSK = 1
#define SSID_SECURITY	WIFI_SECURITY_TYPE_PSK

#endif /* SRC_CONFIG_H_ */
