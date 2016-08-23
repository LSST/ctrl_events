#!/usr/bin/env python

#
# LSST Data Management System
#
# Copyright 2008-2014  AURA/LSST.
#
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the LSST License Statement and
# the GNU General Public License along with this program.  If not,
# see <https://www.lsstcorp.org/LegalNotices/>.
#

import os
import platform
import unittest
import lsst.ctrl.events as events
from lsst.daf.base import PropertySet
import lsst.utils.tests
from eventsEnvironment import EventsEnvironment

def setup_module(module):
    lsst.utils.tests.init()

class EventSelectorTestCase(lsst.utils.tests.TestCase):
    """Test receiving events using the selector mechanism"""

    def sendEvent(self, runid, brokerName, topic):
        trans = events.EventTransmitter(brokerName, topic)

        root = PropertySet()
        root.set(events.Event.TOPIC, topic)
        root.set("myname","myname")
        root.set(events.Event.STATUS, "my special status")

        locationID = events.LocationId()

        event = events.StatusEvent(runid, locationID, root)
        # ok...now publish it
        trans.publishEvent(event)

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testEventSelector(self):
        testEnv = EventsEnvironment()
        broker = testEnv.getBroker()
        thisHost = platform.node()

        host_pid = "%s_%d" % (thisHost, os.getpid())

        topic = "test_events_11_%s" % host_pid

        runid = 'test_runid_11_%d' % os.getpid()

        rec = events.EventReceiver(broker, topic, "%s = '%s'" % (events.Event.RUNID, runid))

        #
        # send a test event, and wait to receive it
        #
        self.sendEvent(runid, broker, topic)

        # we'll get the second event, not the first
        val = rec.receiveEvent()
        self.assertIsNotNone(val)
        ps = val.getPropertySet()
        self.assertTrue(ps.exists(events.Event.RUNID))
        self.assertEqual(ps.get(events.Event.RUNID),runid)

        self.sendEvent("invalid", broker, topic)
        # shouldn't receive anything else
        val2 = rec.receiveEvent(1)
        self.assertIsNone(val2)

class EventQueueMemoryTester(lsst.utils.tests.MemoryTestCase):
    pass

if __name__ == "__main__":
    lsst.utils.tests.init()
    unittest.main()
