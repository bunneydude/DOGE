'use strict';

/* Controllers */

var chartDataTemplate;
var globalChart;
var lineChartData;
var tempLineChart;

angular.module('DeviceManager.chartController', []).
  controller('chartController', ['$scope', 'pageService', 'chartService','$timeout',  function ($scope, pageService, chartService) {

    if (!$scope.chartTemplate) {
      pageService.get(null, 'js/highcharts.json').then(function (success) {
        $scope.chartTemplate = success;
        $scope.createCharts();
      }, function (fail) {
        console.log('Problem getting chart template', fail)
      });
    } else {
      $scope.createCharts();
    }


    $scope.createCharts = function () {
      //console.log('in createCharts scope');    

    
    
     
     var dataDescription = {
        timeseries: {
          yAxisLabels: [''],
          labels: ['Total'],
          dataAttr: ['label', ['timestamp', 'count']],
          detailDataAttr: ['count'],
          colors: ['rgba(43,166,203,0.9)',
            'rgba(52,52,52,0.3)',
            'rgba(169,234,181,0.9)',
            'rgba(43,43,43,0.5)'],
          borderColor: '#1b97d1',
          areaColors: ['rgba(43,166,203,0.9)',
            'rgba(52,52,52,0.3)',
            'rgba(169,234,181,0.9)',
            'rgba(43,43,43,0.5)']
        },
        pie: {
          dataAttr: ['label', ['timestamp', 'count']],
          colors: [
            'rgba(43,166,203,0.4)',
            'rgba(52,52,52,0.3)',
            'rgba(169,234,181,0.5)',
            'rgba(43,43,43,0.5)',
            '#ff9191', '#ffa1a1', '#ffb6b6', '#ffcbcb'],
          borderColor: '#ff0303'
        },
        bar: {
          labels: ['barexample'],
          dataAttr: ['label', 'value'],
          colors: [createGradient('rgba(43,166,203,0.9)', 'rgba(43,166,203,0.4)')],
          borderColor: 'rgba(43,166,203,1)'
        }
      };
        
    //This variable is set in chart-directive. Controls chart redraw on view switch
    if (chart_created) {
        //console.log('chart_created reinit');
        $scope.lineChartData =   lineChartData;
    }

    $scope.$on('chartCreated', function(e, chart){    
        //console.log('chartCreated');
        globalChart=   chart;
    });

    var ipaddr = location.hostname;
    var socket = io.connect('http://'+ipaddr+':3000');         
    socket.emit('join',{'socketid':'chart'});

     socket.on('init', function(data) {
        //console.log('chart init');
        chartDataTemplate = data;
        tempLineChart = angular.copy($scope.chartTemplate.line);
        lineChartData = chartService.convertLineChart(chartDataTemplate, tempLineChart, dataDescription.timeseries, '');
        $scope.lineChartData =   lineChartData;
        $scope.$apply();
      });

      socket.on('update', function(data) {
        for (var i = 0, _len = data.length; i < _len; i++) {
            var newData = data[i];
            //console.log(newData);
            globalChart.series[i].addPoint(newData,true, false);
        }
        $scope.$apply();
        
      });

        
    }
    
    function createGradient(color1, color2) {
      var perShapeGradient = {
        x1: 0,
        y1: 0,
        x2: 0,
        y2: 1
      };
      return {
        linearGradient: perShapeGradient,
        stops: [
          [0, color1],
          [1, color2]
        ]
      }
    }


  }]);



angular.module('DeviceManager.servicesa', [], function ($provide) {

  $provide.factory('pageService', function ($q, $http) {

    return {
      get: function (id, url) {
        var items, deferred;

        deferred = $q.defer();

        $http.get((url || configuration.ITEMS_URL)).
          success(function (data, status, headers, config) {
            var result;
            if (id) {
              angular.forEach(data, function (obj, index) {
                if (obj.id === id) {
                  result = obj;
                }
              });
            } else {
              result = data;
            }
            deferred.resolve(result);
          }).
          error(function (data, status, headers, config) {
            console.error(data, status, headers, config);
            deferred.reject(data);
          });

        return deferred.promise;
      }
    }

  })
});
