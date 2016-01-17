/**
 * 
 * 
 * Main server
 * 
 */

// imports
var nodeSerialport = require("serialport"),
	webserver	= require('./webserver'),
	WebSocketServer = require('ws').Server,
	BoardCommunication = require('./board-communication'),
	cp = require('child_process'),
	sleep = require('sleep');

//configure the logger
var log4js = require('log4js');
log4js.configure('log4js.json');
var logger = log4js.getLogger(require('path').basename(__filename));

// start the web server
webserver.startServer(__dirname+'/web/views', __dirname+'/web/static');

// start the WebSocket with the user interface (browser)
var wss = new WebSocketServer({server: webserver.getInstance()});

// start the usb-watcher process that detects usb devices
var boardWatcher = cp.fork(__dirname + '/usb-watcher.js');

// instantiate a new BoardCommunication object (this way of instantiate the object should be revised if more than one board is connected)
// in this demo-project, we will suppose that only one board at time is connected to the host
var board = new BoardCommunication();

/* define handlers for the communication -------------------------------------------- */
// async signals handler
function __asynchSignalsHandler(signal)
{
	if (signal.target==="USER")
	{
		// if this signal has "USER" as a target, broadcast to all users
		wss.broadcast(signal);
	}
}

// handles the connection close (this handler notifies the usb-watcher process when the board is not connected anymore)
function __connectionCloseHandle(device)
{
	var message = {
		cmd: "DEVICE_CONNECTION_CLOSE",
		device: device
	};
	boardWatcher.send(message);
	
	var signal = {
	    target : "USER",
		signal : "SIG_DEVICE_DISCONNECTED",
		message: {
			device: device
		}
	};
	
	// notify all users that the device is disconnected
	wss.broadcast(signal);
}

// handles the open event (notifies users connected)
function __connectionOpenHandle(device)
{
	var signal = {
	    target : "USER",
		signal : "SIG_DEVICE_CONNECTED",
		message: {
			device: device
		}
	};
	
	logger.info("Notify all connected users about the new device");
	
	// notify all users that the device is disconnected
	wss.broadcast(signal);
}


/* communication with the usb-watcher process --------------------------------------- */
// listen messages from the usb-watcher
boardWatcher.on('message', function(m) {
	
	// we got a new device connected
	if (m.cmd === "DEVICE_DETECTED")
	{
		logger.debug("New device detection received from the child process");
		board.connect(m.device, __asynchSignalsHandler, __connectionOpenHandle, __connectionCloseHandle);
	}
});


/* communication with the user (browser) -------------------------------------------- */
function __onWebSocketMessageReceive(socket, packet)
{	
	// the message received from the user (browser) is always serialized
	var object = JSON.parse(packet);
	
	// check the target and in case forward the packet to the board
	if (object.target==="BOARD")
	{
		// when receiving response from the board, return data into a serialized_string
		var opts = {
			callbackPacketType: 'json_serialized_string'
		};
		
		// send the packet to the board
		board.sendSerializedPacket(packet, function(responsePacket) {
			
			// send the response back to the user (browser)
			socket.send(responsePacket);
			
		}, opts);
	}
	else if (object.target==="HOST")
	{
		// handle this packet and don't send it to the board
		if (object.cmd==="CMD_GET_CONNECTED_DEVICES")
		{
			var devices = [];
			
			// only one device managed
			var device = board.getConnectedDevice();
			
			if (false !== device)
			{
				devices.push(device);
			}
			
			var responsePacket = {
			    target: "USER",
				cmd_reply: "CONNECTED_DEVICES",
				message: {
					devices: devices
				}
			};
			
			// send message
			var serialized_packet = JSON.stringify(responsePacket);
			logger.debug("Sending packet to the user: %s", serialized_packet);
			socket.send(serialized_packet);
		}
	}
}

// attach the broadcast handler
wss.broadcast = function broadcast(data) {
  wss.clients.forEach(function each(client) {
	// the client must receive always data in serialized json strings
    client.send(JSON.stringify(data));
  });
};

// attach the connection handler
wss.on('connection', function (socket) {
	logger.debug("User WebSocket opened on %s:%s", socket.upgradeReq.connection.remoteAddress, socket.upgradeReq.connection.remotePort);
	// attach an handler for the message receive event
	socket.on('message', function(message){
		__onWebSocketMessageReceive(socket, message);
	});
});


/* handlers for processes exit ------------------------------------------------------- */
// attach handlers for process's exit
boardWatcher.on('exit', function(code, signal) {
	logger.warn("The usb-watcher process exited with code %s and signal %s", code, signal);
});

// close connection on process's exit
process.on('exit', function(code) {
	boardWatcher.disconnect();
	logger.debug("usb-watcher disconnected");
	boardWatcher.kill();
	logger.debug("usb-watcher killed");
	logger.info("Exiting now...");
});

