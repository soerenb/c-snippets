/**
 * This program implements a GObject for a simple class, then
 * registers the object on the D-Bus and starts serving requests.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "License" in the same
 * directory as this file.
 * Copyright (c) 2013 Sören Brinkmann <soeren.brinkmann@gmail.com>
 * Copyright (c) 2007 Nokia Corporation. All rights reserved.
 *
 * In more complicated code, the GObject definition, implementation
 * and D-Bus registration would all live in separate files. In this
 * server, they're all present in this file.
 *
 * This program will pull the automatically generated D-Bus/GLib stub
 * code (which contains the marshaling code as well as a binding/call
 * table.
 *
 * This program also might serve as an introduction into implementing
 * custom GType/GObjects, but it is not the primary purpose here.
 * Important things like object life-time management (freeing of
 * objects), sub-classing and GObject properties are not covered at
 * all.
 *
 * Demonstrates a simple implementation of "tracing" as well (via the
 * NO_DAEMON define, as when built as non-daemonizing version, will
 * output information about what is happening and where. Adding
 * timestamps to each trace message is left out (see gettimeofday()
 * system call for a simple solution).
 */

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <stdlib.h>		/* exit, EXIT_FAILURE */
#include <unistd.h>		/* daemon */

/* Pull symbolic constants that are shared (in this example) between
   the client and the server. */
#include "common.h"

typedef struct {
	/* The parent class object state. */
	GObject parent;
} MdtObject;

typedef struct {
	/* The parent class state. */
	GObjectClass parent;
} MdtObjectClass;

/* Forward declaration of the function that will return the GType of
   the Value implementation. Not used in this program. */
GType mdt_object_get_type(void);

/* Macro for the above. It is common to define macros using the
   naming convention (seen below) for all GType implementations,
   and that's why we're going to do that here as well. */
#define MDT_TYPE_OBJECT              (mdt_object_get_type())

#define MDT_OBJECT(object) \
        (G_TYPE_CHECK_INSTANCE_CAST ((object), \
         MDT_TYPE_OBJECT, MdtObject))
#define MDT_OBJECT_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_CAST ((klass), \
         MDT_TYPE_OBJECT, MdtObjectClass))
#define MDT_IS_OBJECT(object) \
        (G_TYPE_CHECK_INSTANCE_TYPE ((object), \
         MDT_TYPE_OBJECT))
#define MDT_IS_OBJECT_CLASS(klass) \
        (G_TYPE_CHECK_CLASS_TYPE ((klass), \
         MDT_TYPE_OBJECT))
#define MDT_OBJECT_GET_CLASS(obj) \
        (G_TYPE_INSTANCE_GET_CLASS ((obj), \
         MDT_TYPE_OBJECT, MdtObjectClass))

G_DEFINE_TYPE(MdtObject, mdt_object, G_TYPE_OBJECT)

/**
 * Since the stub generator will reference the functions from a call
 * table, the functions must be declared before the stub is included.
 */
gboolean value_object_hello(MdtObject *obj, GError ** error);

/**
 * Pull in the stub for the server side.
 */
#include "server.h"

/* A small macro that will wrap g_print and expand to empty when
   server will daemonize. We use this to add debugging info on
   the server side, but if server will be daemonized, it doesn't
   make sense to even compile the code in.

   The macro is quite "hairy", but very convenient. */
