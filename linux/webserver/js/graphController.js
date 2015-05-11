'use strict';

var nodes;
var edges;
var routing_edges;
var network;

var hidden_edges = [];

/* Controllers */
angular.module('DeviceManager.graphController', []).
  controller('graphController', ['$scope', '$window',  function ($scope, $window) {


    //These should be global so that all functions can modify network properties
    var userRoute = [];
    var delRoute = [];
    var userEdge = [];
    var userNode = "";
    var newRoute = [];
    var options;
    var mode ;
    var legend_toggle = 1;
    var socket;
 
    var ipaddr = location.hostname;
    socket = io.connect('http://'+ipaddr+':4000');
    socket.emit('join',{'socketid':'browsersock'});
    draw(nodes,edges);


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
       var nodesJsonObj = JSON.parse(message.nodes);
       var edgesJsonObj = JSON.parse(message.edges);
       var routingEdgesJsonObj = JSON.parse(message.routing_edges);
       
       nodes = new vis.DataSet();
       edges = new vis.DataSet();
       routing_edges = new vis.DataSet();

       var nodesArrayLength = nodesJsonObj.length;

       //x,y coordinates for the nodes
       var node_x = 0;
       var node_y = 0;
       
       //This is Edison  
       var node = {'group':nodesJsonObj[nodesArrayLength-1].group,'id':JSON.stringify(nodesJsonObj[nodesArrayLength-1].id),'label':JSON.stringify(nodesJsonObj[nodesArrayLength-1].label),'x':200,'y':70};
       nodes.add(node);
 
       for (var i=0;i < nodesArrayLength-1; i++){
          node = {'group':nodesJsonObj[i].group,'id':JSON.stringify(nodesJsonObj[i].id),'label':JSON.stringify(nodesJsonObj[i].label),'x':node_x,'y':node_y};
         node_x += 200;
         if (node_x == 600) {
            node_x = 0;
            node_y -= 100;
         };
          nodes.add(node);
       }
       
       var edgesArrayLength = edgesJsonObj.length;

       for (var i=0;i < edgesArrayLength; i++){
          var edge = {'from':JSON.stringify(edgesJsonObj[i].from),'to':JSON.stringify(edgesJsonObj[i].to),'label':JSON.stringify(edgesJsonObj[i].label),'id':JSON.stringify(edgesJsonObj[i].id),'style':'arrow','arrowScaleFactor': .5,};
          edges.add(edge);
       }

       var routingEdgesArrayLength = routingEdgesJsonObj.length;

       for (var i=0;i < routingEdgesArrayLength; i++){
          var route_edge = {'from':JSON.stringify(routingEdgesJsonObj[i].from),'to':JSON.stringify(routingEdgesJsonObj[i].to),'label':JSON.stringify(routingEdgesJsonObj[i].label),'id':JSON.stringify(routingEdgesJsonObj[i].id),'style':'arrow','arrowScaleFactor': .5,'color':'gold'};
          edges.add(route_edge);

       }
      
       //Filter out routing edges (arrow). We will add these back based on user feedback 
       routing_edges = edges.get({
         filter: function (item) {
          return (item.color === 'gold');
         }
       });
       edges.remove(routing_edges);
      
       draw(nodes,edges);
       $scope.drawLegend();
     })


     function getNodeColor (node_group) {
       var node_color;
       if (node_group == '915mhz') {
         node_color = 'lightblue';
       }
       else if (node_group == '2.4ghz') {
         node_color = 'lightgreen';
       }
       else if (node_group == '433mhz') {
         node_color = 'tomato';
       }
       else if (node_group == 'edison') {
         node_color = 'gold';
       }
       else { node_color = 'lightgreen'}

       return node_color;
    }

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
   

       
    $scope.toggleLegend = function() {
     
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
    
    
    $scope.drawLegend = function(){ 
      var nodes_legend = new vis.DataSet();
      nodes_legend.add({id: 1000, x:-3500,y:0,label: 'Edison', group: 'edison'});
      nodes_legend.add({id: 1001, x:-3400,y:0,label: '433MHz', group: '433mhz'});
      nodes_legend.add({id: 1002, x:-3300,y:0,label: '915MHz', group: '915mhz'});
      nodes_legend.add({id: 1003, x:-3200,y:0,label: '2.4GHz', group: '2.4ghz'});
      nodes_legend.add({id: 1004, x:-3080,y:0,label: 'Masked Node', color: 'gray'});
      nodes_legend.add({id: 1005, x:-2925,y:0,label: 'Hidden Edges', color:{border:'red'}});
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
      
      var network_legend = new vis.Network(container, data, options_legend);
    }


     $scope.toggleNodeRoutes = function () {

       //Clear any previous use cases
      $scope.clearUserBox();
 
      document.getElementById('instructions').innerHTML =  'Select source or destination node for which you want to show/hide the routing edges';
     
 
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';

      network.on('select',function(params) {
              mode = 'toggle-routes';
              document.getElementById('route').innerHTML = 'Selected Node:'+params.nodes;
              userEdge = params.edges;
              userNode = parseInt(params.nodes);
      });
   }
 
    $scope.toggleNodeEdges = function () {

       //Clear any previous use cases
      $scope.clearUserBox();
 
     
      document.getElementById('instructions').innerHTML =  'Select node for which you want to show/hide neighbor edges';
 
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';

      network.on('select',function(params) {
              mode = 'toggle-edges';
              document.getElementById('route').innerHTML = 'Selected Node:'+params.nodes;
              userEdge = params.edges;
              userNode = parseInt(params.nodes);
      });
   }

    
    // convenience method to stringify a JSON object
    function toJson (obj) {
      return JSON.stringify(obj); 
    }

  
    $scope.defaultEdgeView = function () {

      var edgeObj;
      
      //Un-hide all hidden neighbor edges
      for (var i = 0; i < hidden_edges.length; i++) {
        edgeObj = hidden_edges[i];
        edges.add(edgeObj);
      }
       
      hidden_edges = [];

      //Change all node borders that were red back to blue
      var nodeArr = nodes.get();
      for( var i=0; i< nodeArr.length; i++ ) {
        var node_color = getNodeColor(nodeArr[i].group);
        nodes.update({id: nodeArr[i].id,color:{background:node_color, border:'blue'}});
      }        

      //Hide all route edges
      routing_edges.forEach( function(edgeObj) {
              edges.remove(edgeObj.id);
      })



    }
    
    $scope.addRoute = function () {
     
      //Clear any previous use cases
      $scope.clearUserBox();
 
      mode = 'add-route';
      
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
   
     
    $scope.reqNewRoute = function() {
      
      //Clear any previous use cases
      $scope.clearUserBox();
      
      mode = 'req-new-route';
      
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

    $scope.deleteRoute = function() {
      
      //Clear any previous use cases
      $scope.clearUserBox();

      mode = 'delete-route';
      
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
    
    //Mask or unmask a node or edge
    $scope.elementMasking = function(option) {
     
      //Clear any previous use cases
      $scope.clearUserBox();

      mode = option;
             
      //Display user instructions and selections
      document.getElementById('instructions').innerHTML = 'Select Node/Edge to be '+option+'ed. Click Confirm when done or Cancel to exit';
      
      
      //Make user confirmation buttons visible
      var div = document.getElementById('user-confirm');
      div.style.display = 'block';
      
      network.on('select',function(params) {
        if (params['edges'] != "" && params['nodes'] == "") {  
          document.getElementById('route').innerHTML = 'Selected Edge Id\'s:'+params.edges;
          userEdge = params.edges ;
          
        }
        else if (params['nodes'] != "" && params['edges'] == "") {  
          document.getElementById('route').innerHTML = 'Selected Nodes:'+params.nodes;
	  userNode = parseInt(params.nodes) ;
        }
        else if (params['nodes'] != "" && params['edges'] != "") {  
          document.getElementById('route').innerHTML = 'Selected Nodes:'+params.nodes + " Selected Edge Id\'s:"+params.edges;
         userEdge = params.edges;
         userNode = parseInt(params.nodes);
        }



       });
    }
    
    
    $scope.clearUserBox = function() {
      document.getElementById('selection').innerHTML = "";
      document.getElementById('instructions').innerHTML = "";
      document.getElementById('route').innerHTML = "";
      document.getElementById('user-confirm').style.display = 'none';
      document.getElementById('user-checkbox').style.display = 'none';
      userRoute = [];
      delRoute = [];
      newRoute = [];
      userEdge = [];
      userNode = "";
      mode = "";
      network.off ('select');
     
    }
    
    //Function to update edge properties if masked
    function updateEdge (masking_request,id) {

      if (masking_request == 'mask') {
          try {
              edges.update({id: id, style:'dash-line',color:'gray'});
          }
          catch(err) {
              alert (err);
          } 
      }
      else if (masking_request == 'unmask') {
          try {
              edges.update({id: id, style:'line',color:'black','style':'arrow'});
          }
          catch(err) {
              alert (err);
          } 
      }

    }
    
    $scope.saveUserData = function () {
  
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
      else if (mode == 'mask' || mode == 'unmask' ){

        
        document.getElementById('selection').innerHTML = mode+" Node/Edge edge requested for Edges:" +  userEdge +" Nodes:"+ userNode;

        //If a node was selected, mask it
        if (userNode != "") {
            var command = mode+'_node';
            var message = {'command':command,'data': userNode};
            sendMessageWait(message);
        }

        //If selection had only edges, mask it
        if (userEdge != null && userNode == "") {
          var command = mode+'_physical_edge';
          var id;
          if (userEdge.length > 1) {
            userEdge.forEach( function(id) {
              id = parseInt(id);
              var edgeObj = edges.get(id);
              var message = {'command' : command, 'data' : [parseInt(edgeObj.from),parseInt(edgeObj.to),id]};
              sendMessage(message);
            });
          } 
          else {
              id = parseInt(userEdge);
              var edgeObj = edges.get(id);
              var message = {'command' : command, 'data' : [parseInt(edgeObj.from),parseInt(edgeObj.to),id]};
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
         else if (message.command == 'unmask_node') {
           try {
             var node_group = nodes.get(parseInt(message.data)).group;
             var node_color = getNodeColor(node_group);

             nodes.update({id: parseInt(message.data),color:{background:node_color, border:'blue'}});
           } 
           catch(err) {
            alert (err);
           }
         }
 
         else if (message.command == 'mask_physical_edge') {
           //Format of array is [from,to,id]
           var edge_array = /\[(\d+),(\d+),(\d+)\]/.exec(message.data);
           //update edge based on id
           updateEdge('mask',edge_array[3]);
         }
        
         else if (message.command == 'unmask_physical_edge'  ) {
           //Format of array is [from,to,id]
           var edge_array = /\[(\d+),(\d+),(\d+)\]/.exec(message.data);
           //update edge based on id
           updateEdge('unmask',edge_array[3]);
         }
 
         else if (message.command == 'alert') {
           document.getElementById('instructions').innerHTML = 'Message from Routing Processor Recvd: ' + message.data;
         }
     
       });

      }
      
      else if (mode == 'toggle-edges'  ) {
          var id;
	  var edge_view = "show-edges";
          
          //If user selected edges only has routes - this is the case where edges are hidden. 
          //So unhide edges now
          if (userEdge.length > 1) {
            userEdge.forEach( function(id) {
              if (edges.get(id).color != 'gold') 
               {
                 edge_view = "hide-edges";
                }
            });    
          }
          else {
            edge_view = "show-edges";
          }             
         

          if (edge_view == 'hide-edges') {
          
            //Hide all edges for this node and add it to the hidden edges array
            if (userEdge.length > 1) {
              userEdge.forEach( function(id) {
                id = parseInt(id);
                var addHiddenEdge = 1;
                //Only add to hidden edges if it  previously didnt exist
                for (var i = 0; i < hidden_edges.length; i++) {
	          if (parseInt(hidden_edges[i].id) === id) {
                    addHiddenEdge = 0;
                  }
                }

                //Do not hide routes in this mode. This is only for neighbor edges
                if (edges.get(id).color === 'gold') {
                  addHiddenEdge = 0;
                }
                if (addHiddenEdge) {
                  hidden_edges.push (edges.get(id));
                  edges.remove(id);
                }
              });
            } 
          
          //Get original color of the node
          var node_group = nodes.get(userNode).group;
          var node_color = getNodeColor(node_group);

          //Add red border to indicate hidden edges
          nodes.update({id:userNode,color:{border:'red'}});

          }
      
          else if (edge_view == 'show-edges'  ) {
      
            //Go through all elements of hidden edge array and check for edges which have user selected node
            //If edge found, add edge to network view and remove the edge from the hidden edge array. 
            for (var i = hidden_edges.length-1; i >= 0 ; i--){ 
              edgeObj = hidden_edges[i];
              if ( parseInt(edgeObj.from) == userNode || parseInt(edgeObj.to) == userNode) {
                //Add to network view
                edges.add(edgeObj);
                //Remove edge from the array
                hidden_edges.splice(i, 1);
              }
            }

            //Get original color of the node
            var node_group = nodes.get(userNode).group;
            var node_color = getNodeColor(node_group);

            //Add blue border to indicate viewable edges
            nodes.update({id:userNode,color:{background: node_color, border:'blue'}});
          }
      }

      else if (mode == 'toggle-routes'  ) {


        //Go through all route edges 
        routing_edges.forEach( function(edgeObj) {
          //Check if user selected node is part of to/from of current route edge
          if ( parseInt(edgeObj.from) == userNode || parseInt(edgeObj.to) == userNode) {
            
            //If routing edge isnt already visible - add it to the view
            if ( edges.get(edgeObj.id) == null) {
              edges.add(edgeObj);
            }
            else {
              edges.remove(edgeObj);
            }
          }
        })
      }

      
      
      $scope.clearUserBox();
    }
    
    function draw(nodes,edges) {
    var e=document.getElementsByName("Network Filter")[0];

  

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
   
    dragNetwork: true,
    zoomable: true,
    
    
    //Enable Navigation controls
    navigation: true,
    keyboard: true,  
    
    dataManipulation: {
     enabled: true,
     initiallyVisible: true
    },
    
    edges: {
     width: 2,
     widthSelectionMultiplier: 2,
     fontFace: 'verdana',
     fontSize: 14,
     color : {
      highlight: 'blue',
      color: 'black',
     },
    },
    //Define groups to be used for nodes
    groups: {
     '433mhz': {
      shape: doge_node_shape,
      color: {
        background:'tomato',
        border: 'blue',
        highlight: {
           background: 'tomato',
         }
      }
     },
     '915mhz': {
       shape: doge_node_shape,
       color: {
         background: 'lightblue',
         border: 'blue',
         highlight: {
           background: 'lightblue',
         }
       }
     },
     '2.4ghz': {
      shape: doge_node_shape,
      color: {
        background: 'lightgreen',
        border: 'blue',
        highlight: {
           background: 'lightgreen',
         }
      }
     },
     'edison': {
      shape: doge_node_shape,
      color: { 
        background: 'gold',
        border: 'blue',
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



  }]);



