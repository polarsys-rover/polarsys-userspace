/*
 * Documentation links:
 *
 *   - Protobuf: https://htmlpreview.github.io/?https://raw.githubusercontent.com/dcodeIO/ProtoBuf.js/master/docs/ProtoBuf.html
 *   - Paho/MQTT: https://www.eclipse.org/paho/files/jsdoc/index.html
 */

/* Generate a random string to use as an MQTT client id. */
function generateClientId() {
	var randomLen = 16;
    var clientId = 'client_';
    var chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';

    for (i = 0; i < randomLen; i++) {
		var idx = Math.floor(Math.random() * chars.length);
        clientId += chars.charAt(idx);
	}

    return clientId;
}

var roverDashboardApp = angular.module('roverDashboardApp', []);

roverDashboardApp.controller('MainPageController', ['$scope', 'mqtt', function ($scope, mqtt) {
    /* localStorage refers to https://developer.mozilla.org/en-US/docs/Web/API/Storage */
    var host = localStorage.getItem('mqtt-host') || window.location.hostname || '127.0.0.1';
    var port = parseInt(localStorage.getItem('mqtt-port')) || 9001;

	$scope.host = host;
	$scope.port = port;
	$scope.mqttService = mqtt;

	$scope.connect = function () {
        localStorage.setItem('mqtt-host', $scope.host);
        localStorage.setItem('mqtt-port', $scope.port);

		mqtt.connect($scope.host, $scope.port, function (errorStr) {
			console.log("Result = " + errorStr);
		});
	}

	$scope.disconnect = function () {
		mqtt.disconnect();
	}
}]);

function create_protobuf_decoder(protobufFileUrl, protobufTypename, callback) {
    console.log('Trying to load proto file ' + protobufFileUrl);

    dcodeIO.ProtoBuf.loadProtoFile(protobufFileUrl, function (err, builder) {
        if (err != null) {
            /* Failure to create the protobuf builder. */
            console.log("Failed to create protobuf builder: " + err);
            callback(err, null);
            return;
        }

        console.log('Successfully loaded proto file ' + protobufFileUrl);

        protobufBuilder = builder;

        /* Now that we have a message builder for our proto, try to instanciate a
           Message type object with the requested type name. */
        protobufMessage = protobufBuilder.build(protobufTypename);
        console.log('Successfully created a ' + protobufTypename + ' message instance.');
        callback(null, protobufMessage);
    });
}

/* Angular controller for the sensors panel. */
roverDashboardApp.controller('RoverSensorsController', ['$scope', 'mqtt', function ($scope, mqtt) {
    $scope.sensors = {
        accel: {},
        gyro: {},
        compass: {},
        sonar: null,
    };

    $scope.mqtt = mqtt;

    var onMessage = function (message) {

        var decodedMessage = $scope.messageDecoder.decode(message.payloadBytes);
        $scope.$apply(function () {
            if (decodedMessage.accel != null) {
                $scope.sensors.accel.x = decodedMessage.accel.x;
                $scope.sensors.accel.y = decodedMessage.accel.y;
                $scope.sensors.accel.z = decodedMessage.accel.z;
            } else {
                $scope.sensors.accel.x = null;
                $scope.sensors.accel.y = null;
                $scope.sensors.accel.z = null;
            }

            if (decodedMessage.gyro != null) {
                $scope.sensors.gyro.x = decodedMessage.gyro.x;
                $scope.sensors.gyro.y = decodedMessage.gyro.y;
                $scope.sensors.gyro.z = decodedMessage.gyro.z;
            } else {
                $scope.sensors.gyro.x = null;
                $scope.sensors.gyro.y = null;
                $scope.sensors.gyro.z = null;
            }

            if (decodedMessage.compass != null) {
                $scope.sensors.compass.x = decodedMessage.compass.x;
                $scope.sensors.compass.y = decodedMessage.compass.y;
                $scope.sensors.compass.z = decodedMessage.compass.z;
            } else {
                $scope.sensors.compass.x = null;
                $scope.sensors.compass.y = null;
                $scope.sensors.compass.z = null;
            }

            $scope.sensors.sonar = decodedMessage.sonar;
        });
    };

    create_protobuf_decoder(
        'sensors.proto', 'PolarsysRover.RoverSensors',
        function (errorStr, messageDecoder) {
            if (errorStr == null) {
                $scope.messageDecoder = messageDecoder;

                /* Ok, we got the schematic to decode protobuf messages, subscribe to the MQTT topic. */
                mqtt.subscribe('/polarsys-rover/sensors', onMessage);
            } else {
                console.log("RoverSensorsController error: " + errorStr);
            }
        }
    );
}]);

