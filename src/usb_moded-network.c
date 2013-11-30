/*
  @file usb-moded_network.c : (De)activates network depending on the network setting system.

  Copyright (C) 2011 Nokia Corporation. All rights reserved.
  Copyright (C) 2012 Jolla. All rights reserved.

  @author: Philippe De Swert <philippe.de-swert@nokia.com>
  @author: Philippe De Swert <philippe.deswert@jollamobile.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the Lesser GNU General Public License 
  version 2 as published by the Free Software Foundation. 

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
 
  You should have received a copy of the Lesser GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
  02110-1301 USA
*/

/*============================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "usb_moded.h"
#include "usb_moded-network.h"
#include "usb_moded-config.h"
#include "usb_moded-log.h"
#include "usb_moded-modesetting.h"

#if CONNMAN
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#endif

const char default_interface[] = "usb0";

static char* get_interface(struct mode_list_elem *data)
{
  char *interface = NULL;

  if(data)
  {
	if(data->network_interface)
	{	
		interface = strdup(data->network_interface);
	}
  }
  else
  	interface = (char *)get_network_interface();

  if(interface == NULL)
  {
	interface = malloc(sizeof(default_interface)*sizeof(char));
	strncpy(interface, default_interface, sizeof(default_interface));
  }

  log_debug("interface = %s\n", interface);
  return interface;
}

/**
 * Turn on ip forwarding on the usb interface
 */
static void set_usb_ip_forward(struct mode_list_elem *data)
{
  const char *interface, *nat_interface;
  char command[128];

  interface = get_interface(data);
  nat_interface = get_network_nat_interface();

  write_to_file("/proc/sys/net/ipv4/ip_forward", "1");
  snprintf(command, 128, "/sbin/iptables -t nat -A POSTROUTING -o %s -j MASQUERADE", nat_interface);
  system(command);

  snprintf(command, 128, "/sbin/iptables -A FORWARD -i %s -o %s  -m state  --state RELATED,ESTABLISHED -j ACCEPT", nat_interface, interface);
  system(command);

  snprintf(command, 128, "/sbin/iptables -A FORWARD -i %s -o %s -j ACCEPT", interface, nat_interface);
  system(command);

  free((char *)interface);
  free((char *)nat_interface);
}

/**
 * Activate the network interface
 *
 */
int usb_network_up(struct mode_list_elem *data)
{
  const char *ip, *interface, *gateway;
  char command[128];

#if CONNMAN
  DBusConnection *dbus_conn_connman = NULL;
  DBusMessage *msg = NULL, *reply = NULL;
  DBusError error;

  dbus_error_init(&error);

  if( (dbus_conn_connman = dbus_bus_get(DBUS_BUS_SYSTEM, &error)) == 0 )
  {
         log_err("Could not connect to dbus for connman\n");
  }

  if ((msg = dbus_message_new_method_call("net.connman", "/", "net.connman.Service", connect)) != NULL)
  {
        if ((reply = dbus_connection_send_with_reply_and_block(dbus_conn_connman, msg, -1, NULL)) != NULL)
        {
            dbus_message_get_args(reply, NULL, DBUS_TYPE_INT32, &ret, DBUS_TYPE_INVALID);
            dbus_message_unref(reply);
        }
        dbus_message_unref(msg);
  }
  dbus_connection_unref(dbus_conn_connman);

  log_debug("connman state = %d\n", ret);
  return(ret);

#else

  interface = get_interface(data); 
  ip = get_network_ip();
  gateway = get_network_gateway();

  if(ip == NULL)
  {
	sprintf(command,"ifconfig %s 192.168.2.15", interface);
	system(command);
	goto clean;
  }
  else if(!strcmp(ip, "dhcp"))
  {
	sprintf(command, "dhclient -d %s\n", interface);
	system(command);
  }
  else
  {
	sprintf(command, "ifconfig %s %s\n", interface, ip);
	system(command);
  }

  /* TODO: Check first if there is a gateway set */
  if(gateway)
  {
	sprintf(command, "route add default gw %s\n", gateway);
        system(command);
  }

  if(data->nat)
	set_usb_ip_forward(data);

clean:
  free((char *)interface);
  free((char *)gateway);
  free((char *)ip);

  return(0);
#endif /* CONNMAN */
}

/**
 * Deactivate the network interface
 *
 */
int usb_network_down(struct mode_list_elem *data)
{
#if CONNMAN
#else
  const char *interface;
  char command[128];

  interface = get_interface(data);

  sprintf(command, "ifconfig %s down\n", interface);
  system(command);

  free((char *)interface);
  
  return(0);
#endif /* CONNMAN */
}

/**
 * Update the network interface with the new setting if connected.
 * 
*/
int usb_network_update(void)
{
  struct mode_list_elem * data;

  if(!get_usb_connection_state())
	return(0);

  data = get_usb_mode_data();
  if(data->network)
  {
	usb_network_down(data);	
	usb_network_up(data);	
	return(0);
  }
  else
	return(0);

}
