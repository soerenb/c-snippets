/**
 * A simple test program to test using of the Value object over the
 * D-Bus.
 *
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "LICENSE" in the same
 * directory as this file.
 * Copyright (c) 2013 Sören Brinkmann <soeren.brinkmann@gmail.com>
 * Copyright (c) 2007 Nokia Corporation. All rights reserved.
 *
 * It will first create a GLib/D-Bus proxy object connected to the
 * server's Value object, and then start firing away value set
 * methods.
 *
 * The methods are fired from a timer based callback once per second.
 * If the method calls fail (the ones that modify the values), the
 * program will still continue to run (so that the server can be
 * restarted if necessary).
 */

#include <glib.h>
#include <dbus/dbus-glib.h>
#include <stdlib.h>		/* exit, EXIT_FAILURE */
#include <string.h>		/* strcmp */

/* Pull the common symbolic defines. */
#include "common.h"

/* Pull in the client stubs that were generated with
   dbus-binding-tool */
#include "client.h"

/**
 * Print out an error message and optionally quit (if fatal is TRUE)
 */
static void handleError(const char *msg, const char *reason, gboolean fatal)
{
	g_printerr(PROGNAME ": ERROR: %s (%s)\n", msg, reason);
	if (fatal)
		exit(EXIT_FAILURE);
}


/**
 * This function will be called repeatedly from within the mainloop
 * timer launch code.
 *
 * The function will start with two statically initialized variables
 * (int and double) which will be incremented after each time this
 * function runs and will use the setvalue* remote methods to set the
 * new values. If the set methods fail, program is not aborted, but an
 * message will be issued to the user describing the error.
 *
 * It will purposefully start value2 from below the default minimum
 * threshold (set in the server code).
 *
 * NOTE: There is a design issue in the implementation in the Value
 *       object: it does not provide "adjust" method which would make
 *       it possible for multiple clients to adjust the values,
 *       instead of setting them separately. Now, if you launch
 *       multiple clients (at different times), the object internal
 *       values will end up fluctuating between the clients.
 */
static gboolean timerCallback(DBusGProxy *remoteobj)
{
	GError *error = NULL;

	/* Set the first value. */
	my_dbus_test_hello(remoteobj, &error);
	if (error)
		handleError("Failed to call remote hello", error->message,
				FALSE);

	/* Tell the timer launcher that we want to remain on the timer
	   call list in the future as well. Returning FALSE here would
	   stop the launch of this timer callback. */
	return TRUE;
}

/**
 * The test program itself.
 *
 * 1) Setup GType/GSignal
 * 2) Create GMainLoop object
 * 3) Connect to the Session D-Bus
 * 4) Create a proxy GObject for the remote Value object
 * 5) Register signals that we're interested from the Value object
 * 6) Register signal handlers for them
 * 7) Start a timer that will launch timerCallback once per second.
 * 8) Run main-loop (forever)
 */
int main(int argc, char **argv)
{
	/* The D-Bus connection object. Provided by GLib/D-Bus wrappers. */
	DBusGConnection *bus;
	/* This will represent the Value object locally (acting as a proxy
	   for all method calls and signal delivery. */
	DBusGProxy *remoteMdt;
	/* This will refer to the GMainLoop object */
	GMainLoop *mainloop;
	GError *error = NULL;

	/* Create a new GMainLoop with default context (NULL) and initial
	   state of "not running" (FALSE). */
	mainloop = g_main_loop_new(NULL, FALSE);
	/* Failure to create the main loop is fatal (for us). */
	if (!mainloop)
		handleError("Failed to create the mainloop", "Unknown (OOM?)",
				TRUE);

	g_print(PROGNAME ":main Connecting to Session D-Bus.\n");
	bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
	if (error)
		handleError("Couldn't connect to the Session bus",
				error->message, TRUE);
		/* Normally you'd have to also g_error_free() the error object
		   but since the program will terminate within handleError,
		   it is not necessary here. */

	g_print(PROGNAME ":main Creating a GLib proxy object for Mdt.\n");

	/* Create the proxy object that we'll be using to access the object
	   on the server. If you would use dbus_g_proxy_for_name_owner(),
	   you would be also notified when the server that implements the
	   object is removed (or rather, the interface is removed). Since
	   we don't care who actually implements the interface, we'll use the
	   more common function. See the API documentation at
	   http://maemo.org/api_refs/4.0/dbus/ for more details. */
	remoteMdt = dbus_g_proxy_new_for_name(bus, MDT_SERVICE_NAME,	/* name */
						MDT_SERVICE_OBJECT_PATH,	/* obj path */
						MDT_SERVICE_INTERFACE
						/* interface */ );
	if (!remoteMdt)
		handleError("Couldn't create the proxy object",
			    "Unknown(dbus_g_proxy_new_for_name)", TRUE);

	/* Register the signatures for the signal handlers.
	   In our case, we'll have one string parameter passed to use along
	   the signal itself. The parameter list is terminated with
	   G_TYPE_INVALID (i.e., the GType for string objects. */

	g_print(PROGNAME ":main Starting main loop (first timer in 1s).\n");

	/* Register a timer callback that will do RPC sets on the values.
	   The userdata pointer is used to pass the proxy object to the
	   callback so that it can launch modifications to the object. */
	g_timeout_add(1000, (GSourceFunc)timerCallback, remoteMdt);

	/* Run the program. */
	g_main_loop_run(mainloop);
	/* Since the main loop is not stopped (by this code), we shouldn't
	   ever get here. The program might abort() for other reasons. */

	/* If it does, return failure as exit code. */
	return EXIT_FAILURE;
}
