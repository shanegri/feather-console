var eventSource = new EventSource((TESTING == true ? "http://192.168.43.72/" : "") + "socket");

var canvas = document.createElement("canvas");
canvas.width      = B_WIDTH;
canvas.height     = B_HEIGHT;
canvas.style.width  = "400px";
canvas.style.height = "400px";
canvas.style.background = "black";

var p1Score = document.createElement("div");
p1Score.style.color = "white";
p1Score.style.position = "absolute";
p1Score.style.top = "10px";
p1Score.style.left = "200px";
p1Score.style.fontSize = "50px";

var p2Score = document.createElement("div");
p2Score.style.color = "white";
p2Score.style.position = "absolute";
p2Score.style.top = "350px";
p2Score.style.left = "200px";
p2Score.style.fontSize = "50px";

p2Score.innerHTML = 0
p1Score.innerHTML = 0

document.body.appendChild(canvas);
document.body.appendChild(p2Score);
document.body.appendChild(p1Score);

var c = canvas.getContext("2d");
var handling = false;

//Data format
// 0: player 1 y position
// 1: player 2 y position
// 2: ball     x position
// 3: ball     y position
// 4: player 1 score
// 5: player 2 score
eventSource.onmessage = e => {
    if(handling) return;
    handling = true;
    let data = JSON.parse(e.data);
    c.fillStyle = "black";
    c.fillRect(0,0,B_WIDTH,B_HEIGHT);
    c.fillStyle = "white";
    c.fillRect(P_OFFSET, data[0],1, P_HEIGHT);  
    c.fillRect(B_WIDTH - P_OFFSET,data[1],1, P_HEIGHT);    
    c.fillRect(data[2],data[3],1,1); 
    p2Score.innerHTML = data[4]
    p1Score.innerHTML = data[5]
    handling = false;
}
