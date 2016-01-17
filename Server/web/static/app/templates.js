var domTemplates = {
	numberOfDevicesConnected: '\
	<span class="number-of-devices"> \
		{{? it.number>0 }}<i class="fa fa-check"></i>&nbsp;{{?}}\
		<span class="number">{{=it.number}}</span> \
		dispositiv{{ if(it.number==1) }}o{{ else }}i{{ ; }} conness{{ if(it.number==1) }}o{{ else }}i{{ ; }}\
	</span>'
	, deviceDropdownItem: '\
		<li class="text-right">\
			{{? it.room_icon}}<i class="{{=it.room_icon}}"></i>{{?}}&nbsp;{{=it.room_name}}<br/>\
			<span class="muted"><small>{{=it.room_type}}</small></span>\
		</li>'
	, doorStatus: '\
		<span class="door-status{{? it.status==="open" }} open{{?}}">\
			{{? it.status==="open" }}\
				<i class="fa fa-check"></i>&nbsp;Aperta\
			{{?? it.status==="close"}}\
				<i class="fa fa-times"></i>&nbsp;Chiusa\
			{{??}}\
			{{?}}\
		</span>'
};