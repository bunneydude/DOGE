var express = require('express');
var http = require('http');
var io = require('socket.io');
 
 
var app = express();
app.use(express.static('./public'));
//Specifying the public folder of the server to make the html accesible using the static middleware
 
var server =http.createServer(app).listen(8124);
//Server listens on the port 8124
io = io.listen(server); 

var nw_data; 
/*initializing the websockets communication , server instance has to be sent as the argument */
io.sockets.on("connection",function(socket){
      
      //Clients will join the server in rooms/channels
      //rpsock is the routing processor socket
      //browswersock is the web browser socket
      socket.on('join', function (data) {
        console.log(data.socketid);
        socket.join(data.socketid);
        //If browser just joined - send it the DOGE network info
        if (data.socketid === 'browsersock') {
          io.to('browsersock').emit("load_network",nw_data);
          console.log('browser connected - sending load n/w data');

        }
 
      });
      var ack_to_client = {
          data:"Server Received the message"
        }
      io.to('browsersock').emit(JSON.stringify(ack_to_client));
  
    console.log('Socket.io Connection with the client established');
    
    //Handle incoming messages from the browser
    socket.on("message",function(data){
        /*This event is triggered at the server side when client sends the data using socket.send() method */
        io.to('rpsock').emit (data) 
        console.log(data);
    }); 

    //Routing Processor will send the network info as a 'load network message'
    //Handle load network message to render network on vis
    socket.on("load_network",function(data){
        console.log("Received load network");
        nw_data = data;
        //Debug
         //data = JSON.parse(data);
         //console.log(data);
        var ack_to_client = {
          data:"Server Received the message"
        }
        io.to('rpsock').emit('ack',JSON.stringify(ack_to_client));
    });
 
});
