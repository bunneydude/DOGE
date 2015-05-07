// Generated by CoffeeScript 1.6.3
(function() {
  var app, fs, generateChartData, getUpdateData, io,  server, __dirname;

  var express = require ('express');
 
  var request = require('request');

  app = express();
  
 

  app.use(require('express').static('../../webserver'));



  fs = require('fs');

  __dirname = '';


  var webserver = app.listen(8000,function(){
    console.log("Server started on port 8000");
   });

  var chartserver = app.listen(3000, function() {
  });

  chart_io = require('socket.io')(chartserver);

  var graphserver = app.listen(4000, function() {
  });

  graph_io = require('socket.io')(graphserver);

 
   generateChartData = function() {
    var lineChartData;
    return lineChartData = [
      {
        label: 'node2',
        datapoints: []
      }, {
        label: 'node3',
        datapoints: []
      }, {
        label: 'node4',
        datapoints: []
      }, {
        label: 'node5',
        datapoints: []
      }, {
        label: 'node6',
        datapoints: []

      }  , {
        label: 'node7',
        datapoints: []

      }   , {
        label: 'node8',
        datapoints: []

      }   , {
        label: 'node9',
        datapoints: []

      }   , {
        label: 'node10',
        datapoints: []

      }    
    ];
  };

  

chart_io.sockets.on('connection', function(socket) {
   
    socket.on('join', function (data) {
        socket.join(data.socketid);
    });
 
    chart_io.to('chart').emit('init', generateChartData());
    console.log('connected');
   
     
    socket.on('updateComponent', function(data) {});

    socket.on('update',function(data){
        console.log ('Recvd data from python');
        data = JSON.parse(data);
        //console.log (data);
        chart_io.to('chart').emit('update',data);

    });


  });


var nw_data; 
var sent_nw_data = 0;

/*initializing the websockets communication , server instance has to be sent as the argument */
graph_io.sockets.on("connection",function(socket){
      
    //Clients will join the server in rooms/channels
    //rpsock is the routing processor socket
    //browswersock is the web browser socket
    socket.on('join', function (data) {
        console.log(data.socketid);
        socket.join(data.socketid);
        //If browser just joined - send it the DOGE network info
        if (data.socketid === 'browsersock' && sent_nw_data == 0) {
          graph_io.to('browsersock').emit("load_network",nw_data);
          console.log('browser connected - sending load n/w data');
          sent_nw_data = 1;
        }
 
    });


    console.log('Socket.io Connection with the client established');
    
    //Handle incoming messages from the browser
    socket.on("message",function(data,callback){
        console.log('Received request from browser:');
        console.log(data);
        graph_io.to('rpsock').emit(data);
    }); 

    //Routing Processor will send the network info as a 'load network message'
    //Handle load network message to render network on vis
    socket.on("load_network",function(data){
        console.log("Received load network");
        nw_data = data;
        sent_nw_data  = 0;
        //Debug
        //data = JSON.parse(data);
        //console.log(data);
    });

    socket.on("confirm",function(data){
        graph_io.to('browsersock').emit("confirm",data);
        console.log("Received confirmation from RP:");
       // Debug
         data = JSON.parse(data);
         console.log(data);
    });
 
});
}).call(this);
