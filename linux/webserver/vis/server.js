var express = require('express');
var http = require('http');
var io = require('socket.io');
 
 
var app = express();
app.use(express.static('./public'));
//Specifying the public folder of the server to make the html accesible using the static middleware
 
var server =http.createServer(app).listen(8124);
//Server listens on the port 8124
io = io.listen(server); 
/*initializing the websockets communication , server instance has to be sent as the argument */

 
io.sockets.on("connection",function(socket){
      
      socket.on('join', function (data) {
        console.log(data.socketid);
        socket.join(data.socketid); 
      });
        
      /*sending data to the client , this triggers a message event at the client side */
    console.log('Socket.io Connection with the client established');
    
    //Handle incoming messages
    socket.on("message",function(data){
        /*This event is triggered at the server side when client sends the data using socket.send() method */
        io.to('rpsock').emit (data) 
        console.log(data);
    }); 

    //Handle load network message to render network on vis
    socket.on("load_network",function(data){
        io.to('browsersock').emit(data);
        data = JSON.parse(data);
        console.log("Received load network");
        console.log(data);
        var ack_to_client = {
          data:"Server Received the message"
        }
        io.to('rpsock').emit('ack',JSON.stringify(ack_to_client));
    });
 
});
