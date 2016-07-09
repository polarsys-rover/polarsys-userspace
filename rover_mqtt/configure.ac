AC_PREREQ(2.61)

# package version number (not shared library version)
# odd micro numbers indicate in-progress development
# even micro numbers indicate released versions
m4_define(rover_mqtt_version_major, 0)
m4_define(rover_mqtt_version_minor, 1)
m4_define(rover_mqtt_version_micro, 0)

m4_define([rover_mqtt_version],
          [rover_mqtt_version_major.rover_mqtt_version_minor.rover_mqtt_version_micro])
m4_define([rover_mqtt_api_version],
          [rover_mqtt_version_major.rover_mqtt_version_minor])

AC_INIT(rover_mqtt, rover_mqtt_version)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIR(m4)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_INSTALL

AX_CXX_COMPILE_STDCXX_11

AC_LANG([C++])

# enable pkg-config
PKG_PROG_PKG_CONFIG

AX_PTHREAD
AX_CXX_CHECK_LIB(mosquittopp,
		 [mosqpp::lib_init(void)],
		 ,
		 AC_MSG_ERROR([mosquittopp not found.]))

PKG_CHECK_MODULES(protobuf, protobuf)

AC_CHECK_PROG([PROTOC], [protoc], [protoc])
AS_IF([test "x${PROTOC}" == "x"], [AC_MSG_ERROR([protobuf compiler "protoc" not found.])])

# Checks for library functions.
#AC_CHECK_FUNCS([memset])

AC_SUBST(rover_mqtt_CFLAGS)
AC_SUBST(rover_mqtt_LIBS)


#icondir=${datadir}/icons/hicolor/32x32/apps
#AC_SUBST(icondir)

AC_OUTPUT([
Makefile
src/Makefile
])

echo ""
echo "   rover_mqtt $VERSION"
echo "   ====================="
echo ""
echo "   To build the project, run \"make\""
echo ""
