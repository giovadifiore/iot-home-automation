//(function($) {
		
	var hostChannelUrl	= 'ws://'+window.location.hostname+':'+window.location.port,
		hostChannel		= null,
		dom				= {
			$modalNotConn	: $('#noBoardConnected'),
			$numberOfDevices: $('#numberOfConnectedDevices'),
			$deviceList		: $('#dropdownConnectedDevices'),
			temperature		: {
				$tempLabels		: $('.board-temperature-value'),
				$tempContainer	: $('.board-temperature-container'),
				$toggle			: $('#temperatureMonitoringToggle')
			},
			$slider			: $('#ledSlider'),
			$getAccelero	: $('#getAccelero'),
			$toggleTemp		: $('#toggleTemp'),
			boardInfo		: {
				$name:			$('.board-name-value'),
				$connection: 	$('.board-connection-value'),
				$manufacturer:  $('.board-manufacturer-value'),
				$serialNumber:	$('.board-serialnumber-value'),
				$productId:		$('.board-productid-value'),
				$roomName:		$('.board-room-name'),
				$roomType:		$('.board-room-type-value')
			},
			door			: {
				$status			: $('.board-door-value'),
				$autoLightToggle: $('#doorAutoLightToggle')
			},
			lights			: {
				$toggles	: $(".board-light-toggle")
			},
			experimentals	: {
				$acceleroBtn: $('#readAcceleroBtn'),
				$output		: $('#experimentalOutput'),
				$cleanBtn	: $('#clearExpOutput'),
				$startAlarmBtn : $('#startAlarmBtn')
			},
			pages			: {
				$container:		$('#pagesContainer')
			}
		};
	
	var appStatus = {
		tempMonitoring: true,
		boardConnected: false,
		autoLightOnDoor: false,
		domReady: false
	};
	
	/* Internal variables ----------------------------------------------------------------------- */
	var _emptyFunc				= function(){},
		_backgroundTaskHandler	= null,
		_callbackParamsHash	= [];

	/**
	 * @brief	This is the main initialization function.
	 * 			- Initializes the WebSocket to the host
	 * 			- Discovers all the boards connected
	 * 			- Updates the internal status
	 */
	function _init()
	{
		// discover all devices connected to the hsot  (currently only one is supported)
		function __discoveryNewDevices()
		{	
			function ___devicesConnected(response)
			{
				if (response.cmd_reply==="CONNECTED_DEVICES")
				{
					var devices = response.message.devices;
					if (devices.length > 0)
					{
						var mainDevice = devices[0]; // only the first device is supported
						
						// do the connect Handler
						_boardConnectHandler(mainDevice);
					}
					else
					{
						domSetNotConnectedModal('show');
					}				
					
					// ready to attach the general message dispatcher function
					bindHostOnMessage(_hostMessageDispatcher);
					
					// attach all dom event handlers
					domAttachEventHandlers();					
				}
			}
					
			var discoveryPacket =  {
				target: "HOST",
				cmd: "CMD_GET_CONNECTED_DEVICES"
			};

			bindHostOnMessageOnce(___devicesConnected);
			
			sendPacketToHost(discoveryPacket);
		}
		
		// create new websocket with the board's host
		hostChannel = new WebSocket(hostChannelUrl);
		
		// attach the onopen event handler
		hostChannel.onopen = __discoveryNewDevices;
	}
	
	function _boardDisconnectHandler(device)
	{
		// reset the board status
		domResetBoardStatus();
		domSetNotConnectedModal('show');
		appStatus.boardConnected = false;
		
		// update the number
		// TODO: support more devices
		domUpdateNumberOfDevices(0);
		domResetDeviceList();
		
		// suspend the background task
		_backgroundTaskSuspend();
	}
	
	function _boardConnectHandler(device)
	{
		// update the board status into the dom
		domUpdateBoardStatus(device);
		domSetNotConnectedModal('hide');
		appStatus.boardConnected = true; // connected!
		
		// update the number of connected devices
		// TODO: support more devices
		domUpdateNumberOfDevices(1);
		domPushNewDeviceInList(device.information);
		
		// we have to trigger again the pageOpen to refresh all the assets status
		if (appStatus.domReady)
		{
			$.onPageChange(undefined, domGetOpenedPage());
		}			
		
		// start the background task
		_backgroundTaskStart(_mainBackgroundTask, 1000);
	}
	
		
	/**
	 * @brief	This is the main message dispatcher function
	 * @param	A message from the host or from the board through the host
	 */
	function _hostMessageDispatcher(message, callbackParameters)
	{
		// we need to understand if this message is a signal, a reply message or a command
		if (typeof message.cmd === "string")
		{
			// this is a command
			_dispatchCommand(message, callbackParameters);
		} else if (typeof message.cmd_reply === "string")
		{
			// this is a cmd_reply from the board
			_dispatchCommandReply(message, callbackParameters);
		} else if (typeof message.signal === "string")
		{
			// this is a signal (exception)
			_dispatchSignal(message, callbackParameters);
		}
	}
	
	function _dispatchCommand(packet, params)
	{
		// no commands received from the board for now
	}
	
	function _dispatchCommandReply(packet, params)
	{	
		switch(packet.cmd_reply)
		{
			case "TEMPSENS_VALUE":
				domUpdateTemperature(parseFloat(packet.message.value));
			break;
			case "DOOR_STATUS":
				domUpdateDoorStatus(packet.message.value);
			break;
			case "DOOR_AUTOLIGHT_STATUS":
				var showNotification = true;
				if (typeof params !== 'undefined')
				{
					showNotification = params.showNotification;
				}
				domUpdateDoorAutolightStatus(packet.message.value, showNotification); // update the status and show the notification
			break;
			case "LED_STATUS":
				domUpdateLightToggleStatus(packet.message.led, packet.message.value);
			break;
			case "ACCELERO_AXES":
				domAppendToExperimentalOutput(JSON.stringify(packet));
			break;
			case "ALARM_STARTED":
				domAppendToExperimentalOutput(JSON.stringify(packet));
			break;
			default:
				console.log("Unhandled %s command reply packet received", packet.cmd_reply);
			break;
		}
	}
	
	function _dispatchSignal(packet, params)
	{
		switch(packet.signal)
		{
			case "SIG_DEVICE_DISCONNECTED":
				_boardDisconnectHandler(packet.message.device);
			break;
			case "SIG_DEVICE_CONNECTED":
				_boardConnectHandler(packet.message.device);
			break;
			case "SIG_DOOR_STATUS_CHANGED":
				domUpdateDoorStatus(packet.message.value, true); // show notification
			break;
			case "SIG_USR_BTN_PRESSED":
				domShowNotification("Comandi asincroni", "Premuto pulsante utente", "glyphicon glyphicon-exclamation-sign");
			break;
			default:
				console.log("Unhandled %s signal packet received", packet.signal);
			break;
		}
	}
	
	/**
	 * @brief	Start the background task
	 * @param	task	The function to be called every period of time
	 * @param	period	The task's period in ms
	 */
	function _backgroundTaskStart(task, period)
	{
		if (typeof task === 'function')
		{
			_backgroundTaskHandler = setInterval(task, period);
		}
	}
	
	/**
	 * @brief	Suspend the background task
	 */
	function _backgroundTaskSuspend()
	{
		clearInterval(_backgroundTaskHandler);
	}	
		
	/**
	 * @brief	Main background task function 
	 */
	function _mainBackgroundTask()
	{
		// request the temperature
		if (appStatus.tempMonitoring)
		{
			var packet = {
	  			target: "BOARD",
	  			cmd: "CMD_GET_TEMPSENS_VALUE"
	  		};  		
	  		sendPacketToHost(packet);
		}
	}
	
	/**
	 * @brief This function sends a packet to the board's host if the channel is not null
	 * @param packet The packet to send to the board's host
	 * @return void
	 */
	function sendPacketToHost(packet, cmdReplyKey, params)
	{
		if (hostChannel!==null)
		{
			if (typeof cmdReplyKey !== 'undefined' && 
				typeof cmdReplyKey === 'string' && 
				typeof params !== 'undefined')
			{
				// This registers the params on the key cmdReplyKey
				// When a message with cmd_reply === cmdReplyKey will arrive, the params will be passed
				_callbackParamsHash[cmdReplyKey] = params;
			}
			hostChannel.send(JSON.stringify(packet));
		}
	}
	
	/**
	 * @brief 	This function binds a function callback to the onmessage event triggered by the host.
	 * 			This function automatically parses the packet from the json format
	 * @param func 	A function to be called when the host sends a new message
	 */
	function bindHostOnMessage(func)
	{
		if (hostChannel !== null)
		{
			function _onMessageTemp(msg)
			{
				if (typeof func === 'function' && typeof msg !== 'undefined')
				{
					var msgObject = JSON.parse(msg.data),
						params;
						
					// Check if this message is a cmd_reply message and a params object was stored for this cmd_reply
					if (typeof msgObject.cmd_reply === "string")
					{
						var cmd_reply = msgObject.cmd_reply;
						// now check if we have params stored for this message
						if (typeof _callbackParamsHash[cmd_reply] !== 'undefined')
						{
							// we got params!
							params = _callbackParamsHash[cmd_reply];
							// remove this element
							delete _callbackParamsHash[cmd_reply];
						}
					}
					func(JSON.parse(msg.data), params);
				}
			}			
			hostChannel.onmessage = _onMessageTemp;
		}
	}
	
	/**
	 * @brief	This function is the same of bindHostOnMessage but the callback is fired only one time
	 * @param func	A function to be called when the host sends the next message
	 */
	function bindHostOnMessageOnce(func)
	{
		if (hostChannel!==null)
		{
			function _onMessageTempOnce(msg)
			{
				if (typeof func === 'function' && typeof msg !== 'undefined')
				{
					var msgObject = JSON.parse(msg.data),
						params;
						
					// Check if this message is a cmd_reply message and a params object was stored for this cmd_reply
					if (typeof msgObject.cmd_reply === "string")
					{
						var cmd_reply = msgObject.cmd_reply;
						// now check if we have params stored for this message
						if (typeof _callbackParamsHash[cmd_reply] !== 'undefined')
						{
							// we got params!
							params = _callbackParamsHash[cmd_reply];
							// remove this element
							delete _callbackParamsHash[cmd_reply];
						}
					}
					func(JSON.parse(msg.data), params);
				}
				// if the callback didn't change during the callback, reset it
				if (hostChannel.onmessage === _onMessageTempOnce)
				{
					hostChannel.onmessage = null;
				}					
			};	
			hostChannel.onmessage = _onMessageTempOnce;
		}
	}
	
	/**
	 * @brief This function will be called when the #doors page will appear
	 * @param fromPage
	 */
	function evtOnDoorsPageOpen(fromPage)
	{
		// we are going to request the door status
		var packet = {
			target: "BOARD",
			cmd: "CMD_GET_DOOR_STATUS"
		};
		sendPacketToHost(packet);
		
		// we are going to request the door autolight status
		var packet = {
			target: "BOARD",
			cmd: "CMD_GET_DOOR_AUTOLIGHT_STATUS"
		};
		// don't show the dom notification when the callback arrives
		var params = {
			showNotification: false
		};
		sendPacketToHost(packet, "DOOR_AUTOLIGHT_STATUS", params);
	}
	
	function evtOnLightsPageOpen(fromPage)
	{
		// here we have to check the status of each light to fix the toggles
		dom.lights.$toggles.each(function(){
			var $toggle = $(this),
				ledName = $toggle.attr('data-led-name');
			
			var packet = {
				target : "BOARD",
				cmd : "CMD_GET_LED_STATUS",
				message : {
					led : ledName
				}
			};
			sendPacketToHost(packet);
		});
	}
	
	function evtOnDashboardPageOpen(fromPage)
	{
		var status = "off";
		if (appStatus.tempMonitoring===true)
		{
			status = "on";
		}
		// update the temperature toggle
		domUpdateTemperatureToggle(status);
	}
		
	/* DOM manipulation functions ------------------------------------------------------------------------------- */
	
	/**
	 * @brief general dom event handlers
	 */
	function domAttachEventHandlers()
	{
		// on page change event (bind a function to the global jQuery ($) object so that other js modules can call it, ie. the menu-pages.js)
		function _onPageChange(fromPage, toPage)
		{
			if (!appStatus.boardConnected) return false;
			
			switch(toPage)
			{
				case "#dashboard":
					evtOnDashboardPageOpen(fromPage);
				break;
				case "#doors":
					evtOnDoorsPageOpen(fromPage);
				break;
				case "#lights":
					evtOnLightsPageOpen(fromPage);
				break;
			}
		}
		$.onPageChange = _onPageChange;
		
		// trigger the event for the first time on document loading
		$.onPageChange(undefined, domGetOpenedPage());
		
		// on temperature toggle change
		dom.temperature.$toggle.change(function() {
			appStatus.tempMonitoring = $(this).prop('checked');
			dom.temperature.$tempContainer.toggleClass('temperature-monitoring', appStatus.tempMonitoring);
		});
		
		// on autolight toggle change
		dom.door.$autoLightToggle.on("change", function(e, source) {
			
			// change the global status
			appStatus.autoLightOnDoor = $(this).prop('checked');
			
			// prepare the packet to the board
			var cmd = "CMD_DOOR_AUTOLIGHT_ON";
			if (appStatus.autoLightOnDoor==false)
			{
				cmd = "CMD_DOOR_AUTOLIGHT_OFF";
			}
			var packet = {
				target: "BOARD",
				cmd: cmd
			};			
			// sends the command
			sendPacketToHost(packet);
		});
		
		// on change lights toggles
		dom.lights.$toggles.on("change", function(e) {
			
			var tmpValue = $(this).prop('checked'),
				led = $(this).attr('data-led-name'),
				value = "off";
						
			if (tmpValue===true)
			{
				value = "on";
			}
			var packet = {
				target : "BOARD",
				cmd: "CMD_SET_LED_STATUS",
				message : {
					led : led,
					value: value
				}
			};
			sendPacketToHost(packet);
		});
		
		dom.experimentals.$acceleroBtn.on("click", function() {
			// request the accelero data
			var packet = {
				target: "BOARD",
				cmd: "CMD_GET_ACCELERO_AXES"
			};			
			sendPacketToHost(packet);
		});
		
		dom.experimentals.$cleanBtn.on("click", function() {
			domClearExperimentalOutput();
		});
		
		
		dom.experimentals.$startAlarmBtn.on("click", function(){
			var packet = {
				target: "BOARD",
				cmd: "CMD_START_ALARM"
			}
			sendPacketToHost(packet);
		});
		
		// now set that the all dom elements modified by this app (and events handlers) are ready
		appStatus.domReady = true;
	}
	
	/**
	 * 
	 */
	function domRenderTemplate(tpl, params)
	{
		if (typeof doT === 'undefined')
		{
			console.error("Please include the doT library to use this function");
		}
		var _tempFunc = doT.template(tpl);
		return _tempFunc(params);
	}
	
	/**
	 * @brief Update the information about the board
	 */
	function domUpdateBoardStatus(mainDevice)
	{
		dom.boardInfo.$name.text(mainDevice.information.device_name);
		dom.boardInfo.$connection.text(mainDevice.physical.comName);
		dom.boardInfo.$manufacturer.text(mainDevice.physical.manufacturer);
		dom.boardInfo.$serialNumber.text(mainDevice.physical.serialNumber);
		dom.boardInfo.$serialNumber.text(mainDevice.physical.serialNumber);
		dom.boardInfo.$productId.text(mainDevice.physical.productId.toUpperCase().replace('0X', '0x'));
		dom.boardInfo.$roomName.html('<i class="'+mainDevice.information.room_icon+'"></i>&nbsp;'+mainDevice.information.room_name);
		dom.boardInfo.$roomType.text(mainDevice.information.room_type);
	}
	
	/**
	 * @brief Reset the status of the board
	 */
	function domResetBoardStatus()
	{
		$.each(dom.boardInfo, function(k,$v) {
			$v.empty();
		});
		dom.boardInfo.$roomName.text("Nessun dispositivo connesso");
	}
	
	/**
	 * @brief	Manage the NotConnected modal
	 * @param	cmd One of 'show' or 'hide' as defined in the bootstrap docs.
	 */
	function domSetNotConnectedModal(cmd)
	{
		dom.$modalNotConn.modal(cmd);
	}
	
	/**
	 * @brief	This function updates the temperature float
	 * @param 	temp the float value of the temperature
	 */
	function domUpdateTemperature(temp)
	{
		dom.temperature.$tempContainer.addClass('temperature-monitoring');
		dom.temperature.$tempLabels.text(Math.round(temp*10)/10 + " °C");
	}
	
	function domUpdateTemperatureToggle(status)
	{
		var checkedStatus = true;
		if (status==="off")
		{
			checkedStatus = false;
		}
		dom.temperature.$toggle.prop('checked', checkedStatus).bootstrapToggle('updateSilent');
	}
	
	/**
	 * @brief	This function updates the door status panel
	 * @param status
	 */
	function domUpdateDoorStatus(status, showNotification)
	{
		dom.door.$status.html(domRenderTemplate(domTemplates.doorStatus, {status: status}));
		if (showNotification===true)
		{
			var msg = "Porta aperta";
			if (status=='close')
			{
				msg = "Porta chiusa";
			}
			domShowNotification("Controllo porte", msg, "mega-octicon octicon-sign-in");
		}
	}
	
	function domUpdateDoorAutolightStatus(status, showNotification)
	{
		// we have to update the toggle with the data received from the board
		var checkedStatus = true;
		if (status==="off")
		{
			checkedStatus = false;
		}
		// the updateSilent event won't call the onChange event
		dom.door.$autoLightToggle.prop('checked', checkedStatus).bootstrapToggle('updateSilent');
		if (showNotification===true)
		{
			var title = "Controllo luci",
				icon = "fa fa-lightbulb-o fa-2x",
				msg = "Accensione automatica attivata";
			if (status==="off")
			{
				msg = "Accensione automatica disattivata";
			}
			domShowNotification(title, msg, icon);
		}
	}
	
	function domUpdateLightToggleStatus(ledName, status)
	{
		var checked = false;
		if (status==="on")
		{
			checked = true;
		}
		dom.lights.$toggles.filter('[data-led-name="'+ledName+'"]').prop('checked', checked).bootstrapToggle('updateSilent');
	}
	
	function domShowNotification(title, message, icon, type)
	{
		if (typeof type==='undefined')
		{
			type = 'info';
		}
		$.notify({
			icon: icon,
			title: title,
			message: message
		},{
			// settings
			type: type,
			delay: 1000,
			newest_on_top: true,
			offset: {
				y: 80,
				x: 20
			}
		});
	}
	
	function domAppendToExperimentalOutput(string)
	{
		dom.experimentals.$output.val(dom.experimentals.$output.val()+string);
	}
	
	function domClearExperimentalOutput()
	{
		dom.experimentals.$output.val('');
	}
	
	function domGetOpenedPage()
	{
		return dom.pages.$container.find('.active-page').attr('data-page');
	}
	
	function domUpdateNumberOfDevices(number)
	{
		var html = domRenderTemplate(domTemplates.numberOfDevicesConnected, {number: number});
		dom.$numberOfDevices.html(html);
	}
	
	function domPushNewDeviceInList(deviceInformation)
	{
		dom.$deviceList.append(domRenderTemplate(domTemplates.deviceDropdownItem, deviceInformation));
	}
	
	function domResetDeviceList()
	{
		dom.$deviceList.empty();
	}	
	
	// init the application when the dom is ready
	$(document).ready(_init);
	
//}(jQuery));