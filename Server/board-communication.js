var Options = require('options');

function BoardCommunication()
{
	var methods				= this.constructor.prototype,
		nodeSerialport		= require("serialport"),	
		instance			= null,
		status				= 0,
		asyncCallback		= function() {},
		connCallback		= function() {},
		closeCallback		= function() {},
		pairIntervalHandler = null,
		pairAttemptNumber	= 1,
		pairAttemptMaxRetry	= 3,
		pairAttemptInterval	= 1000,
		_connectedDevice	= null,
		_cmdSentCallback	= false,
		_packetQueue		= [];
	
	var statusTypeDef	= {
		NOT_CONNECTED	: 0,
		PAIRING			: 1,
		READY			: 2,
		BUSY			: 3
	};
	
	var sendingOptions = new Options({
		enqueueOnBusy		: true,
		callbackPacketType	: 'json_object'
	});
	
	//configure the logger
	var log4js = require('log4js');
	log4js.configure('log4js.json');
	var logger = log4js.getLogger(require('path').basename(__filename));
	logger.setLevel('INFO');
	
	status = statusTypeDef.NOT_CONNECTED;
	
	function __buildPairRequestPacket()
	{
		var packet = {
			target 	: "BOARD",
			cmd		: "CMD_PAIR_REQUEST"
		};
		return packet;
	}
	
	function __pairAttempt(device)
	{
		logger.info("Attempt to pair #%s...", pairAttemptNumber);
		methods.sendObjectPacket(__buildPairRequestPacket(), function(response) {
			if (response.cmd_reply == "PAIR_COMPLETE" && status == statusTypeDef.PAIRING )
			{
				logger.info("Pair complete with device %s", device.serialNumber);
				
				// reset the pair attempt
				clearInterval(pairIntervalHandler);
				pairAttemptNumber = 1;
				
				// store a pointer to the connected device
				_connectedDevice = {
					physical: device,
					information: response.message
				};
				
				// mark the communication ready
				status = statusTypeDef.READY;
				
				// finally call the connCallback
				connCallback(_connectedDevice);
			}
		});
	}
	
	methods.connect = function(device, asyncHandle, connectedHandle, closeHandle)
	{		
		logger.debug("Attempt to connect...");
		instance = new nodeSerialport.SerialPort(device.comName, {
			baudrate : 9600,
		    parser: nodeSerialport.parsers.readline("\r\n")
		});
		
		// replace the default handler for connected event received from the channel
		if (typeof connectedHandle === 'function')
		{
			connCallback = connectedHandle;
		}
		
		instance.on('open', function() {
			
			status = statusTypeDef.PAIRING;
			
			logger.info("Connected to the device %s [%s] through %s", device.serialNumber, device.manufacturer, device.comName);
			
			// Start the pairing process (with repeated attempt)
			__pairAttempt(device);
			
			// next attempts...
			pairIntervalHandler = setInterval(function() {
				if (pairAttemptNumber < pairAttemptMaxRetry)
				{
					pairAttemptNumber++;
					__pairAttempt(device);
				} else {
					clearInterval(pairIntervalHandler);
					logger.warn("Unable to pair with the device %s after %s attempts", device.serialNumber, pairAttemptNumber);
				}
			}, pairAttemptInterval);
		});
		
		// replace the default handler for close event received from the channel
		if (typeof closeCallback === 'function')
		{
			closeCallback = closeHandle;
		}
		
		instance.on('close', function() {
			logger.debug("Close callback called...");
			
			// reset the pairAttemptNumber 
			pairAttemptNumber = 1;
			
			// reset the _connectedDevice
			_connectedDevice = null;
			
			// reset the status
			status = statusTypeDef.NOT_CONNECTED;
			
			// call the closeCallback
			closeCallback(device);
		});
		
		// handle the error callback
		instance.on("error", function(){
			logger.debug("Error callback called...");
		});
		
		// replace the default handler for asynchronous signals received from the board
		if (typeof asyncHandle === 'function')
		{
			asyncCallback = asyncHandle;
		}
		
		var __packetReceived = function(packet)
		{
			logger.trace("Received packed from the board: %s", packet);
			if ( _cmdSentCallback === false )
			{
				logger.debug("No callback attached. Calling the asynchronous callback for the signal received");
				asyncCallback(JSON.parse(packet));
			}
		};
		instance.on('data', __packetReceived);
	};
	
	methods.sendObjectPacket = function(packet, callback, options)
	{
		if (instance === null)
		{
			logger.warn("The board is not connected");
			return;
		}
		
		if (typeof packet !== 'object')
		{
			logger.error("The packet must be a well formed object");
			return;
		}
		
		var serialized_packet = JSON.stringify(packet);
		
		methods.sendSerializedPacket(serialized_packet, callback, options);		
	};
	
	/**
	 * @brief This method is used to send serialized packet to the board
	 * @param packet	The string packet to send (json representation of the packet)
	 * @param callback	The callback function called when a reply from the board will be sent
	 * @parma options	An object containing:
	 * 						- callbackPacketType: ('json_object'|'json_serialized_string') the type of the parameter returned into the callback
	 * 						- enqueueOnBusy		: true | false If the communication is busy, enqueue the packet and send it when the communication will became ready
	 * @return null
	 */
	methods.sendSerializedPacket = function(packet, callback, options)
	{
		// clone the sendingOptions
		// reset current options
		sendingOptions.reset();
		var opts = sendingOptions;
		
		// merge default options with the custom received now
		opts.merge(options);
		
		if (instance === null)
		{
			logger.warn("The board is not connected");
			return;
		}
		
		if (!(status === statusTypeDef.READY || status === statusTypeDef.PAIRING))
		{
			logger.trace("Communcation status not ready (status # is %s)", status);
			logger.debug("Attempt to send: %s", packet);
			if (opts.value.enqueueOnBusy===true)
			{
				logger.info("The communication is busy. Enqueueing the packet");
				// push the json representation of the packet inside the queue
				var queueElement = {
					packet : packet,
					callback : callback,
					options: options
				};
				_packetQueue.push(queueElement);
			} else {
				logger.warn("The communication is busy. Please try again");
			}
			return;
		}
		
		if (typeof packet !== 'string')
		{
			logger.error("The packet must be a well formed serialized string");
		}
		
		// do we have a callback? can we attach the callback (ready status)?
		if (typeof callback === 'function')
		{
			// register the custom callback
			var __customCallback = function(json_packet)
			{							
				// from here, we accept data from the board only in a json packet... so we have to parse to an object
				if (opts.value.callbackPacketType==='json_object')
				{
					callback(JSON.parse(json_packet));
				}					
				else if (opts.value.callbackPacketType==='json_serialized_string')
				{
					callback(json_packet);		
				}
				
				// this set the status to READY
				if (status === statusTypeDef.BUSY)
				{
					status = statusTypeDef.READY;
				}					
				
				// enable the async callback again
				_cmdSentCallback = false;
				
				// check if there are packets inside the queue
				if (_packetQueue.length > 0)
				{
					var queueElement = _packetQueue.shift();
					methods.sendSerializedPacket(queueElement.packet, queueElement.callback, queueElement.options);
				}
			};
			
			// this notifies the asynchronous callback that a custom callback has been attached
			_cmdSentCallback = true;
			
			// attach the customCallback only once
			instance.once('data', __customCallback);
		}
		
		// mark the communication busy (because we always need a reply message from the board that will set the communication ready again)
		if (status === statusTypeDef.READY)
		{
			status = statusTypeDef.BUSY;
		}
			
		logger.trace("Sending packet to the board: %s", packet);
		
		// we can serialize the json and send it
		instance.write(packet+'\r');
	};
	
	methods.getConnectedDevice = function()
	{
		if (status !== statusTypeDef.READY && status !== statusTypeDef.BUSY)
		{
			return false;
		}		
		return _connectedDevice;
	};
	
	
}

// exports the class type, not a new object
module.exports = BoardCommunication;