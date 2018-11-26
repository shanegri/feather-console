var eventSource = new EventSource((TESTING == true ? "http://192.168.1.174/" : "") + "socket");

var canvas = document.createElement("canvas");
canvas.width      = B_WIDTH;
canvas.height     = B_HEIGHT;
canvas.style.width  = "400px";
canvas.style.height = "400px";
canvas.style.background = "black";

document.body.appendChild(canvas);

var c = canvas.getContext("2d");

eventSource.onmessage = e => {
    let data = JSON.parse(e.data);
    c.fillStyle = "black";
    c.fillRect(0,0,B_WIDTH,B_HEIGHT);
    c.fillStyle = "white";
    c.fillRect(P_OFFSET, data[0],1, P_HEIGHT);  
    c.fillRect(B_WIDTH - P_OFFSET,data[1],1, P_HEIGHT);    
    c.fillRect(data[2],data[3],1,1);        
}