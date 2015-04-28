(function(){
  var app, fs, generateChartData, io, server, __dirname;

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


  });


}).call(this);

