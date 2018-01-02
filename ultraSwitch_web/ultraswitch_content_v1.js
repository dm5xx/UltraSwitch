function addContent()
{
	var div = document.getElementById('content');
	console.log(div);

	var htmlString = '';

	for(var b = 0; b < numberOfRelayBoards; b++)
	{

		htmlString += '<div id="container"';

		if(b+1 == numberOfRelayBoards)
			htmlString += ' class="myLast"';

		htmlString += '><div class="rotate">Board '+b+'<img id="greenBullet'+b+'" src="http://h.mmmedia-online.de/dot.png"></div><div id="board'+b+'"></div></div><div id="tempResponse"></div>';
	}

	div.innerHTML = htmlString;
}

var textboard0 = ["Nord","Ost","Süd","West","Yeasu AN","Icom AN","Filter 80m","Filter 40m","Filter 20m","Beam","Vertikal","Dipol","Ant auf 5000","Ant auf Icom","Rotor AN","PA AN"];
var board0Status = [];
var board0Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard1 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board1Status = [];
var board1Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard2 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board2Status = [];
var board2Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard3 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board3Status = [];
var board3Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard4 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board4Status = [];
var board4Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard5 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board5Status = [];
var board5Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard6 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board6Status = [];
var board6Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];

var textboard7 = ["Port 0","Port 1","Port 2","Port 3","Port 4","Port 5","Port 6","Port 7","Port 8","Port 9","Port 10","Port 11","Port 12","Port 13","Port 14","Port 15",];
var board7Status = [];
var board7Group = [[0,1,2,3],[6,7,8],[9,10,11],[12,13]];


