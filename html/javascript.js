function updateFeedbacksOfTrack()
{
	var track = document.getElementById('s_totrack');
	if (!track)
	{
		return false;
	}
	var url = '?cmd=feedbacksoftrack&track=' + track.value;
	requestUpdateItem('feedbacks', url);
	return false;
}

function addSlave()
{
	var slaveCounter = document.getElementById('slavecounter');
	if (!slaveCounter)
	{
		return false;
	}
	var slavesDiv = document.getElementById('slaves');
	if (!slavesDiv)
	{
		return false;
	}

	slaveCounter.value++;
	var url = '/?cmd=slaveadd&priority=' + slaveCounter.value;
	requestAddItem('new_slave_' + slaveCounter.value, url);
	return false;
}

function addRelation()
{
	var relationCounter = document.getElementById('relationcounter');
	if (!relationCounter)
	{
		return false;
	}
	var relationDiv = document.getElementById('relation');
	if (!relationDiv)
	{
		return false;
	}

	relationCounter.value++;
	var url = '/?cmd=relationadd&priority=' + relationCounter.value;
	requestAddItem('new_priority_' + relationCounter.value, url);
	return false;
}

function addFeedback()
{
	var feedbackCounter = document.getElementById('feedbackcounter');
	if (!feedbackCounter)
	{
		return false;
	}

	var track = document.getElementById('track');
	if (!track)
	{
		return false;
	}

	feedbackCounter.value++;
	var url = '/?cmd=feedbackadd&counter=' + feedbackCounter.value + '&track=' + track.value;
	requestUpdateItem('div_feedback_' + feedbackCounter.value, url);
	return false;
}

function checkIntegerValue(name, min, max)
{
	if (min > max)
	{
		return;
	}
	var input = document.getElementById(name);
	if (!input)
	{
		return;
	}
	if (input.value < min)
	{
		input.value = min;
		return;
	}
	if (input.value > max)
	{
		input.value = max;
	}
}

function incrementIntegerValue(name, max)
{
	var input = document.getElementById(name);
	if (!input)
	{
		return;
	}
	var value = parseInt(input.value) + 1;
	if (value > max)
	{
		return;
	}
	input.value = value;
}

function decrementIntegerValue(name, min)
{
	var input = document.getElementById(name);
	if (!input)
	{
		return;
	}
	var value = parseInt(input.value) - 1;
	if (value < min)
	{
		return;
	}
	input.value = value;
}

function setToggleButton(elementName, on)
{
	var element = document.getElementById(elementName);
	if (element)
	{
		if (on == 'true')
		{
			element.classList.remove('button_off');
			element.classList.add('button_on');
		}
		else
		{
			element.classList.remove('button_on');
			element.classList.add('button_off');
		}
	}
}

function deleteElement(elementName)
{
	var element = document.getElementById(elementName);
	if (element)
	{
		element.parentNode.removeChild(element);
	}
}

function onClickAccessory(accessoryID)
{
	var element = document.getElementById('a_' + accessoryID);
	var url = '/?cmd=accessorystate';
	url += '&state=' + (element.classList.contains('accessory_off') ? 'on' : 'off');
	url += '&accessory=' + accessoryID;
	fireRequestAndForget(url);
	return false;
}

function onClickStreet(streetID)
{
	var element = document.getElementById('st_' + streetID);
	var url = '/?cmd=streetexecute';
	url += '&street=' + streetID;
	fireRequestAndForget(url);
	return false;
}

function onClickSwitch(switchID)
{
	var element = document.getElementById('sw_' + switchID);
	var url = '/?cmd=switchstate';
	url += '&state=' + (element.classList.contains('switch_straight') ? 'turnout' : 'straight');
	url += '&switch=' + switchID;
	fireRequestAndForget(url);
	return false;
}

function onClickFeedback(feedbackID)
{
	var element = document.getElementById('f_' + feedbackID);
	var url = '/?cmd=feedbackstate';
	url += '&state=' + (element.classList.contains('feedback_free') ? 'occupied' : 'free');
	url += '&feedback=' + feedbackID;
	fireRequestAndForget(url);
	return false;
}

function onClickSignal(signalID)
{
	var element = document.getElementById('si_' + signalID);
	var url = '/?cmd=signalstate';
	url += '&state=' + (element.classList.contains('signal_green') ? 'red' : 'green');
	url += '&signal=' + signalID;
	fireRequestAndForget(url);
	return false;
}


