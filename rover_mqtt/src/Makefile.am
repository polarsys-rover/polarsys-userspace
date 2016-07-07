
bin_PROGRAMS = rover_mqtt imu
rover_mqtt_SOURCES = \
	rover_mqtt.cpp \
	RobotSensorValues.cpp \
	SensorsThreadSimulated.cpp \
	MQTTThread.cpp

imu_SOURCES = \
	imu.cpp
imu_LDADD = -lRTIMULib

rover_mqtt_CXXFLAGS = $(rover_mqtt_CFLAGS) $(PTHREAD_CFLAGS)
rover_mqtt_LIBS = $(rover_mqtt_LIBS) $(PTHREAD_LIBS)

CLEANFILES = *~
