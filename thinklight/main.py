#!/usr/bin/env python
# -- coding: utf-8 --
"""
Copyright (C) 2012 Mikael Lammentausta

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
"""

import os, sys
from time import sleep
from ctypes import CDLL
# optparse to support python <2.7
from optparse import OptionParser


usage = "USAGE: thinklight [options]"
parser = OptionParser(usage=usage)
parser.add_option("--on",
    action="store_true", dest="power",
    help="turn the light ON")
parser.add_option("--off",
    action="store_false", dest="power",
    help="turn the light OFF")
parser.add_option("-b", "--blink",
    dest="blink",
    help="blink light (smoothly) at an interval")


class Thinklight(object):

    min = 0
    max = 5
    blinking = False
    _dll = None

    def __init__(self):
        try:
            # Check device;
            # (when device is not found, libthinklight gives
            # segmentation fault)
            import usb
            dev = usb.core.find(idVendor=0x1130, idProduct=0x0202)
            if not dev:
                raise Exception("Thinklight device (1130:0202) not found")
        except ImportError:
            pass

        # The compiled libthinklight.so should be located in the
        # same directory, installed by distutils
        self._dll = CDLL(os.path.join(
            os.path.dirname(__file__), "libthinklight.so"))
        if self._dll is None:
            raise Exception("Could not init libthinklight")
        self._dll.thinklight_init()

    def turn_on(self):
        self._dll.thinklight_turn_on()

    def turn_off(self):
        self.blinking = False
        self._dll.thinklight_turn_off()

    def increase_brightness(self):
        self._dll.thinklight_increase_brightness()

    def decrease_brightness(self):
        self._dll.thinklight_decrease_brightness()

    def set_brightness(self, value):
        self._dll.thinklight_set_brightness(value)

    def get_brightness(self):
        return self._dll.thinklight_get_brightness()

    def blink(self, interval=None):
        z = True
        self.blinking = True
        while(self.blinking):
            current = self.get_brightness()

            if current == self.min:
                z = True
            elif current == self.max:
                z = False

            if current < self.max and z == True:
                self.increase_brightness()
            elif current > self.min:
                self.decrease_brightness()

            sleep(interval)


if __name__ == "__main__":
    (options, args) = parser.parse_args()
    if options.power is None and not options.blink:
        parser.print_help()
        sys.exit(1)

    try:
        light = Thinklight()
        if options.power is True:
            light.turn_on()
        elif options.power is False:
            light.turn_off()
        elif options.blink:
            interval = float(options.blink)
            light.blink(interval)

    except KeyboardInterrupt:
        try:
            if light.blinking:
                light.turn_off()
        except:
            pass
        sys.exit(1)

    except Exception, e:
        print e
        sys.exit(1)

sys.exit(0)
