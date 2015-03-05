app =require('express')();
server = require('http').createServer(app)
io = require('socket.io')(server)
fs= require('fs')
__dirname=''
 
server.listen 8000, -> 
    console.log 'listening'

app.get '/', (req, res)->
    console.log 'recieved request'
    fs.readFile __dirname + '../index.html', (err,data)->
        if err
            console.log 'error'
            res.writeHead 500
            res.end 'error loading index.html'
        else
            console.log 'success'
            res.writeHead 200
            res.end data
            
rollDice = -> 
    return Math.floor( Math.random() * ( 0 + 200 - 1 ) ) + 1;
      
generateChartData = ->
 
     lineChartData = [
        {
          label: 'test1',
          datapoints: [
            {
              timestamp: 1383230753238,
              count: rollDice()
            },
            {
              timestamp: 1383230784165,
              count: rollDice()
            },
            {
              timestamp: 1383230821680,
              count: rollDice()
            }
          ]
        },
        {
          label: 'test2',
          datapoints: [
            {
              timestamp: 1383230753238,
              count: rollDice()
            },
            {
              timestamp: 1383230784165,
              count: rollDice()
            },
            {
              timestamp: 1383230821680,
              count: rollDice()
            }
          ]
        },
        {
          label: 'test3',
          datapoints: [
            {
              timestamp: 1383230753238,
              count: rollDice()
            },
            {
              timestamp: 1383230784165,
              count: rollDice()
            },
            {
              timestamp: 1383230821680,
              count: rollDice()
            }
          ]
        }
      ]
getUpdateData =(timestamp)->
    [
        [
            timestamp
            rollDice()
        ],
        [
            timestamp
            rollDice()
        ],
        [
            timestamp
            rollDice()
        ]
    ]
    
io.on 'connection', (socket) ->
    timestamp= 1383230821680
    ### this is just for testing it needs to be compiled with actual state of each pin  this could be done via a local database###
    socket.emit 'init', generateChartData()
    console.log 'connected'
    int= setInterval ->
            timestamp +=  10000
            socket.emit 'update', getUpdateData(timestamp)
            console.log 'update Sent'
        ,10000;
    socket.on 'updateComponent', (data)->
    socket.on 'disconnect', ->
        clearInterval int
        ### here is where you pu in all the magic to update the network device###