USB_MODED_CFLAGS += -D_GNU_SOURCE
USB_MODED_LIBS   += -lpthread

sbin_PROGRAMS = usb_moded \
		usb_moded_util 

usb_moded_CPPFLAGS = \
        $(USB_MODED_CFLAGS)

usb_moded_LDFLAGS = \
	-Wl,--as-needed

usb_moded_LDADD = \
        $(USB_MODED_LIBS)

usb_moded_SOURCES = \
        usb_moded.c \
	usb_moded.h \
	usb_moded-modes.h \
	usb_moded-dbus.c \
	usb_moded-dbus.h \
	usb_moded-dbus-private.h \
	usb_moded-udev.h \
	usb_moded-config-private.h \
	usb_moded-modules.h \
	usb_moded-log.h \
	usb_moded-log.c \
	usb_moded-common.c \
	usb_moded-common.h \
	usb_moded-config.c \
	usb_moded-config.h \
	usb_moded-network.c \
	usb_moded-network.h \
	usb_moded-modesetting.c \
	usb_moded-modesetting.h \
 	usb_moded-mac.c \
	usb_moded-mac.h \
	usb_moded-dyn-config.c \
	usb_moded-dyn-config.h \
	usb_moded-udev.c \
	usb_moded-trigger.c \
	usb_moded-modules.c \
	usb_moded-configfs.c \
	usb_moded-configfs.h \
	usb_moded-worker.h \
	usb_moded-worker.c \
	usb_moded-android.h \
	usb_moded-android.c \
	usb_moded-sigpipe.h \
	usb_moded-sigpipe.c \
	usb_moded-control.h \
	usb_moded-control.c

if USE_MER_SSU
usb_moded_SOURCES += \
	usb_moded-ssu.h \
	usb_moded-ssu.c
endif

if MEEGOLOCK
usb_moded_SOURCES += \
	usb_moded-devicelock.h \
	usb_moded-devicelock.c \
	usb_moded-dsme.h \
	usb_moded-dsme.c
endif

if APP_SYNC
usb_moded_SOURCES += \
	usb_moded-appsync.c \
	usb_moded-appsync.h \
	usb_moded-systemd.c \
	usb_moded-systemd.h
endif

if APP_SYNC_DBUS
usb_moded_SOURCES += \
	usb_moded-appsync-dbus.c \
	usb_moded-appsync-dbus.h \
	usb_moded-appsync-dbus-private.h 
endif

if SAILFISH_ACCESS_CONTROL
usb_moded_CPPFLAGS += `pkg-config --cflags sailfishaccesscontrol`
usb_moded_LDFLAGS  += `pkg-config --libs sailfishaccesscontrol`
endif

usb_moded_util_CPPFLAGS = \
        $(USB_MODED_CFLAGS)

usb_moded_util_LDFLAGS = \
	-Wl,--as-needed

usb_moded_util_LDADD = \
        $(USB_MODED_LIBS)
	
usb_moded_util_SOURCES = \
	usb_moded-util.c
