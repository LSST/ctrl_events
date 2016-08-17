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
import lsst.pex.exceptions as ex
import lsst.utils.tests

def setup_module(module):
    lsst.utils.tests.init()

class UnknownBrokerTestCase(lsst.utils.tests.TestCase):
    """Test to check unknown host case"""

    def testUnknownBroker(self):
        """Test to be sure that a bad broker name throws a runtime event"""

        broker = "unknown_broker.lsstcorp.org"
        topic = "test_events_7_%s_%d" % (platform.node(), os.getpid())
    
        self.assertRaises(ex.Exception, events.EventTransmitter, broker, topic)
    

class UnknownBrokerMemoryTestCase(lsst.utils.tests.MemoryTestCase):
    pass

if __name__ == "__main__":
    lsst.utils.tests.init()
    unittest.main()
