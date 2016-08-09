# 
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
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
# see <http://www.lsstcorp.org/LegalNotices/>.
#
from __future__ import print_function

"""demonstrate simple use of the Log facility."""
"""
testLog

Run with:
   python testLog
"""

import os
import tempfile
import lsst.log as log
import lsst.ctrl.events as events

if __name__ == "__main__":

    broker = "example.host.com"
    topic = "loggingtest"

    recv = events.EventReceiver(broker, topic)

    confStr = "log4j.rootLogger=TRACE, EA\n"
    confStr += "log4j.appender.EA=EventAppender\n"
    confStr += "log4j.appender.EA.BROKER="+broker+"\n"
    confStr += "log4j.appender.EA.TOPIC="+topic+"\n"

    tempDir = tempfile.mkdtemp()
    outputFileName = os.path.join(tempDir, "log.out")

    log.configure_prop(confStr.format(outputFileName))

    # test a simple message
    #with log.LogContext("component"):
    log.trace("this is a trace message")

    ev = recv.receiveEvent()
    ps = ev.getPropertySet()
    print(ps.get("message"))
