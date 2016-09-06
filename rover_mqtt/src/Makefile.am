
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^

tracepoints.h tracepoints.c: tracepoints.tp
	lttng-gen-tp tracepoints.tp

dist_noinst_DATA = sensors.proto controls.proto

BUILT_SOURCES = \
	sensors.pb.h \
	sensors.pb.cc \
	controls.pb.h \
	controls.pb.cc \
	tracepoints.h \
	tracepoints.c

bin_PROGRAMS = rover_mqtt imu motors

motors_SOURCES = \
    motors.cpp \
    PicoBorgRevReal.hpp \
    PicoBorgRevReal.cpp

rover_mqtt_SOURCES = \
	rover_mqtt.cpp \
	RobotSensorValues.cpp \
	RobotSensorValues.hpp \
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
	PiSenseHat.hpp \
	PiSenseHatReal.cpp \
	PiSenseHatReal.hpp \
	PiSenseHatSim.cpp \
	PiSenseHatSim.hpp \
	SelectLoopThread.cpp \
	SelectLoopThread.hpp \
	tracepoints.c \
	tracepoints.h \
	UltraBorg.hpp \
	UltraBorgReal.cpp \
	UltraBorgReal.hpp \
	UltraBorgSim.cpp \
	UltraBorgSim.hpp

noinst_LTLIBRARIES = libproto.la
libproto_la_SOURCES = \
	sensors.pb.cc \
	sensors.pb.h \
	controls.pb.cc \
	controls.pb.h

libproto_la_CFLAGS = \
	$(protobuf_CFLAGS) \
	-Wall -Werror

rover_mqtt_CPPFLAGS = \
	$(BOOST_CPPFLAGS)

rover_mqtt_CXXFLAGS = \
	$(rover_mqtt_CFLAGS) \
	$(PTHREAD_CFLAGS) \
	$(protobuf_CFLAGS) \
	-Wall -Werror -Weffc++

rover_mqtt_LDFLAGS = \
	$(BOOST_PROGRAM_OPTIONS_LDFLAGS)

rover_mqtt_LDADD = \
	$(rover_mqtt_LIBS) \
	$(PTHREAD_LIBS) \
	$(protobuf_LIBS) \
	$(BOOST_PROGRAM_OPTIONS_LIBS) \
	$(lttng_ust_LIBS) \
	-lRTIMULib \
	libproto.la

imu_SOURCES = \
	imu.cpp
imu_LDADD = -lRTIMULib

CLEANFILES = *~
MOSTLYCLEANFILES = sensors.pb.cc sensors.pb.h

