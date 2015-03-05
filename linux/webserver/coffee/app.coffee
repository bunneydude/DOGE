### debugmode allows to acccess a local component list with out the need for server access ###
debugMode=false

### setting up the name of the Contorller###
app  = angular.module 'DeviceManager', ['ngAnimate', 'ngRoute','ngResource','ngSanitize',  'ionic' ,  'DeviceManager.directives',    'DeviceManager.services','DeviceManager.servicesa','DeviceManager.controllers'],
    ($routeProvider, $locationProvider)->
        $routeProvider.when '/pin', {
            templateUrl: 'views/Pin.html'
        }
        $routeProvider.when '/device', {
            templateUrl: 'views/Device.html'
        }
    
        $routeProvider.when '/deviceChart', {
            templateUrl: 'views/DeviceChart.html'
            controller :'ChartController'
            ### The chart Controller is located in page-controller.js ###
        }
        
        $routeProvider.when '/deviceDetails', {
            templateUrl: 'views/DeviceDetails.html'
        }
    
        $routeProvider.when '/pinAdvanced', {
            templateUrl: 'views/PinAdvanced.html'  
        }

        $routeProvider.when '/pinDetails', {
            templateUrl: 'views/PinDetails.html'
        }
        $routeProvider.when '/About', {
            templateUrl: 'views/About.html'
        }
        
        $routeProvider.when '/LoadData', {
            templateUrl: 'views/LoadData.html'
        }

        $routeProvider.otherwise {
            templateUrl: 'views/DeviceList.html'
        }

      

### setting up the socket mananger and make sure everything is sent to the rootscope this allows us to use socket.io from with in angular###


            
app.controller 'DeviceController',
    class DeviceController
        @$inject : ['$scope', '$routeParams' ,'$ionicModal']
        constructor : (@$scope,  @$routeParams , $ionicModal) ->
            @prepareApplication @$scope, $ionicModal
            if  debugMode
                @setupSocketConnection @$scope
            else
                @setupTestData(@$scope)
           
        ### Here we prepare the spplicaiton itself setting states before  initialization and ###
        prepareApplication : ($scope, $ionicModal) -> 
            $scope.deviceReady= false
            $scope.currentDevice= false
            $scope.currentTabs= []
            $scope.devices=[]
            $scope.path=[]
         
            ### this should come from the server also ###
            $scope.pinModes =[ { name :'input', possibleStates:[{name: 'Pull Up', color:'#FF3300'} , {name: 'Pull Down', color:'#CC6600'} ,{name: 'Float', color:'#FF0000'}] },{ name :'output', possibleStates:[{name: 'High', color:'#0000FF'}, {name: 'High', color:'#0066CC'}] }]  
            $scope.setCurrentDevice = (id)->
                for device in $scope.devices
                    if device.id is id 
                        $scope.currentDevice = device
                $scope.pushPage id
                $scope.currentTabs=[{ name:'Device' , link: 'device', selected:true}, { name:'Device Details' , link: 'deviceDetails'} ] 
            $scope.setCurrentPin = (id)->
                for pin in $scope.currentDevice.pins
                    if pin.id is id 
                        $scope.currentPin = pin
                    for subpin in pin.subpins
                        if subpin.id is id 
                            $scope.currentPin = subpin
                 $scope.pushPage id
                 $scope.currentTabs=[{ name:'Pin ' , link: 'pin', selected:true},  { name:'Pin Details' , link: 'pinDetails'} ] 
            ### here is the gp io data ###
            $scope.gpIOsForPin = ->
                Ios= []
                for io in $scope.gpIos 
                    if io.pin is currentPin.id
                        Ios.push io.pin
                return Ios        
            $ionicModal.fromTemplateUrl('views/IoModal.html',{
                scope: $scope
                animation: 'slide-in-up'
            }).then((modal) ->
                $scope.modal = modal;
            );
            
            $scope.openModal =(index,pinID) -> 
                if pinID is -1
                    $scope.currentIO=$scope.gpIos[index]
                else
                    index2=0
                    for io in $scope.gpIos 
                        if io.pin is currentPin.id
                            index2++
                            if index2 is index
                                $scope.currentIO= io
                $scope.modal.show();
            $scope.closeModal = ->
                $scope.modal.hide();
            $scope.$on '$destroy', ->
                $scope.modal.remove();
            ### this will handle how to navigate ###
            $scope.pushPage =(id)->
                $scope.path.push(id)
                $scope.inSubSection =true
                
            $scope.popPage = ->
                lastID = $scope.path.pop()
                length =$scope.path.length
                location=''
                if length > 1 
                    $scope.setCurrentPin lastID
                    location='#/pin'
                else if length is 1
                    $scope.setCurrentDevice lastID
                    location='#/device'
                else 
                    $scope.inSubSection =false
                    $scope.currentTabs= []
                    location=''
                ### this is an extra pop this will need to be fixed later ###
                $scope.path.pop()
                location
          
            $scope.goback= ->
                location=$scope.popPage()
                window.location.hash =location
            $scope.selectTab=($index)-> 
                for tab in  $scope.currentTabs
                    tab.selected =false
                 $scope.currentTabs[$index].selected=true

        ### Here we setup the socket connection and define functions that we want to use ###
        setupSocketConnection : ($scope, socket) ->
            ### Here are the listeners which recieve data from the server  ###
            socket.on 'init', (data) -> 
                $scope.deviceReady= true
                ### We assume that some of the data being send back form the sever is data on how the device is laidout this make for easy resuablity of the main app###
                $scope.devices=data
            socket.on 'error', (data) ->
                
            socket.on 'update:pin', (data) ->
                $scope.deviceStatus=data.status

            ### Here is were we send data to the server  ###            
            $scope.changePinState = (state)-> 
                $scope.currentPin.activemode =state.name
                $scope.currentPin.currentColor =state.color
                socket.emit 'updateComponent', { 
                    pinID: currentPin.id
                    activeMode: currentPin.activemode
                    input: currentPin.input
                    output: currentPin.output
                }

        setupTestData : ($scope) -> 
                $scope.devices= window.getDeviceInfo()
                $scope.gpIos = window.getIoInfo()

                $scope.changePinState = (state)-> 
                    $scope.currentPin.activemode =state.name
                    $scope.currentPin.currentColor =state.color
                
                $scope.changeIoState = (state)-> 
                    $scope.currentIO.activemode =state.name
                    $scope.currentIO.currentColor =state.color
                    