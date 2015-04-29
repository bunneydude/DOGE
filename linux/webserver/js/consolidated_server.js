(function(){
  var app, fs, generateChartData, io, server, __dirname;

var nw_data; 
var recvd=0;
var loop = 1;

express = require('express');
var http = require('http');
io = require('socket.io');
 
fs = require('fs');
app = express();

app.use(express.static('./webserver/'));
//Specifying the public folder of the server to make the html accesible using the static middleware
 
var server =http.createServer(app).listen(8000);
io = io.listen(server); 

app.get('/', function(req, res) {
    console.log('recieved request');
    return fs.readFile(__dirname + '../index.html', function(err, data) {
      if (err) {
        console.log('error');
        res.writeHead(500);
        return res.end('error loading index.html');
      } else {
        console.log('success');
        res.writeHead(200);
        return res.end(data);
      }
    });
  });



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

  

io.sockets.on('connection', function(socket) {
   
    socket.on('join', function (data) {
        socket.join(data.socketid);
        console.log (data.socketid);
        //If browser just joined - send it the DOGE network info
        if (data.socketid === 'browsersock') {
          io.to('browsersock').emit("load_network",nw_data);
          console.log('browser connected - sending load n/w data');

        }
 
      });
 
    io.to('chart').emit('init', generateChartData());
    console.log('connected');
    
    socket.on('updateComponent', function(data) {});

    socket.on('update',function(data){
        console.log ('Recvd data from python');
        data = JSON.parse(data);
        //console.log (data);
        io.to('chart').emit('update',data);

    });

    //Handle incoming messages from the browser
    socket.on("message",function(data,callback){
        console.log('Received request from browser:');
        console.log(data);
        io.to('rpsock').emit(data);
    }); 

    //Routing Processor will send the network info as a 'load network message'
    //Handle load network message to render network on vis
    socket.on("load_network",function(data){
        console.log("Received load network");
        nw_data = data;
        //Debug
        // data = JSON.parse(data);
         //console.log(data);
        var ack_to_client = {
          data:"Server Received the message"
        }
        io.to('rpsock').emit('ack',JSON.stringify(ack_to_client));
    });

    socket.on("confirm",function(data){
        io.to('browsersock').emit("confirm",data);
        console.log("Received confirmation from RP:");
       // Debug
         data = JSON.parse(data);
         console.log(data);
    });


  });


}).call(this);

