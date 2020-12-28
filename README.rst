.. _blinky-sample:

Zephyr Demo Wifi
######

Overview
********

This application is simple demo how to connect to Wifi, a test connectivity.
It can used with any connected interface (ETHERNET, GPRS, etc ), but not tested.

It was tested with  STM32L4+ Discovery kit IoT node (B-L4S5I-IOT01A) from 
ST Microelectronics.

This sample adds to the console commands, to easy test the connectivity.

Usage
********

Connect a terminal to Zephyr Console, and test with this commands.

.. code-block:: none

	// Connect to Wifi, if not connected
	wifi connect <<SSID>> <<PSK>>
	Connection requested
	Connected

	// Connect to Wifi, if not connected
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
	
	


After a successfull connection it keeps a open socket to send and receive throwght the console.
CTRL + D to disconnect the socket.


Requirements
************

A Zephyr development environment configured 
``https://docs.zephyrproject.org/latest/getting_started/index.html``

A board with IP connectivity, like B-L4S5I-IOT01A properly configured.
For WI-FI autoconnect, the expects a if with the label wifi0.


Building and Running
********************

Build and flash Zephyr Demo Wifi as follows, changing ``b_l4s5i_iot01a`` for your board:
.. zephyr-app-commands::
   :zephyr-app: szephyr-demo-wifi
   :board: b_l4s5i_iot01a
   :goals: build flash
   :compact:

After flashing, you should be able to access the console and test the commands.


