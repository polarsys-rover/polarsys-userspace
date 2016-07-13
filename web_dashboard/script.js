/*
 * Documentation links:
 * 
 *   - Protobuf: https://htmlpreview.github.io/?https://raw.githubusercontent.com/dcodeIO/ProtoBuf.js/master/docs/ProtoBuf.html
 *   - Paho/MQTT: https://www.eclipse.org/paho/files/jsdoc/index.html
 */

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

var ProtobufOverMQTTOverWebsocketsClient = function (protoFileUrl, mqttHost, mqttPort, mqttClientId, mqttSubscribeTopic) {
	console.log('Creating a ProtobufOverMQTTOverWebsocketsClient');
	this.protoFileUrl = protoFileUrl;
	this.mqttHost = mqttHost;
	this.mqttPort = mqttPort;
	this.mqttClientId = mqttClientId;
	this.mqttSubscribeTopic = mqttSubscribeTopic;
}

ProtobufOverMQTTOverWebsocketsClient.prototype.connect = function (callback) {
	var instance = this;
	
	console.log('Trying to load proto file ' + instance.protoFileUrl);
	dcodeIO.ProtoBuf.loadProtoFile(instance.protoFileUrl, function (err, builder) {
		if (err != null) {
			/* There was an error creating the protobuf builder. */ 
			callback(err);
			return;
		}
		
		instance.protobufBuilder = builder;

		/* Now that we have a builder for our protos, try to connect to the MQTT broker. */
		console.log('Trying to connect to the MQTT broker ' + instance.mqttHost + ':' + instance.mqttPort + ' as client ' + instance.mqttClientId);
		instance.mqtt_client = new Paho.MQTT.Client(instance.mqttHost, instance.mqttPort, instance.mqttClientId);	
		instance.mqtt_client.onMessageArrived = function (message) {
			instance.onMessageArrived(message);
		};
		instance.mqtt_client.onMessageDelivered = function (message) {
			instance.onMessageDelivered(message);
		};
		instance.mqtt_client.onConnectionLost = function (message) {
			instance.onConnectionLost(message);
		};
		instance.mqtt_client.connect({
			onSuccess: function (context) {
				console.log('Connected to broker, subscribing to ' + instance.mqttSubscribeTopic);
				instance.mqtt_client.subscribe(instance.mqttSubscribeTopic);
				callback(null);				
			},
			onFailure: function (response) {
				callback(response.errorMessage);
			},
		});	
	});	
}

ProtobufOverMQTTOverWebsocketsClient.prototype.onMessageArrived = function (message) {
	var type = this.protobufBuilder.build('PolarsysRover.RoverSensors');
	var decoded = type.decode(message.payloadBytes);

	console.log(decoded);
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

var g_client_instance = null;

$(document).ready(function () {
	var clientId = generateClientId();

	g_client_instance = new ProtobufOverMQTTOverWebsocketsClient('sensors.proto', '127.0.0.1', 9001, clientId, 'rover');
	g_client_instance.connect(function (err) {
		if (err) {
			console.log('Error: ' + err);			
		} else {
			console.log('We are connected!');
		}
	});	
});
