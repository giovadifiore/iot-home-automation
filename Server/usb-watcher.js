/**
 * New node file
 */

// import modules
var sleep 			= require('sleep'),
	nodeSerialport	= require('serialport');

// configure the logger
var log4js = require('log4js');
log4js.configure('log4js.json');
var logger = log4js.getLogger(require('path').basename(__filename));
logger.setLevel('INFO');

// global variables
var productIdSearched = '0xCAFE';
var connectedDevices = [];

// functions
function __matchProductId(prodId)
{
	return productIdSearched.toLowerCase() === prodId.toLowerCase();
}

function __isAlreadyConnected(serialNumber)
{
	return connectedDevices.indexOf(serialNumber) !== -1;
}

function __deviceConnectionClosed(device)
{
	if (__isAlreadyConnected(device.serialNumber))
	{
		logger.debug("Attempt to de-register the device %s", device.serialNumber);
		delete connectedDevices[connectedDevices.indexOf(device.serialNumber)];
		logger.info("Device %s de-registered", device.serialNumber);
	}
}

function __waitConnection()
{
	nodeSerialport.list(function (err, ports) {
	  ports.forEach(function(port) {
		  if (__matchProductId(port.productId))
		  {
			  if (!__isAlreadyConnected(port.serialNumber))
			  {
				  logger.info('Detected new device with serialnumber: %s', port.serialNumber);
				  connectedDevices.push(port.serialNumber);
				  var message = {
					'cmd' 		: 'DEVICE_DETECTED',
					'device'	: port
				  };
				  process.send(message);
			  }
		  }
	  });
	});
}

process.on('message', function(m) {
	if (m.cmd === "DEVICE_CONNECTION_CLOSE")
	{
		__deviceConnectionClosed(m.device);
	}
});

setInterval(__waitConnection, 100);