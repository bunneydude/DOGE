    

    //These should be global so that all functions can modify network properties
    var nodes = [];
    var edges = [];
    var routing_edges = [];
    var network = null;
    var userRoute = [];
    var delRoute = [];
    var maskEdge = [];
    var maskNode = "";
    var newRoute = [];
    var options;
    var mode ;
    var route_toggle = 1;
    var legend_toggle = 1;
    var socket;
    var connection;

  $(document).ready(function(){
 
    socket = io.connect();
    socket.emit('join',{'socketid':'browsersock'});

    document.getElementById('selection').innerHTML = "Loaded browser";
     
    socket.on('connect', function() {
      console.log('connected');
    });
    socket.on('disconnect', function() {
      console.log('disconnected');
    });
    socket.on('error', function(err) {
      if(err === 'handshake error') {
        console.log('handshake error', err);
      } else {
        console.log('io error', err);
      }
    });
  
    socket.on ("load_network",function(message){
      
       message = JSON.parse(message);
       var nodesJsonObj = $.parseJSON(message.nodes);
       var edgesJsonObj = $.parseJSON(message.edges);
       var routingEdgesJsonObj = $.parseJSON(message.routing_edges);
       
       nodes = new vis.DataSet();
       edges = new vis.DataSet();
       routing_edges = new vis.DataSet();

       var nodesArrayLength = nodesJsonObj.length;

       for (var i=0;i < nodesArrayLength; i++){
          node = {'group':JSON.stringify(nodesJsonObj[i].group),'id':JSON.stringify(nodesJsonObj[i].id),'label':JSON.stringify(nodesJsonObj[i].label)};
          nodes.add(node);
       }
       
       var edgesArrayLength = edgesJsonObj.length;

       for (var i=0;i < edgesArrayLength; i++){
          edge = {'from':JSON.stringify(edgesJsonObj[i].from),'to':JSON.stringify(edgesJsonObj[i].to),'label':JSON.stringify(edgesJsonObj[i].label),'id':JSON.stringify(edgesJsonObj[i].id)};
          edges.add(edge);
       }

       var routingEdgesArrayLength = routingEdgesJsonObj.length;

       for (var i=0;i < routingEdgesArrayLength; i++){
          route_edge = {'from':JSON.stringify(routingEdgesJsonObj[i].from),'to':JSON.stringify(routingEdgesJsonObj[i].to),'label':JSON.stringify(routingEdgesJsonObj[i].label),'id':JSON.stringify(routingEdgesJsonObj[i].id),'style':'dash-line'};
          edges.add(route_edge);

       }
      
       //Filter out routing edges (dash-line edges). We will add these back based on user feedback 
       routing_edges = edges.get({
         filter: function (item) {
          return (item.style === 'dash-line');
         }
       });
       edges.remove(routing_edges);
       
       draw(nodes,edges);
       drawLegend();
     })

  });



    //Socket function to send data to node.js server
    function sendMessage(data) {
      socket.send(toJson(data));
    }
   
    //Socket function to send data to node.js server
    function sendMessageWait(data) {
      var resp;
      socket.send(toJson(data),function(response){
        resp = response;
        console.log(response);
      document.getElementById('selection').innerHTML = "Response:"+resp;
         
     }); 

      return resp;
    }
   

       
    function toggleLegend () {
     
       var e = document.getElementById('mynetworklegend');
       if(legend_toggle) {
          e.style.display = 'none'; 
          legend_toggle=0;
       }
       else {
          e.style.display = 'block'; 
          legend_toggle=1;
       }

   
    }
    
    
    function drawLegend (){ 
      var nodes_legend = new vis.DataSet();
      nodes_legend.add({id: 1000, x:-3500,y:0,label: 'Edison', group: 'edison'});
      nodes_legend.add({id: 1001, x:-3400,y:0,label: '433MHz', group: '433mhz'});
      nodes_legend.add({id: 1002, x:-3300,y:0,label: '916MHz', group: '916mhz'});
      nodes_legend.add({id: 1003, x:-3200,y:0,label: '1.2GHz', group: '1.2ghz'});
      nodes_legend.add({id: 1004, x:-3080,y:0,label: 'Masked Node', color: 'gray'});
      var data = {
       nodes: nodes_legend,
      };
      
      var options_legend = {
        dragNetwork: false,
        groups: options.groups,
        zoomable: false,
      }
      // create a network
      var container = document.getElementById('mynetworklegend');
      
      network_legend = new vis.Network(container, data, options_legend);
    }


    
   function toggleRouteDisplay () {
     if (route_toggle) {
      edges.add(routing_edges);
      route_toggle = 0;
     }
     else {
      edges.remove(routing_edges);
      route_toggle = 1;
     }
   }

    
    // convenience method to stringify a JSON object
    function toJson (obj) {
      return JSON.stringify(obj); //, null, 4);
    }


    function addRoute() {
      mode = 'add-route';
      
      //Clear out states from any previous use cases
      document.getElementById('selection').innerHTML = "";
      //network.off ('select');
      
     //Display user instructions and selections
      document.getElementById('instructions').innerHTML = 'Select Source Node followed by intermediate node(s) (if any) and then the Destination Node. Click Confirm when done or Cancel to exit';
      document.getElementById('route').innerHTML = 'Route';
      
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';
      
      network.on('select',function(params) {
        if (params['nodes'] != "") {  
         document.getElementById('route').innerHTML +=  '->'+params.nodes;
         userRoute.push(parseInt(params.nodes)) ;
        }
       });
     }
    
    function reqNewRoute() {
      mode = 'req-new-route';
      
      //Clear out states from any previous use cases
      document.getElementById('selection').innerHTML = "";
      //network.off ('select');
      
      
      //Display user instructions and selections
      document.getElementById('instructions').innerHTML = 'Select Source Node followed by the Destination Node.Click Confirm when done or Cancel to exit';
      document.getElementById('route').innerHTML = 'New Route Request';
      
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';
      //Make checkbox visible
      document.getElementById('user-checkbox').style.display = 'block';
      
      network.on('select',function(params) {
        if (params['nodes'] != "") {  
         document.getElementById('route').innerHTML +=  '->'+params.nodes;
         newRoute.push(parseInt(params.nodes)) ;

         //Dont allow more than 2 nodes to be selected in this mode
         if ((newRoute.length > 2) && (mode == 'req-new-route')) {
           alert('Error - You selected more than 2 nodes');
           newRoute = [];
           params.nodes = "";
           document.getElementById('route').innerHTML = 'New Route Request';
           
         }
        }
       });
    
    }
    function deleteRoute() {
      mode = 'delete-route';
      
      //Clear out states from any previous use cases
      document.getElementById('selection').innerHTML = "";
      //network.off ('select');
      
      //Display user instructions and selections
      document.getElementById('instructions').innerHTML = 'Select Source Node followed by the Destination Node.Click Confirm when done or Cancel to exit';
      document.getElementById('route').innerHTML = 'Delete Route';
      
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';
      
      network.on('select',function(params) {
         if (params['nodes'] != "") {  
         document.getElementById('route').innerHTML +=  '->'+params.nodes;
         delRoute.push(parseInt(params.nodes)) ;

         //Dont allow more than 2 nodes to be selected in this mode
         if ((delRoute.length > 2) && (mode == 'delete-route')) {
           alert('Error - You selected more than 2 nodes');
           delRoute = [];
           params.nodes = "";
           document.getElementById('route').innerHTML = 'Delete Route';
           
         }
        }
       });
    
    }
    
    //Mask node or edge
    function maskElement() {
      mode = 'mask-element';
      
      //Clear out states from any previous use cases
      document.getElementById('selection').innerHTML = "";
      //network.off ('select');
             
      //Display user instructions and selections
      document.getElementById('instructions').innerHTML = 'Select Node/Edge to be masked.Click Confirm when done or Cancel to exit';
      
      
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';
      
      network.on('select',function(params) {
        if (params['edges'] != "") {  
          maskEdge = params.edges ;
          
        }
        if (params['nodes'] != "") {  
          maskNode = parseInt(params.nodes) ;
        }
       });
    }
    
    
    function clearUserBox () {
      document.getElementById('instructions').innerHTML = "";
      document.getElementById('route').innerHTML = "";
      document.getElementById('user-confirm').style.display = 'none';
      document.getElementById('user-checkbox').style.display = 'none';
      userRoute = [];
      delRoute = [];
      newRoute = [];
      maskEdge = [];
      maskNode = "";
      mode = "";
      properties = [];
      network.off ('select');
     
    }
    
    //Function to update edge properties if masked
    function updateEdge (id) {
      try {
        edges.update({id: id, style:'dash-line',color:'gray'});
      }
      catch(err) {
        alert (err);
      } 
    }
    
    function saveUserData () {

      if (mode == 'add-route') {
         document.getElementById('selection').innerHTML = "User defined route requested = " +userRoute;
          var message = {'command': 'add_route', 'data': userRoute};
          sendMessage(message);
      }
      else if (mode == 'delete-route'){
        document.getElementById('selection').innerHTML = "Route deletion requested = " +delRoute;
        var message = {'command':'delete_route','data': delRoute};
        sendMessage(message);

      }
      else if (mode == 'req-new-route'){
        if (document.getElementById('useMaskRouteBox').checked == true) {
          //Add true/false string to array of nodes
          newRoute.push("true");
	  document.getElementById('selection').innerHTML = "New route requested = " +newRoute+" Allow masked route";
          //Clear out user selection
          document.getElementById('useMaskRouteBox').checked = false;
          
          var message = {'command':'request_new_route','data': newRoute};
          sendMessage(message);

         }
        else {
          document.getElementById('selection').innerHTML = "New route requested = " +newRoute;
	  //Add true/false string to array of nodes
          newRoute.push("false");

	  var message = {'command':'request_new_route','data': newRoute};
          sendMessage(message);

        }
      }
      else if (mode == 'mask-element'){

        
        document.getElementById('selection').innerHTML = "Mask Node/Edge edge requested for Edges:" +  maskEdge +" Nodes:"+ maskNode;

        //If a node was selected, mask it
        if (maskNode != "") {
            var message = {'command':'mask_node','data': maskNode};
            sendMessageWait(message);
        }

        //If selection had only edges, mask it
        if (maskEdge != null && maskNode == "") {
          if (maskEdge.length > 1) {
            maskEdge.forEach( function(id) {
              id = parseInt(id);
              var edgeObj = edges.get(id);
              var message = {'command' : 'mask_physical_edge', 'data' : [parseInt(edgeObj.from),parseInt(edgeObj.to),id]};
              sendMessage(message);
            });
          } 
          else {
              id = parseInt(maskEdge);
              var edgeObj = edges.get(id);
              var message = {'command' : 'mask_physical_edge', 'data' : [parseInt(edgeObj.from),parseInt(edgeObj.to),id]};
              sendMessage(message);
          }
       }
              
       socket.on ("confirm", function(message){
       
         message = JSON.parse(message); 
         document.getElementById('instructions').innerHTML = 'Confirmation Message from Routing Processor Recvd: Command='+message.command+' Data='+message.data;
         
         if (message.command == 'mask_node') {
           try {
            nodes.update({id: parseInt(message.data), color: 'gray'});
           }
           catch(err) {
            alert (err);
           }
         }
   
         else if (message.command == 'mask_physical_edge') {
           var edge_array = /\[(\d+),(\d+),(\d+)\]/.exec(message.data);
           updateEdge(edge_array[3]);
         }

         else if (message.command == 'alert') {
           document.getElementById('instructions').innerHTML = 'Message from Routing Processor Recvd: ERROR - Cannot mask this node because it has routing tables';
           //alert(message.data);
         }
     
       });

      }

      
      
      clearUserBox();
    }
    
    function draw(nodes,edges) {
    var e=document.getElementsByName("Network Filter")[0];

    //nodes = new vis.DataSet();
    //nodes.add ( [
   //  {'id': '0', 'label': 'E',group:'edison'},
    // {'id': '1', 'label': '1',group:'916mhz'},
    // {'id': '2', 'label': '2',group:'433mhz'},
    // {'id': '3', 'label': '3',group:'433mhz'},
    // {'id': '4', 'label': '4',group:'433mhz'},
    // {'id': '5', 'label': '5',group:'916mhz'},
    // {'id': '6', 'label': '6',group:'1.2ghz'},
   // ]);

   // edges = new vis.DataSet();
    //edges.add([
    // {'id': '1',from: '0', to: '1',label:'12'},
    // {'id': '2',from: '0', to: '1',label:'50'},
    // {'id': '3',from: '0', to: '2',label:'60'},
    // {'id': '4',from: '2', to: '3',label:'49'},
    // {'id': '5',from: '2', to: '4',label:'73'},
    // {'id': '6',from: '0', to: '5',label:'73'},
    // {'id': '7',from: '5', to: '6',label:'73'},
   // ]);

  

    var grpfilt =document.getElementsByName("Network Filter")[0];
    var group_filter = grpfilt.value;

    //If this is not a full network view - filter based on user request
    if (group_filter != 'Full') {

     //Filter by radio type
     var radio_group_nodes = nodes.get({
      filter: function (item) {
      return (item.group == group_filter);
      }
     });

     //Define updated network of nodes & edges
     var e = nodes.get({
      filter: function (item) {
       return (item.group == 'edison');
      }
     });


    var radio_group_edges = edges.get({
     filter: function (item) {
      return (item.group == group_filter);
     }
    });


    var data = {
     nodes: radio_group_nodes,
     edges: radio_group_edges
    };

    } //End of if filter is not full network 

    //If full network - display all nodes

    else {
     var data = {
      nodes: nodes,
      edges: edges
     };
    };


    // create a network
    var container = document.getElementById('mynetwork');


    //Define shape to be used for all nodes here
    var doge_node_shape = "ellipse";


    options = {
   
    dragNetwork: false,
    zoomable: false,
    
    dataManipulation: {
    enabled: true,
    initiallyVisible: true
    },
    
    edges: {
     width: 2,
     widthSelectionMultiplier: 2,
     fontFace: 'verdana',
     fontSize: 14,
     inheritColor: true,
     //style: line,
     color : {
      highlight: 'blue',
      color: 'black',
     },
     //labelAlignment: 'line-above'
    },
    //Define groups to be used for nodes
    groups: {
     '433mhz': {
      shape: doge_node_shape,
      color: {
        background:'tomato',
        highlight: {
           background: 'tomato',
         }
      }
     },
     '916mhz': {
       shape: doge_node_shape,
       color: {
         background: 'lightblue',
         highlight: {
           background: 'lightblue',
         }
       }
     },
     '1.2ghz': {
      shape: doge_node_shape,
      color: {
        background: 'lightgreen',
        highlight: {
           background: 'lightgreen',
         }
      }
     },
     'edison': {
      shape: doge_node_shape,
      color: { 
        background: 'gold',
        highlight: {
           background: 'gold',
         }
      }
     },
    },
    nodes: {
     borderWidthSelected: 3,
    },
    locales: {
     // create a new locale (text strings should be replaced with localized strings)
     mylocale: {
      edit: 'Edit',
      del: 'Delete Edge',
      back: 'Back',
      addNode: 'Add Node',
      addEdge: 'Add Edge',
      editNode: 'Edit Node',
      editEdge: 'Edit Edge',
      addDescription: 'This operation is currently not supported. Click Back to exit',
      edgeDescription: 'Click on a node and drag the edge to another node to connect them.',
      editEdgeDescription: 'This operation is currently not supported. Click Back to exit', //'Click on the control points and drag them to a node to connect to it. Click Back when done',
      createEdgeError: 'Cannot link edges to a cluster.',
      deleteClusterError: 'Clusters cannot be deleted.'
     }
    },

    // use the new locale
    locale: 'mylocale',
    
    //Handle vis data manipulation functions
    
    //onConnect is used for adding edges
    onConnect:function(data,callback) {
      document.getElementById('selection').innerHTML = 'Edge Selection from: ' + data.from +' to: '+ data.to ;
      var message = {'command':'add_edge', 'data' : [parseInt(data.from),parseInt(data.to)]};
      sendMessage(message);
      callback(data);
    
    },
    onAdd: function(data,callback) {
     //Adding node not supported right now
      data = "";
      callback(data);
    },
    onEditEdge: function(data,callback) {
      // data = {id: edgeID,  from: nodeId1,to: nodeId2};
      //document.getElementById('selection').innerHTML = 'This operation is currently not supported';
      data = ""; 
      callback(data);
    },
    onDelete: function(data,callback) {
     // data = {nodes: [selectedNodeIds], edges: [selectedEdgeIds]};
     //Only allow edge deletion
     if (data.nodes == "") {
       var r=confirm("Are you sure you want to delete this edge?");
       if (r==true) {
         var edgeArray = edges.get(data.edges);
         var message = {'command' : 'delete_edge', 'data' : [parseInt(edgeArray[0].from),parseInt(edgeArray[0].to),parseInt(edgeArray[0].id)]};
         sendMessage(message);
         callback(data);
       }
       else {
         //Clear out deleted edges/nodes
         data.nodes = "";
         data.edges = "";
         callback(data);
       }
     }
     else {
       alert('ERROR: You selected a node while trying to mask an edge - Please retry by just selecting edges ');
       data.nodes = "";
       data.edges = "";
       callback(data);
     }
    },

   
   }; //End of options

    network = new vis.Network(container, data, options);

   
    network.on("resize", function(params) {console.log(params.width,params.height)});

   
    } // End of draw() function