function onContextLayoutItem(event, ID)
{
	if (event.shiftKey)
	{
		return true;
	}
	event.stopPropagation();
	hideAllContextMenus();
	var menu = document.getElementById(ID + '_context');
	if (menu)
	{
		menu.style.display = 'block';
	}
	return false;
}

function onChangeCheckboxShowHide(checkboxId, divId)
{
	var checkbox = document.getElementById(checkboxId);
	if (!checkbox)
	{
		return;
	}
	var div = document.getElementById(divId);
	if (!div)
	{
		return;
	}
	div.hidden = !checkbox.checked;
}

function onChangeTrackType()
{
	var typeSelect = document.getElementById('s_type');
	if (!typeSelect)
	{
		return;
	}
	var length = document.getElementById('i_length');
	if (!length)
	{
		return;
	}
	var typeValue = typeSelect.value;
	length.hidden = (typeValue == 1 || typeValue == 5)
}

function updateLayoutItem(elementName, data)
{
	var parentElement = document.getElementById('layout');
	if (parentElement)
	{
		var elementContextName = elementName + '_context';
		deleteElement(elementName);
		deleteElement(elementContextName);
		parentElement.innerHTML += data;
		var i;
		var tags = document.getElementsByClassName('layout_item');
		for (i = 0; i < tags.length; i++)
		{
			var tag = tags[i];
			var clone = tag.cloneNode(true);
			tag.parentNode.replaceChild(clone, tag);
		}
	}
}

