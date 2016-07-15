/*
 * Documentation links:
 *
 *   - Protobuf: https://htmlpreview.github.io/?https://raw.githubusercontent.com/dcodeIO/ProtoBuf.js/master/docs/ProtoBuf.html
 *   - Paho/MQTT: https://www.eclipse.org/paho/files/jsdoc/index.html
 */

/* Generate a random string to use as an MQTT client id. */
function generateClientId(basename) {
	var randomLen = 16;
    var clientId = 'client_' + basename + '_';
    var chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';

    for (i = 0; i < randomLen; i++) {
		var idx = Math.floor(Math.random() * chars.length);
        clientId += chars.charAt(idx);
	}

    return clientId;
}

/* Client used to subscribe to an MQTT topic over websockets, where the messages
   are protobuf message. */
var ProtobufOverMQTTOverWebsocketsClient = function (protobufFileUrl, protobufTypename, mqttHost, mqttPort, mqttClientId, mqttSubscribeTopic, onMessage) {
	console.log('Creating a ProtobufOverMQTTOverWebsocketsClient');

	this.protobufFileUrl = protobufFileUrl;
	this.protobufTypename = protobufTypename;
	this.mqttHost = mqttHost;
	this.mqttPort = mqttPort;
	this.mqttClientId = mqttClientId;
	this.mqttSubscribeTopic = mqttSubscribeTopic;
	this.onMessage = onMessage;
}

ProtobufOverMQTTOverWebsocketsClient.prototype.connect = function (callback) {
    var instance = this;

    console.log('Trying to load proto file ' + instance.protobufFileUrl);

    dcodeIO.ProtoBuf.loadProtoFile(instance.protobufFileUrl, function (err, builder) {
        if (err != null) {
            /* Failure to create the protobuf builder. */
            console.log("Failed to create protobuf builder: " + err);
            callback(err);
            return;
        }

        instance.protobufBuilder = builder;

        /* Now that we have a message builder for our proto, try to instanciate a
           Message type object with the requested type name. */
        instance.protobufMessage = instance.protobufBuilder.build(instance.protobufTypename);

        /* Try to connect to the MQTT broker. */
        console.log('Trying to connect to the MQTT broker ' + instance.mqttHost + ':' + instance.mqttPort
                                                            + ' as client ' + instance.mqttClientId);
        instance.mqtt_client = new Paho.MQTT.Client(instance.mqttHost, instance.mqttPort, instance.mqttClientId);

        /* Setup the MQTT client callbacks. */
        instance.mqtt_client.onMessageArrived = function (message) {
            instance.onMessageArrived(message);
        };
        instance.mqtt_client.onMessageDelivered = function (message) {
            instance.onMessageDelivered(message);
        };
        instance.mqtt_client.onConnectionLost = function (message) {
            instance.onConnectionLost(message);
        };

        /* Connect for real to the broker. */
        instance.mqtt_client.connect({
            onSuccess: function (context) {
                console.log('Connected to broker, subscribing to ' + instance.mqttSubscribeTopic);

                /* Subscribe to the requested topic. */
                instance.mqtt_client.subscribe(instance.mqttSubscribeTopic);

                /* Everything went fine, call the user callback. */
                callback(null);
            },
            onFailure: function (response) {
                /* Failure to connect to the broker. */
                callback(response.errorMessage);
            },
        });
    });
}

ProtobufOverMQTTOverWebsocketsClient.prototype.onMessageArrived = function (message) {
    /* A message arrived, try to decode it using the previously instanciented Message. */
    decodedMessage = this.protobufMessage.decode(message.payloadBytes);
    this.onMessage(decodedMessage);
}

ProtobufOverMQTTOverWebsocketsClient.prototype.onMessageDelivered = function (message) {
    console.log('Message delivered');
}

ProtobufOverMQTTOverWebsocketsClient.prototype.onConnectionLost = function (response) {
    if (response.errorCode != 0) {
        console.log('Connection lost: ' + response.errorMessage);
    } else {
        console.log('Disconnected.');
    }
}

var roverDashboardApp = angular.module('roverDashboardApp', []);

/* Angular controller for the sensors panel. */
roverDashboardApp.controller('RoverSensorsController', function ($scope) {
    $scope.sensors = null;

    var onMessage = function (message) {
        console.log("On message");
        console.log(message);
        $scope.$apply(function () {
            $scope.sensors = message;
        });
    }

    var clientId = generateClientId('sensors');
    var client = new ProtobufOverMQTTOverWebsocketsClient('sensors.proto', 'PolarsysRover.RoverSensors',
                                                          '127.0.0.1', 9001, clientId, '/polarsys-rover/sensors',
                                                          onMessage);
    client.connect(function (err) {
        if (err) {
            console.log('Error: ' + err);
        } else {
            console.log('We are connected!');
        }
    });
});


roverDashboardApp.filter('monoFloat', function() {
    return function(input) {
        var s = "";
        if (input > 0) {
            s += " ";
        }
        s += input.toFixed(2).toString();
        return s;
    }
});
