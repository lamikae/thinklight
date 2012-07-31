/***************************************************************************
 *   Copyright (C) 2010 by Mika Haulo                                      *
 *   mika@haulo.fi                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "thinklight.h"
#include <usb.h>

/* Dream Cheeky USB Think Light */
#define VENDOR_ID   0x1130
#define PRODUCT_ID  0x0202

#define LED1 0x01 
#define LED2 0x02
#define LED3 0x04
#define LED4 0x08
#define LED5 0x10

static struct usb_dev_handle* usb_handle;

int thinklight_init()
{
	thinklight_brightness = thinklight_min;
	
	struct usb_bus *usb_bus;
	struct usb_device *dev;
	usb_init();
	usb_find_busses();
	usb_find_devices();
	
	usb_bus = usb_get_busses();
	int device_found = 0;
	
	while (usb_bus != 0 && device_found == 0)
	{
		dev = usb_bus->devices;
		
		while (dev != 0)
		{
			if (dev->descriptor.idVendor == VENDOR_ID &&
				dev->descriptor.idProduct == PRODUCT_ID)
			{
				device_found = 1;
				break;
			}
			
			dev = dev->next;
		}
		
		usb_bus = usb_bus->next;
	}
	
	int init_result;
	usb_handle = usb_open(dev);
	
	if (usb_handle == NULL)
	{
		return -1;
	}
	
	usb_detach_kernel_driver_np(usb_handle, 0);
	usb_detach_kernel_driver_np(usb_handle, 1);
	
	init_result = usb_set_configuration(usb_handle, 1);
	if (init_result != 0)
	{
		usb_close(usb_handle);
		return init_result;
	}
	
	init_result = usb_claim_interface(usb_handle, 0);
	if (init_result != 0)
	{
		usb_close(usb_handle);
		return init_result;
	}
	
	init_result = usb_set_altinterface(usb_handle, 0);
	if (init_result != 0)
	{
		usb_release_interface(usb_handle,0);
		usb_close(usb_handle);
		return init_result;
	}
	
	return 0;
}


void thinklight_uninit()
{
	usb_release_interface(usb_handle,0);
	usb_close(usb_handle);
}

void thinklight_turn_off()
{
	thinklight_set_brightness(thinklight_level0);
}

void thinklight_turn_on()
{
	thinklight_set_brightness(thinklight_max);
}

int thinklight_get_brightness()
{
	return thinklight_brightness;
}

void thinklight_set_brightness(int brightness)
{
	if (brightness < thinklight_min)
		thinklight_brightness = thinklight_min;
	else if (brightness > thinklight_max)
		thinklight_brightness = thinklight_max;
	else
		thinklight_brightness = brightness;
	
	thinklight_control();
}

void thinklight_increase_brightness()
{
	if (thinklight_brightness < thinklight_max)
		thinklight_set_brightness(++thinklight_brightness);
}

void thinklight_decrease_brightness()
{
	if (thinklight_brightness > thinklight_min)
		thinklight_set_brightness(--thinklight_brightness);
}


void thinklight_control()
{
	/* All bits up => all leds off */
	char brightness = 0x1F;
	
	if (thinklight_brightness == thinklight_level0)
		brightness = 0x1F;
	
	if (thinklight_brightness >= thinklight_level1)
		brightness ^= LED1;
	
	if (thinklight_brightness >= thinklight_level2)
		brightness ^= LED2;
	
	if (thinklight_brightness >= thinklight_level3)
		brightness ^= LED3;
	
	if (thinklight_brightness >= thinklight_level4)
		brightness ^= LED4;
	
	if (thinklight_brightness >= thinklight_level5)
		brightness ^= LED5;
	
	int data_size = 8;
	int timeout = 5000;
	int index = 1;
	int result;
	char data[data_size];
	
	/*
	 * These magic numbers are found using the official Windows driver and 
	 * a usb data sniffer.
	 */
	data[0] = 0x55;
	data[1] = 0x53;
	data[2] = 0x42;
	data[3] = 0x43;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = 0x02;
	data[7] = brightness;
	
	result = usb_control_msg(usb_handle,
							 USB_DT_HID,
							 USB_REQ_SET_CONFIGURATION,
							 USB_RECIP_ENDPOINT,
							 index,
							 data,
							 data_size,
							 timeout);
}
