var jQueryScriptOutputted = false;

var currentJson;
var autoRefreshInSeconds = 3;
var mobileMultiplier = 3;
var isMouseOver = false;
var isMobile = false;

$(document).ready(function() {
   init();
});

function init(){

	if((navigator.userAgent).indexOf("Mobile") > -1)
	{
		isMobile = true;
		autoRefreshInSeconds *= mobileMultiplier;
	}

    window.setInterval(function(){ 
    						if(isMobile || !isMouseOver) 
    						{ 
    							getAllContent();
    						}}, autoRefreshInSeconds * 1000);        
	
    if(isMobile)
    {
	    window.setInterval(function(){
	    	if(isMouseOver)
	    	{
	    		location.reload();
	    	}
	    }, 1000);
    }

	for(var i = 0; i < numberOfRelayBoards; i++)
	{
		createButtonVars(i);
		createBoards(i);
	}
	getAllContent();

};

function createButtonVars(j)
{
	for(var a = 0; a <=j; a++)
		for(var i = 0; i < 16 ; i++)
			window["but_"+a+'_'+i] = window["board"+a+"Status"][i];
}

function createBoards(boardnumber)
{		
	for(var i = 0; i < 16; i++)
	{
		var e = $('<div class="relayButton" onClick=switchRelay('+boardnumber+','+i+') onmouseover="mouseIsOver()" onmouseout="mouseIsOut()">'+eval('textboard'+boardnumber+'['+i+']')+'</div>');
		var buttonname = 'b_'+boardnumber+'_'+i;

		e.addClass("rounded-corners");
		e.attr('id', buttonname);
		
		$('#board'+boardnumber).append(e);

		var currentButtonValue = eval('but_'+boardnumber+'_'+i);

		if(currentButtonValue == 1)
		{
			var currentButton = document.getElementById(buttonname);
			currentButton.className = currentButton.className + ' bon';
		};
			
		if(currentButtonValue == 0)
		{
			var offbut = document.getElementById(buttonname);
			offbut.className = offbut.className + ' boff';
		};
	};
}

function switchRelay(boardnr, pin)
{
		var nextPinValue;
		var switchOffGroupedPin = null;

		if(window["board"+boardnr+"Status"][pin]== 0)
			nextPinValue = 1;
		else
			nextPinValue = 0;

		// Überprüfe, ob der aufgerufene Pin in einer Pingruppe ist..
		switchOffGroupedPin = checkGroups(boardnr, pin);

		// wenn ja, schalte den Pin der Gruppe AUS, welche AN ist... 
		if(switchOffGroupedPin != null)
		{
			window["board"+boardnr+"Status"][switchOffGroupedPin] = 0;
			window["board"+boardnr+"Status"][pin] = 1;

			var stringForNumber = "";

			for(var a = 15; a >= 0; a--)
				stringForNumber = stringForNumber + window["board"+boardnr+"Status"][a]
			
			var representation = parseInt(stringForNumber,2);
			setAllContent(boardnr, representation);					
		}
		else
		{
			// Schalte den Pin, welche geschaltet werden soll...
			setContent(boardnr, pin, nextPinValue, false);
		}
}

function checkGroups(boardnr, pin)
{
	// Überprüfen aller vorhandener Pin-Gruppen 
	for(var i = 0; i < window["board"+boardnr+"Group"].length ; i++)
	{
		var _currentGroup = window["board"+boardnr+"Group"][i];
		var _numberOfPinsInThisGroup = _currentGroup.length;

		// Wenn der gerade aufgerufene Pin in einer Pin-Gruppe ist...
		if(_currentGroup.contains(pin))
		{
			// Dann überprüfe die einzelnen Pins in dieser Gruppe
			for(var j = 0; j < _numberOfPinsInThisGroup; j++)
			{
				var _currentPin = _currentGroup[j];

				// wenn der aktuell betrachtetet Pin nicht der ist, der gerade aufgerufen wurde, aber trotzdem AN ist, gebe diesen Pin zurück...
				if(_currentPin != pin && window["board"+boardnr+"Status"][_currentPin] == 1)
					return _currentPin;
			}
		}
	}
	return null;
}


var myResponseHandler = function(data) {

	for(var t = 0; t < numberOfRelayBoards; t++)
	{
		$('#greenBullet'+t).show();
	}

	for(var h = 0; h < numberOfRelayBoards; h++)
	{
	  	createButtonVars(h);
	  	reloadBoard(h);
	}

	setTimeout(function(){ 	
	for(var t = 0; t < numberOfRelayBoards; t++)
	{
		$('#greenBullet'+t).hide();
	}}, 500);
}


function getAllContent() {
	$.ajax({
		dataType: 'text',
   		url: configAddress+'/Get',
   		success: function(string) {
			data = $.parseJSON(string);
			currentJson = data;
			fillArrays(currentJson.V);
			myResponseHandler();
		}	
	});
}

function fillArrays(myJson)
{
	for(var b = 0; b < numberOfRelayBoards; b++)
	{
		window["board"+b+"Status"] = con(myJson[b]);
		console.log("board: "+b);
		console.log(window["board"+b+"Status"]);
	}	
}

function setContent(currentboard, pin, value, suppressUpdate) {
	return $.ajax({
		url: configAddress + "/Set/"+currentboard+"/"+pin+"/"+value,
		dataType: 'text',
		success: function(string) {

			if(!suppressUpdate)
			{
				data = $.parseJSON(string);
				currentJson = data;
				fillArrays(currentJson.V);
				myResponseHandler();
			}
		}	
		});
}

function con(mynumber) // Wandle die binärcodierte Integerzahl in ein binary array um
{
	var b = [];

	for (var i = 0; i < 16; i++)
		b[i] = (mynumber >> i) & 1;
	
	return b;
}

function setAllContent(currentboard, representation) {
	return $.ajax({
		url: configAddress + "/Sa/"+currentboard+"/"+representation+"/1",
		dataType: 'text',
		success: function(string) {

		data = $.parseJSON(string);
		currentJson = data;
		fillArrays(currentJson.V);
		myResponseHandler();
		
		}	
		});
}



function reloadBoard(aboardnumber)
{
	for(var i = 0; i < 16; i++)
	{
		var buttonname = 'b_'+aboardnumber+'_'+i;		
		var currentButtonValue = eval('but_'+aboardnumber+'_'+i);
		setButtonColor(buttonname, currentButtonValue);		
	};
}

function setButtonColor(mybuttonname, currentButtonValue)
{
	var currentButton = document.getElementById(mybuttonname);

	if(currentButtonValue == 1)
	{
		currentButton.className = "relayButton rounded-corners bon";
	}

	if(currentButtonValue == 0)
	{
		currentButton.className = "relayButton rounded-corners boff";
	}
	$("#"+mybuttonname).hide().show();
}

function mouseIsOver()
{
	isMouseOver = true;
}

function mouseIsOut()
{
	isMouseOver = false;
}

Array.prototype.contains = function(obj) {
    var i = this.length;
    while (i--) {
        if (this[i] === obj) {
            return true;
        }
    }
    return false;
}