#ifndef INCLUDE_COMMON_DEFS_H
#define INCLUDE_COMMON_DEFS_H
/**
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "License" in the same
 * directory as this file.
 * Copyright (c) 2013 Sören Brinkmann <soeren.brinkmann@gmail.com>
 * Copyright (c) 2007 Nokia Corporation. All rights reserved.
 *
 * This file includes the common symbolic defines for both client and
 * the server. Normally this kind of information would be part of the
 * object usage documentation, but in this example we take the easy
 * way out.
 *
 * To re-iterate: You could just as easily use strings in both client
 *                and server, and that would be the more common way.
 */

/* Well-known name for this service. */
#define MDT_SERVICE_NAME        "my.dbus.test.srv"
/* Object path to the provided object. */
#define MDT_SERVICE_OBJECT_PATH "/MDT"
/* And we're interested in using it through this interface.
   This must match the entry in the interface definition XML. */
#define MDT_SERVICE_INTERFACE   "my.dbus.test"

#endif
