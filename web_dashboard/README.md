This web application is meant to be a small dashboard for the Polarsys
Rover, using its MQTT interface.  It is able to receive and display
sensor values and images from the front camera, as well as send
commands to control the rover.

Using the dashboard requires the application to be served through an
HTTP server.  An easy way to achieve this is by using Python's built-in
HTTP server:

    $ python3 -m http.server
    Serving HTTP on 0.0.0.0 port 8000 ...

Pointing your browser to http://127.0.0.1:8000 will show you the
dashboard:

![Screenshot](https://raw.githubusercontent.com/polarsys-rover/polarsys-userspace/master/web_dashboard/screenshots/screenshot1.png)

Write the IP address and port of the rover, and click the "Connect!"
button.  If the connection was successful, the complete interface
should appear:

![Screenshot](https://raw.githubusercontent.com/polarsys-rover/polarsys-userspace/master/web_dashboard/screenshots/screenshot2.png)
