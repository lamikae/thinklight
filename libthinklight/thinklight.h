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

#ifndef THINKLIGHT_H
#define THINKLIGHT_H

#ifdef __cplusplus
extern "C" {
#endif

enum THINKLIGHT_BRIGHTNESS
{
	thinklight_level0 	= 0,
	thinklight_level1 	= 1,
	thinklight_level2 	= 2,
	thinklight_level3 	= 3,
	thinklight_level4 	= 4,
	thinklight_level5 	= 5,
	thinklight_max		= thinklight_level5,
	thinklight_min		= thinklight_level0,
} thinklight_brightness;

/*
* Try to find the device and init usb connection to it.Must be called before 
* anything else.
* Returns 0 on success, or an error code.
*/
int thinklight_init();

/*
* Close the usb connection and release the interface. Should be called last
* before closing the application using this library. Other functions than
* thinklight_init() cannot be called after calling this function.
*/
void thinklight_uninit();

/*
 * Turn the light on (max brightness, all 5 leds) of off. 
 */
void thinklight_turn_off();
void thinklight_turn_on();

/*
 * Functions to control the brightness of the light.
 */
int thinklight_get_brightness();
void thinklight_set_brightness(int brightness);
void thinklight_increase_brightness();
void thinklight_decrease_brightness();

/*
 * This function does all the dirty work by sending a usb control message
 * to the device. This is internal function, and should not be called directly.
 */
void thinklight_control();

#ifdef __cplusplus
}
#endif

#endif
