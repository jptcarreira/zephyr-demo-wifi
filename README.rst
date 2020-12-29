.. _blinky-sample:

Zephyr Demo Wi-Fi and TCP/UDP connection console
######

Overview
********

This application is simple demo how to connect to Wi-Fi, a  utility to test the connectivity.
It can be used with any board with connected interface (ETHERNET, GPRS, etc ), but not tested.

It was based on the oficial sample
`Wi-Fi Demo <https://docs.zephyrproject.org/latest/samples/net/wifi/README.html>`.

It was tested with ``STM32L4+ Discovery kit IoT`` ``B-L4S5I-IOT01A`` from 
ST Microelectronics.
<https://www.st.com/en/evaluation-tools/b-l4s5i-iot01a.html>

This sample adds to the console commands, to easy test the connectivity.

Usage
********

Connect a terminal to Zephyr Console, and test with this commands.

.. code-block:: none

	// Connect to Wi-Fi, if not connected
	Wi-Fi connect <<SSID>> <<PSK>>
	Connection requested
	Connected

	// Connect to Wi-Fi, if not connected
	>nc tcp <<Ip Adress>> <<Port>>
	CONNECTED. Use CTRL+D to disconnect.
	Hello from my board using TCP!
	Hello from server!
	
	DISCONECTED
	
	>
	
	>nc udp <<Ip Adress>> <<Port>>
	CONNECTED. Use CTRL+D to disconnect.
	Hello from my board using UDP!
	Hello from server!

	DISCONECTED
	
	>

.. image:: https://raw.githubusercontent.com/jptcarreira/zephyr-demo-wifi/master/zephyr-wifi-demo.png


After a successfull connection it keeps a open socket to send and receive throwght the console.
CTRL + D to disconnect the socket.


Requirements
************

A Zephyr development environment configured, see:
`<https://docs.zephyrproject.org/latest/getting_started/index.html>`

A board with IP connectivity, like ``B-L4S5I-IOT01A`` properly configured.
For WI-FI autoconnect, the expects a if with the label ``wifi0``.


Building and Running
********************

Build and flash Zephyr Demo Wifi as follows, changing ``b_l4s5i_iot01a`` to your board:

.. zephyr-app-commands::
   :zephyr-app: szephyr-demo-wifi
   :board: b_l4s5i_iot01a
   :goals: build flash
   :compact:

After flashing, you should be able to access the console and test the commands.


License
************
This repository is open-sourced under the Apache-2.0 license.
For a list of other open-source components included in this repository, see the file 3rd-party-licenses.txt.

