
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^


dist_noinst_DATA = sensors.proto

BUILT_SOURCES = sensors.pb.h

bin_PROGRAMS = rover_mqtt imu

rover_mqtt_SOURCES = \
	rover_mqtt.cpp \
	RobotSensorValues.cpp \
	RobotSensorValues.hpp \
	SensorsThreadSimulated.cpp \
	SensorsThreadSimulated.hpp \
	MQTTThread.cpp \
	MQTTThread.hpp \
	SelectLoopThread.cpp \
	SelectLoopThread.hpp \
	sensors.pb.cc \
	sensors.pb.h

rover_mqtt_CXXFLAGS = \
	$(libmosquittopp_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(protobuf_CFLAGS) \
	-Wall -Werror

rover_mqtt_LDADD = \
	$(libmosquittopp_LIBS) \
	$(PTHREAD_LIBS) \
	$(protobuf_LIBS)

imu_SOURCES = \
	imu.cpp
imu_LDADD = -lRTIMULib

CLEANFILES = *~
MOSTLYCLEANFILES = sensors.pb.cc sensors.pb.h