roverDashboardApp.controller('RoverControlsController', ['$scope', 'mqtt', function ($scope, mqtt) {
    $scope.controls = {
        left: 0,
        right: 0,
    };

    $scope.mqtt = mqtt;

    $scope.messageEncoder = null;
    var publishControls = function() {
        if ($scope.messageEncoder != null) {
            var data = {
              left: parseInt($scope.controls.left),
              right: parseInt($scope.controls.right),
            };
            var byteBuffer = $scope.messageEncoder.encode(data);

            /* This shouldn't be necessary, but it seems like the mqtt library
             * doesn't handle the buffer properly.  It sends a message with a
             * length equal to the capacity of the buffer, instead of equal to
             * its limit (the actual content).  Compacting the buffer reduces
             * the capacity to the limit, which fixes the problem.  */
            byteBuffer.compact();

            mqtt.publish('/polarsys-rover/controls', byteBuffer.buffer);
        }
    }

    create_protobuf_decoder(
        'controls.proto', 'PolarsysRover.RoverControls',
        function (errorStr, messageEncoder) {
            if (errorStr == null) {
                $scope.messageEncoder = messageEncoder;
            } else {
                console.log("RoverControlsController error: " + errorStr);
            }
        }
    );

    $scope.onChange = function () {
        publishControls();
    };

    $scope.motorStopAll = function() {
        $scope.controls.left = 0;
        $scope.controls.right = 0;
        publishControls();
    }

}]);

roverDashboardApp.controller('RoverFrontCameraController', ['$scope', 'mqtt', function ($scope, $mqtt) {
    $scope.imageUrl = '';
    $scope.mqtt = $mqtt;

    var onMessage = function (message) {
        var blob = new Blob([message.payloadBytes], {type: 'image/jpeg'});
        var url = window.URL.createObjectURL(blob);

        $scope.$apply(function() {
            $scope.imageUrl = url;
        });
    }

    $mqtt.subscribe('/polarsys-rover/front-camera', onMessage);
}]);

roverDashboardApp.filter('monoFloat', function() {
    return function(input) {
        if (input == null) {
            return null;
        }

        var s = "";

        if (input >= 0) {
            s += " ";
        }
        s += input.toFixed(2).toString();
        return s;
    }
});

roverDashboardApp.factory('mqtt', function ($rootScope, $timeout) {
	var service = {};

	service.client = null;
    service._subscriptions = [];

    service.subscribe = function (topic, callback) {

        if (service.is_connected()) {
            console.log("(1) Subscribing to " + topic);
            // FIXME: we should check if subscribe worked.
            service.client.subscribe(topic);
        }
        service._subscriptions[topic] = callback
    };

    service.publish = function (topic, data) {
        if (service.is_connected()) {
            //var msg = new Paho.MQTT.Message(data.buffer);

            //console.log(msg);
            service.client.send(topic, data);
        }
    }

    service.is_connected = function () {
        return service.client != null;
    }

	service.connect = function (mqttHost, mqttPort, callback) {
		var clientId = generateClientId();

		service.disconnect();

		/* Try to connect to the MQTT broker. */
        console.log('Trying to connect to the MQTT broker ' + mqttHost + ':' + mqttPort
                                                            + ' as client ' + clientId);
        var client = new Paho.MQTT.Client(mqttHost, mqttPort, clientId);

        /* Setup the MQTT client callbacks. */
        client.onMessageArrived = function (message) {
            service._onMessageArrived(message);
        };
        client.onMessageDelivered = function (message) {
            service._onMessageDelivered(message);
        };
        client.onConnectionLost = function (message) {
            service._onConnectionLost(message);
        };

        /* Connect for real to the broker. */
        client.connect({
            onSuccess: function (context) {
                for (topic in service._subscriptions) {
                    console.log("(2) Subscribing to " + topic);
                    client.subscribe(topic);
                }

                $rootScope.$apply(function () {
                    service.client = client;
                });

                /* Everything went fine, call the user callback without error. */
                callback(null);
            },
            onFailure: function (response) {
                /* Failure to connect to the broker. */
                callback(response.errorMessage);
            },
        });
	};

	service.disconnect = function () {
		if (service.client != null) {
			service.client.disconnect();
            service.client = null;
		}
	};

	service._onMessageArrived = function (message) {
        if (message.destinationName in service._subscriptions) {
            service._subscriptions[message.destinationName](message)
        }
	}

	service._onMessageDelivered = function (message) {
		console.log('Message delivered');
	}

	service._onConnectionLost = function (response) {
        // Use $timeout instead of $rootScope.$apply, because this function might
        // be called from Angular (e.g. when the user clicks on disconnect) and
        // from outside (e.g. when the network goes down).
        $timeout(function () {
            service.client = null;
        }, 0);

		if (response.errorCode != 0) {
			console.log('Connection lost: ' + response.errorMessage);
		} else {
			console.log('Disconnected.');
		}
	}

	return service;
});