function requestUpdateLayoutItem(elementName, url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		{
			updateLayoutItem(elementName, xmlHttp.responseText);
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function addItem(elementName, data)
{
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	element.innerHTML += data;
}

function requestAddItem(elementName, url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		{
			addItem(elementName, xmlHttp.responseText);
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function updateItem(elementName, data)
{
	var element = document.getElementById(elementName);
	if (!element)
	{
		return;
	}
	element.innerHTML = data;
}

function requestUpdateItem(elementName, url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
		{
			updateItem(elementName, xmlHttp.responseText);
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function updateTrack(trackID)
{
	elementName = 't_' + trackID;
	var url = '/?cmd=trackget';
	url += '&track=' + trackID;
	requestUpdateLayoutItem(elementName, url);
}

function dataUpdate(event)
{
	var status = document.getElementById('status');
	var arguments = event.data.split(';');
	var argumentMap = new Map();
	arguments.forEach(function(argument) {
		var parts = argument.split('=');
		if (parts[0] == 'status')
		{
			status.innerHTML += parts[1] + '<br>';
			status.scrollTop = status.scrollHeight - status.clientHeight;
		}
		else
		{
			argumentMap.set(parts[0], parts[1]);
		}
	});

	var elementName = "";
	var command = argumentMap.get('command');
	if (command == 'booster')
	{
		elementName = 'b_booster';
		var on = argumentMap.get('on');
		setToggleButton(elementName, on);
	}
	else if (command == 'locospeed')
	{
		elementName = 'locospeed_' + argumentMap.get('loco');
		var element = document.getElementById(elementName);
		if (element)
		{
			element.value = argumentMap.get('speed');
		}
	}
	else if (command == 'locofunction')
	{
		elementName = 'b_locofunction_' + argumentMap.get('loco') + '_' + argumentMap.get('function');
		var on = argumentMap.get('on');
		setToggleButton(elementName, on);
	}
	else if (command == 'locodirection')
	{
		elementName = 'b_locodirection_' + argumentMap.get('loco');
		var on = argumentMap.get('direction');
		setToggleButton(elementName, on);
	}
	else if (command == 'accessory')
	{
		elementName = 'a_' + argumentMap.get('accessory');
		var element = document.getElementById(elementName);
		if (element && argumentMap.has('state'))
		{
			var state = argumentMap.get('state');
			if (state == 'green')
			{
				element.classList.remove('accessory_off');
				element.classList.add('accessory_on');
			}
			else
			{
				element.classList.remove('accessory_on');
				element.classList.add('accessory_off');
			}
		}
	}
	else if (command == 'accessorysettings')
	{
		var accessoryID = argumentMap.get('accessory');
		elementName = 'a_' + accessoryID;
		var url = '/?cmd=accessoryget';
		url += '&accessory=' + accessoryID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'accessorydelete')
	{
		elementName = 'a_' + argumentMap.get('accessory');
		var element = document.getElementById(elementName);
		if (element)
		{
			element.parentNode.removeChild(element);
		}
	}
	else if (command == 'switch')
	{
		elementName = 'sw_' + argumentMap.get('switch');
		var element = document.getElementById(elementName);
		if (element && argumentMap.has('state'))
		{
			var state = argumentMap.get('state');
			if (state == 'straight')
			{
				element.classList.remove('switch_turnout');
				element.classList.add('switch_straight');
			}
			else
			{
				element.classList.remove('switch_straight');
				element.classList.add('switch_turnout');
			}
		}
	}
	else if (command == 'switchsettings')
	{
		var switchID = argumentMap.get('switch');
		elementName = 'sw_' + switchID;
		var url = '/?cmd=switchget';
		url += '&switch=' + switchID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'switchdelete')
	{
		elementName = 'sw_' + argumentMap.get('switch');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'signal')
	{
		elementName = 'si_' + argumentMap.get('signal');
		var element = document.getElementById(elementName);
		if (element && argumentMap.has('state'))
		{
			var state = argumentMap.get('state');
			if (state == 'green')
			{
				element.classList.remove('signal_red');
				element.classList.add('signal_green');
			}
			else
			{
				element.classList.remove('signal_green');
				element.classList.add('signal_red');
			}
		}
	}
	else if (command == 'signalsettings')
	{
		var signalID = argumentMap.get('signal');
		elementName = 'si_' + signalID;
		var url = '/?cmd=signalget';
		url += '&signal=' + signalID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'signaldelete')
	{
		elementName = 'si_' + argumentMap.get('signal');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'streetsettings')
	{
		var streetID = argumentMap.get('street');
		elementName = 'st_' + streetID;
		var url = '/?cmd=streetget';
		url += '&street=' + streetID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'streetdelete')
	{
		elementName = 'st_' + argumentMap.get('street');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'locointotrack')
	{
		updateTrack(argumentMap.get('track'));
	}
	else if (command == 'trackstate')
	{
		elementName = 't_' + argumentMap.get('track');
		var element = document.getElementById(elementName);
		if (element)
		{
			var reserved = false;
			var occupied = false;
			var blocked = false;
			var error = false;


			if (argumentMap.has('occupied'))
			{
				occupied = argumentMap.get('occupied') == 'true';
			}

			if (argumentMap.has('reserved'))
			{
				reserved = argumentMap.get('reserved') == 'true';
			}

			if (argumentMap.has('blocked'))
			{
				blocked = argumentMap.get('blocked') == 'true';
			}

			element.classList.remove('track_free');
			element.classList.remove('track_reserved');
			element.classList.remove('track_reserved_occupied');
			element.classList.remove('track_occupied');
			element.classList.remove('track_error');
			element.classList.remove('track_blocked');

			if (reserved && occupied)
			{
				element.classList.add('track_reserved_occupied');
			}
			else if (reserved)
			{
				element.classList.add('track_reserved');
			}
			else if (occupied)
			{
				element.classList.add('track_occupied');
			}
			else if (blocked)
			{
				element.classList.add('track_blocked');
			}
			else
			{
				element.classList.add('track_free');
			}

			if (error)
			{
				element.classList.add('track_error');
			}

			var contextElement = document.getElementById(elementName + '_context');
			if (contextElement)
			{
				if (reserved == true)
				{
					element.classList.remove('loco_unknown');
					element.classList.add('loco_known');
					contextElement.classList.remove('loco_unknown');
					contextElement.classList.add('loco_known');
				}
				else
				{
					element.classList.remove('loco_known');
					element.classList.add('loco_unknown');
					contextElement.classList.remove('loco_known');
					contextElement.classList.add('loco_unknown');
				}

				if (blocked == true)
				{
					contextElement.classList.remove('track_unblocked');
					contextElement.classList.add('track_blocked');
				}
				else
				{
					contextElement.classList.remove('track_blocked');
					contextElement.classList.add('track_unblocked');
				}
			}

			var locoElement = document.getElementById(elementName + '_text_loconame');
			if (locoElement)
			{
				var direction = '&larr;';
				if (argumentMap.has('direction'))
				{
					direction = argumentMap.get('direction') == 'true' ? '&rarr; ' : '&larr; ';
				}
				var locoName = argumentMap.has('loconame') ? argumentMap.get('loconame') : '';
				locoElement.innerHTML = direction + locoName;
			}
		}
	}
	else if (command == 'tracksettings')
	{
		updateTrack(argumentMap.get('track'));
	}
	else if (command == 'trackdelete')
	{
		elementName = 't_' + argumentMap.get('track');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'feedback')
	{
		elementName = 'f_' + argumentMap.get('feedback');
		var element = document.getElementById(elementName);
		if (element && argumentMap.has('state'))
		{
			var state = argumentMap.get('state');
			if (state == 'on')
			{
				element.classList.remove('feedback_free');
				element.classList.add('feedback_occupied');
			}
			else
			{
				element.classList.remove('feedback_occupied');
				element.classList.add('feedback_free');
			}
		}
	}
	else if (command == 'feedbacksettings')
	{
		var feedbackID = argumentMap.get('feedback');
		elementName = 'f_' + feedbackID;
		var url = '/?cmd=feedbackget';
		url += '&feedback=' + feedbackID;
		requestUpdateLayoutItem(elementName, url);
	}
	else if (command == 'feedbackdelete')
	{
		elementName = 'f_' + argumentMap.get('feedback');
		deleteElement(elementName);
		deleteElement(elementName + '_context');
	}
	else if (command == 'locosettings' || command == 'locodelete')
	{
		loadLocoSelector();
	}
	else if (command == 'layersettings' || command == 'layerdelete')
	{
		loadLayerSelector();
	}
}

var updater = new EventSource('/?cmd=updater');
updater.onmessage = function(event) {
	dataUpdate(event);
};
updater.onerror = function(event) {
	setTimeout(function() {
		location.reload();
	}, 1000);
};

window.layoutPosX = 0;
window.layoutPosY = 0;

function loadPopup(url)
{
	url += '&posx=' + window.layoutPosX;
	url += '&posy=' + window.layoutPosY;
	url += '&posz=' + document.getElementById('s_layer').value;
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var popup = document.getElementById('popup');
		popup.innerHTML = xmlHttp.responseText;
		popup.style.display = 'block';
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function loadLocoSelector()
{
	var loco = document.getElementById('s_loco').value;
	var url = '/?cmd=locoselector';
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var element = document.getElementById('loco_selector');
		element.innerHTML = xmlHttp.responseText;
		var selector = document.getElementById('s_loco');
		if (selectHasValue('s_loco', loco))
		{
			selector.value = loco;
			loadLoco();
			return;
		}
		for (var i = 1; i < 255; ++i)
		{
			if (selectHasValue('s_loco', i))
			{
				selector.value = i;
				loadLoco();
				return;
			}
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function loadLayerSelector()
{
	var layer = document.getElementById('s_layer').value;
	var url = '/?cmd=layerselector';
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var element = document.getElementById('layer_selector');
		element.innerHTML = xmlHttp.responseText;
		var selector = document.getElementById('s_layer');
		if (selectHasValue('s_layer', layer))
		{
			selector.value = layer;
			loadLayout();
			return;
		}
		for (var i = 1; i < 255; ++i)
		{
			if (selectHasValue('s_layer', i))
			{
				selector.value = i;
				loadLayout();
				return;
			}
		}
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function loadProtocol(type, ID)
{
	var selectControl = document.getElementById('s_control');
	if (!selectControl)
	{
		return;
	}
	var controlID = selectControl.value;
	var selectProtocol = document.getElementById('select_protocol');
	if (!selectProtocol)
	{
		return;
	}
	var elementName = 'select_protocol';
	var url = '/?cmd=protocol' + type;
	url += '&control=' + controlID;
	url += '&' + type + '=' + ID;
	requestUpdateItem(elementName, url);
}

function loadRelationObject(priority)
{
	var elementName = 'relation_object_' + priority;
	var object = document.getElementById(elementName);
	if (!object)
	{
		return;
	}
	var typeSelector = document.getElementById('s_relation_type_' + priority);
	if (!typeSelector)
	{
		return;
	}
	var url = '/?cmd=relationobject';
	url += '&objecttype=' + typeSelector.value;
	url += '&priority=' + priority;
	requestUpdateItem(elementName, url);
}

function loadLayoutContext(event)
{
	if (event.shiftKey)
	{
		return true;
	}
	event.preventDefault();
	hideAllContextMenus();
	menu = document.getElementById('layout_context');
	if (!menu)
	{
		return true;
	}
	menu.style.display = 'block';
	menu.style.left = event.pageX + 'px';
	menu.style.top = event.pageY + 'px';
	window.layoutPosX = Math.floor((event.pageX - 254) / 36);
	window.layoutPosY = Math.floor((event.pageY - 92) / 36);
	return true;
}

function isInLayout(position)
{
	layoutPosition = document.querySelector('.layout').getBoundingClientRect();
	return (position.pageX >= layoutPosition.left && position.pageX <= layoutPosition.right && position.pageY >= layoutPosition.top && position.pageY <= layoutPosition.bottom);
}

function hideAllContextMenus()
{
	var menus = document.getElementsByClassName('contextmenu');
	for (var i = 0; i < menus.length; ++i)
	{
		menus[i].style.display = 'none';
	}
}

function hideElement(name)
{
	var element = document.getElementById(name);
	if (!element)
	{
		return;
	}
	element.style.display = 'none';
}

function loadLoco()
{
	var loco = document.getElementById('s_loco');
	if (loco)
	{
		requestUpdateItem('loco', '/?cmd=loco&loco=' + loco.value);
	}
}

function loadLayout()
{
	var layer = document.getElementById('s_layer');
	if (layer)
	{
		requestUpdateItem('layout', '/?cmd=layout&layer=' + layer.value);
	}
}

function sendTimestamp()
{
	var url = '/?cmd=timestamp&timestamp=';
	var timestamp = Math.round(Date.now() / 1000);
	url += timestamp;
	fireRequestAndForget(url);
}

function startUp()
{
	var body = document.getElementById('body');
	if (body)
	{
		body.onclick = function(event) {
			if (event.button == 2)
			{
				return false;
			}
			hideAllContextMenus();
			return true;
		};
	}
	loadLoco();
	loadLayout();
	sendTimestamp();
}

function ShowTab(tabName)
{
	var tabs = document.getElementsByClassName('tab_content');
	if (!tabs)
	{
		return;
	}

	for (var i = 0; i < tabs.length; ++i)
	{
		var tab = tabs[i];
		tab.classList.add('hidden');
	}

	var tab = document.getElementById('tab_' + tabName);
	if (!tab)
	{
		return;
	}
	tab.classList.remove('hidden');

	var tabButtons = document.getElementsByClassName('tab_button');
	if (!tabButtons)
	{
		return;
	}

	for (var i = 0; i < tabButtons.length; ++i)
	{
		var tabButton = tabButtons[i];
		tabButton.classList.remove('tab_button_selected');
	}

	var tabButton = document.getElementById('tab_button_' + tabName);
	if (!tabButton)
	{
		return;
	}
	tabButton.classList.add('tab_button_selected');
}

function submitEditForm()
{
	var url = '/?';
	var form = document.getElementById('editform');
	var i = 0;
	while (true)
	{
		var formElement = form[i];
		if (formElement == undefined)
		{
			break;
		}
		if (i > 0)
		{
			url += '&';
		}
		url += formElement.name;
		url += '=';
		if (formElement.type == 'checkbox')
		{
			url += formElement.checked;
		}
		else
		{
			url += formElement.value;
		}
		++i;
	}
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.onreadystatechange = function() {
		if (xmlHttp.readyState !== 4 || xmlHttp.status !== 200)
		{
			return;
		}
		var popup = document.getElementById('popup');
		popup.innerHTML = xmlHttp.responseText;
		setTimeout(function() { popup.style.display = 'none'; }, 1500);
	}
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
	return false;
}

function locoSpeedSliderChange(locoId)
{
	var slider = document.getElementById('locospeed_' + locoId);
	if (!slider)
	{
		return false;
	}
	var url = '/?cmd=locospeed&loco=';
	url += locoId;
	url += '&speed=';
	url += slider.value;
	fireRequestAndForget(url);
	return false;
}

function fireRequestAndForget(url)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open('GET', url, true);
	xmlHttp.send(null);
}

function selectHasValue(selectId, value) {
	obj = document.getElementById(selectId);
	if (obj == null)
	{
		return false;
	}
	return (obj.innerHTML.indexOf('value="' + value + '"') >= 0);
}
