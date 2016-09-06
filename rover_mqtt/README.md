# rover_mqtt

The `rover_mqtt` program is the main application that runs on the
Polarsys Rover.  It exposes an MQTT interface allowing remote programs
to control the rover and consume data from its sensors.

This application normally runs on the Raspberry Pi board of the rover,
but it's also possible to build it for x86 and run it on a development
machine for testing or debugging purposes.

## Prerequisites

One of the prerequisites is the
[RTIMU library](https://github.com/RPi-Distro/RTIMULib).  On the rover,
it should be available through the
[archive.raspberrypi.org](http://archive.raspberrypi.org) repository,
and already installed on the image.  If you are building on x86, you
will need to build and install the library yourself.

The other prerequisites are:

* [Boost Program Options](www.boost.org/doc/html/program_options.html)
* [libmosquittopp](https://mosquitto.org)

## Building

These are the steps required to build `rover_mqtt`.

    $ git clone https://github.com/polarsys-rover/polarsys-userspace.git
    $ cd polarsys-userspace
    $ ./bootstrap
    $ ./configure
    $ make
    
The final executable can be found at `src/rover_mqtt`. 

## Running

Without any options, `rover_mqtt` tries to access the real extern boards
(UltraBorg, PicoBorgRev, Pi Sense Hat) through the I2C bus.  If it can't
find any of them, it will exit with an error.

It is possible to simulate the various boards using the `--simulate-*`
flags.  See the help (`-h/--help`) for more information.
