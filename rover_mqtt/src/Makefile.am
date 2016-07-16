
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^


dist_noinst_DATA = sensors.proto controls.proto

BUILT_SOURCES = \
	sensors.pb.h \
	sensors.pb.cc \
	controls.pb.h \
	controls.pb.cc

bin_PROGRAMS = rover_mqtt imu motor

motor_SOURCES = \
	motor.cpp \
	UltraBorg.hpp \
	UltraBorg.cpp

rover_mqtt_SOURCES = \
	rover_mqtt.cpp \
	RobotSensorValues.cpp \
	RobotSensorValues.hpp \
	SensorsThreadSimulated.cpp \
	SensorsThreadSimulated.hpp \
	SensorsThread.cpp \
	SensorsThread.hpp \
	SensorsPublishThread.cpp \
	SensorsPublishThread.hpp \
	MqttInterface.cpp \
	MqttInterface.hpp \
	SelectLoopThread.cpp \
	SelectLoopThread.hpp \
	UltraBorg.cpp \
	UltraBorg.hpp

noinst_LIBRARIES = libproto.a
libproto_a_SOURCES = \
	sensors.pb.cc \
	sensors.pb.h \
	controls.pb.cc \
	controls.pb.h

libproto_a_CFLAGS =  = \
	$(protobuf_CFLAGS) \
	-Wall -Werror

rover_mqtt_CXXFLAGS = \
	$(rover_mqtt_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(protobuf_CFLAGS) \
	-Wall -Werror -Weffc++

rover_mqtt_LDADD = \
	$(rover_mqtt_LIBS) \
	$(PTHREAD_LIBS) \
	$(protobuf_LIBS) \
	-lRTIMULib \
	libproto.a

imu_SOURCES = \
	imu.cpp
imu_LDADD = -lRTIMULib

CLEANFILES = *~
MOSTLYCLEANFILES = sensors.pb.cc sensors.pb.h