#ifdef NO_DAEMON
#define dbg(fmtstr, args...) \
  (g_print(PROGNAME ":%s: " fmtstr "\n", __func__, ##args))
#else
#define dbg(dummy...)
#endif

/**
 * Per object initializer
 *
 * Only sets up internal state (both values set to zero)
 */
static void mdt_object_init(MdtObject * obj)
{
	dbg("Called");
	g_assert(obj != NULL);
}

/**
 * Per class initializer
 *
 * Sets up the thresholds (-100 .. 100), creates the signals that we
 * can emit from any object of this class and finally registers the
 * type into the GLib/D-Bus wrapper so that it may add its own magic.
 */
static void mdt_object_class_init(MdtObjectClass * klass)
{
	dbg("Called");
	g_assert(klass != NULL);

	/* All signals created. */

	dbg("Binding to GLib/D-Bus");

	/* Time to bind this GType into the GLib/D-Bus wrappers.
	   NOTE: This is not yet "publishing" the object on the D-Bus, but
	   since it is only allowed to do this once per class
	   creation, the safest place to put it is in the class
	   initializer.
	   Specifically, this function adds "method introspection
	   data" to the class so that methods can be called over
	   the D-Bus. */
	dbus_g_object_type_install_info(MDT_TYPE_OBJECT,
					&dbus_glib_value_object_object_info);

	dbg("Done");
	/* All done. Class is ready to be used for instantiating objects */
}

/**
 * Function that gets executed on "getvalue2".
 * (Again, similar to "getvalue1").
 */
gboolean value_object_hello(MdtObject *obj, GError ** error)
{
	dbg("Called");

	g_assert(obj != NULL);
	g_print("hello world!\n");

	return TRUE;
}

/**
 * Print out an error message and optionally quit (if fatal is TRUE)
 */
static void handleError(const char *msg, const char *reason, gboolean fatal)
{
	g_printerr(PROGNAME ": ERROR: %s (%s)\n", msg, reason);
	if (fatal) {
		exit(EXIT_FAILURE);
	}
}

/**
 * The main server code
 *
 * 1) Init GType/GObject
 * 2) Create a mainloop
 * 3) Connect to the Session bus
 * 4) Get a proxy object representing the bus itself
 * 5) Register the well-known name by which clients can find us.
 * 6) Create one Value object that will handle all client requests.
 * 7) Register it on the bus (will be found via "/GlobalValue" object
 *    path)
 * 8) Daemonize the process (if not built with NO_DAEMON)
 * 9) Start processing requests (run GMainLoop)
 *
 * This program will not exit (unless it encounters critical errors).
 */
int main(int argc, char **argv)
{
	/* The GObject representing a D-Bus connection. */
	DBusGConnection *bus = NULL;
	/* Proxy object representing the D-Bus service object. */
	DBusGProxy *busProxy = NULL;
	/* Will hold one instance of ValueObject that will serve all the
	   requsts. */
	MdtObject *mdtObj = NULL;
	/* GMainLoop for our program. */
	GMainLoop *mainloop = NULL;
	/* Will store the result of the RequestName RPC here. */
	guint result;
	GError *error = NULL;

	/* Create a main loop that will dispatch callbacks. */
	mainloop = g_main_loop_new(NULL, FALSE);
	if (!mainloop) {
		/* Print error and terminate. */
		handleError("Couldn't create GMainLoop", "Unknown(OOM?)", TRUE);
	}

	g_print(PROGNAME ":main Connecting to the Session D-Bus.\n");
	bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
	if (error) {
		/* Print error and terminate. */
		handleError("Couldn't connect to session bus", error->message,
			    TRUE);
	}

	g_print(PROGNAME ":main Registering the well-known name (%s)\n",
		MDT_SERVICE_NAME);

	/* In order to register a well-known name, we need to use the
	   "RequestMethod" of the /org/freedesktop/DBus interface. Each
	   bus provides an object that will implement this interface.

	   In order to do the call, we need a proxy object first.
	   DBUS_SERVICE_DBUS = "org.freedesktop.DBus"
	   DBUS_PATH_DBUS = "/org/freedesktop/DBus"
	   DBUS_INTERFACE_DBUS = "org.freedesktop.DBus" */
	busProxy = dbus_g_proxy_new_for_name(bus,
					     DBUS_SERVICE_DBUS,
					     DBUS_PATH_DBUS,
					     DBUS_INTERFACE_DBUS);
	if (!busProxy) {
		handleError("Failed to get a proxy for D-Bus",
			    "Unknown(dbus_g_proxy_new_for_name)", TRUE);
	}

	/* Attempt to register the well-known name.
	   The RPC call requires two parameters:
	   - arg0: (D-Bus STRING): name to request
	   - arg1: (D-Bus UINT32): flags for registration.
	   (please see "org.freedesktop.DBus.RequestName" in
	   http://dbus.freedesktop.org/doc/dbus-specification.html)
	   Will return one uint32 giving the result of the RPC call.
	   We're interested in 1 (we're now the primary owner of the name)
	   or 4 (we were already the owner of the name, however in this
	   application it wouldn't make much sense).

	   The function will return FALSE if it sets the GError. */
	if (!dbus_g_proxy_call(busProxy,
			       /* Method name to call. */
			       "RequestName",
			       /* Where to store the GError. */
			       &error,
			       /* Parameter type of argument 0. Note that
			          since we're dealing with GLib/D-Bus
			          wrappers, you will need to find a suitable
			          GType instead of using the "native" D-Bus
			          type codes. */
			       G_TYPE_STRING,
			       /* Data of argument 0. In our case, this is
			          the well-known name for our server
			          example ("org.maemo.Platdev_ex"). */
			       MDT_SERVICE_NAME,
			       /* Parameter type of argument 1. */
			       G_TYPE_UINT,
			       /* Data of argument 0. This is the "flags"
			          argument of the "RequestName" method which
			          can be use to specify what the bus service
			          should do when the name already exists on
			          the bus. We'll go with defaults. */
			       0,
			       /* Input arguments are terminated with a
			          special GType marker. */
			       G_TYPE_INVALID,
			       /* Parameter type of return value 0.
			          For "RequestName" it is UINT32 so we pick
			          the GType that maps into UINT32 in the
			          wrappers. */
			       G_TYPE_UINT,
			       /* Data of return value 0. These will always
			          be pointers to the locations where the
			          proxy can copy the results. */
			       &result,
			       /* Terminate list of return values. */
			       G_TYPE_INVALID)) {
		handleError("D-Bus.RequestName RPC failed", error->message,
			    TRUE);
		/* Note that the whole call failed, not "just" the name
		   registration (we deal with that below). This means that
		   something bad probably has happened and there's not much we can
		   do (hence program termination). */
	}
	/* Check the result code of the registration RPC. */
	g_print(PROGNAME ":main RequestName returned %d.\n", result);
	if (result != 1) {
		handleError("Failed to get the primary well-known name.",
			    "RequestName result != 1", TRUE);
		/* In this case we could also continue instead of terminating.
		   We could retry the RPC later. Or "lurk" on the bus waiting for
		   someone to tell us what to do. If we would be publishing
		   multiple services and/or interfaces, it even might make sense
		   to continue with the rest anyway.

		   In our simple program, we terminate. Not much left to do for
		   this poor program if the clients won't be able to find the
		   Value object using the well-known name. */
	}

	g_print(PROGNAME ":main Creating one Mdt object.\n");
	/* The NULL at the end means that we have stopped listing the
	   property names and their values that would have been used to
	   set the properties to initial values. Our simple Value
	   implementation does not support GObject properties, and also
	   doesn't inherit anything interesting from GObject directly, so
	   there are no properties to set. For more examples on properties
	   see the first GTK+ example programs from the Application
	   Development material.

	   NOTE: You need to keep at least one reference to the published
	   object at all times, unless you want it to disappear from
	   the D-Bus (implied by API reference for
	   dbus_g_connection_register_g_object(). */
	mdtObj = g_object_new(MDT_TYPE_OBJECT, NULL);
	if (!mdtObj) {
		handleError("Failed to create one Mdt instance.",
			    "Unknown(OOM?)", TRUE);
	}

	g_print(PROGNAME ":main Registering it on the D-Bus.\n");
	/* The function does not return any status, so can't check for
	   errors here. */
	dbus_g_connection_register_g_object(bus,
					    MDT_SERVICE_OBJECT_PATH,
					    G_OBJECT(mdtObj));

	g_print(PROGNAME ":main Ready to serve requests (daemonizing).\n");

#ifndef NO_DAEMON

	/* This will attempt to daemonize this process. It will switch this
	   process' working directory to / (chdir) and then reopen stdin,
	   stdout and stderr to /dev/null. Which means that all printouts
	   that would occur after this, will be lost. Obviously the
	   daemonization will also detach the process from the controlling
	   terminal as well. */
	if (daemon(0, 0) != 0) {
		g_error(PROGNAME ": Failed to daemonize.\n");
	}
#else
	g_print(PROGNAME
		": Not daemonizing (built with NO_DAEMON-build define)\n");
#endif

	/* Start service requests on the D-Bus forever. */
	g_main_loop_run(mainloop);
	/* This program will not terminate unless there is a critical
	   error which will cause abort() to be used. Hence it will never
	   reach this point. */

	/* If it does, return failure exit code just in case. */
	return EXIT_FAILURE;
}
