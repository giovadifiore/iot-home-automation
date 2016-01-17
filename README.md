IoT domestic control and home automation proof-of-concept with NodeJS and C
===================
This project represents a proof-of-concept of an implementation for the domestic control and home automation. The implemented system controls actuators of a development board and intercepts environment events by reading its sensors.

The whole architecture has two main parts:

 - A client implemented in C ([GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded)) which is deployed and running on the development board [STM32F4 Discovery](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF252419?sc=internet/evalboard/product/252419.jsp). This development board features a STM32F407VGT6 SoC that embeds a 32-bit ARM Cortex-M4F core.
 - A server implemented using NodeJS which can be deployed on a centralized compute machine. The central server provides a modern user-friendly Web interface that can be used to interact with the board and leverages on the WebSocket technology to exchange data with its underlying NodeJS server.

In this PoC the two parts communicate exchanging JSON messages using the USB serial protocol.  With a little effort, the communication protocol can be generally adapted to work with a more Network-oriented technology.

----------


Client
-------------

The client running on the board as described above, is capable to intercept user inputs from GPIO and sensors embedded on the STM32F4 package and to produce outputs according to the logic.

The enabled I/O for this PoC are:

 - LIS3DSH 3-axis accelerometer;
 - Temperature sensor;
 - User button;
 - GPIO configured leds;
 - GPIO buzzer;

The accelerometer has been tuned to work as a **door opening** detector. It has, in fact, the capability to detect when the development board executes a rotation along its longitudinal axe. When the door opening event is detected by the client board, an event is sent to the centralized server so that it can optionally perform actions according to the designed logic. In this PoC, when the door opening event happens, the server sends a control message to the client board that triggers the **turning on of a configured led**, simulating the turning on of a room light.

The client, moreover,  periodically sends the **temperature** read from the related sensor to the centralized server. The centralized server updates, then, the Web interface that can be monitored by the user. In conjunction with temperature threshold-related events, the user can configure control actions to be sent to the client board. In this PoC an example of control actions for this scenario could be the activation of the **GPIO buzzer to simulate an alarm**.

The user button on the board is used to simulate asynchronous events to be sent to the centralized server. In this PoC, the user button produces an alert in the Web interface.

This client is implemented in C and uses the HAL library provided by ST Microelectronics. The client also uses the [parson](https://github.com/kgabis/parson) library to handle JSON messages sent between client and server (thanks to [kgabis](https://github.com/kgabis)).

Server
-------------
The server is implemented using NodeJS common libraries and it handles two communication ends: the former to the client board, using the USB serial protocol, and the latter with the Web application, using [WebSocket](http://www.w3.org/TR/2011/WD-websockets-20110419/) standard APIs.

On the client board interface, the server receives JSON messages that need to be interpreted as simple response (i.e. ACK) to control actions, or completely asynchronous signals triggered by environment events (e.g. door opening, temperature hazard, etc... ). The server is always listening for incoming messages that are buffered into a queue: in this way the Web application end (i.e. the WebSocket wrapper on the server-side) can extract them in a decoupled manner without affecting receive performance.

When the user log in the Web application, the server performs a number of pair attempts to the board. The communication protocol executes the **pairing** and then the board recognizes the server as an host to be contacted when events happen. Both server and client mutually recognize to be one end of the communication and the user in the Web application gets notified of the pair complete event, becoming ready to interacts with the system.

Status and future work
-------------
This project is currently not being developed and/or upgraded. This repository contains the working code of a demo that I developed during an exam of my Computer Engineering academic career.

If you need further informations about the project, please contact me in private. 