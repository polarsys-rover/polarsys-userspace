# Polarsys Rover Userspace

This repository contains the software that runs on the Polarsys Rover.

## Getting started

Here are some instructions for trying out the `rover_mqtt` program on your host
(desktop/laptop), combined with the web dashboard.

First, make sure that an MQTT broker is running locally.  You can use
mosquitto, since you'll need the mosquitto libraries anyway for `rover_mqtt`.

TODO conf mosquitto for websockets

Second, [build](rover_mqtt/README.md) the `rover_mqtt` project natively, then
launch it like this:

    $ ./rover_mqtt --simulate-ultra-borg --simulate-pico-borg-rev

You can confirm that the MQTT communication works by subscribing to the sensors
topic:

    $ mosquitto_sub -t /polarsys-rover/sensors

If you see some nonsense appearing on the terminal, it means it works.

Finally, fire up a temporary HTTP server in the `web_dashboard` directory:

    $ python3 -m http.server
    Serving HTTP on 0.0.0.0 port 8000 ...

Directing your browser to http://localhost:8000 should present you the
wonderful web interface.  Cross your fingers and click the "Connect!" button.
If everything works correctly, you should be able to see some sensor values
updating and be able to send motor commands using the sliders.
