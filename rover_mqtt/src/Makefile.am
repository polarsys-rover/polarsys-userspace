
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^


dist_noinst_DATA = sensors.proto controls.proto

BUILT_SOURCES = \
	sensors.pb.h \
	sensors.pb.cc \
	controls.pb.h \
	controls.pb.cc

bin_PROGRAMS = rover_mqtt imu motors

motors_SOURCES = \
    motors.cpp \
    PicoBorgRevReal.hpp \
    PicoBorgRevReal.cpp

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
	MotorsControlThread.cpp \
	MotorsControlThread.hpp \
	MqttInterface.cpp \
	MqttInterface.hpp \
	PicoBorgRev.hpp \
	PicoBorgRevReal.cpp \
	PicoBorgRevReal.hpp \
	PicoBorgRevSim.cpp \
	PicoBorgRevSim.hpp \
	SelectLoopThread.cpp \
	SelectLoopThread.hpp \
	UltraBorg.hpp \
	UltraBorgReal.cpp \
	UltraBorgReal.hpp

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

