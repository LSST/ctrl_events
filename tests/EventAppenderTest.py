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

"""
This tests the logging system in a variety of ways.
"""
from builtins import str
from builtins import object

import lsst.log as log
import os
import platform
import shutil
import socket
import sys
import tempfile
import unittest
import lsst.utils.tests
import lsst.ctrl.events as events
from eventsEnvironment import EventsEnvironment


def setup_module(module):
    lsst.utils.tests.init()

class TestLog(lsst.utils.tests.TestCase):

    class StderrCapture(object):
        """Redirect stderr to a file."""

        def __init__(self, filename):
            self.prevfd = None
            self.prev = None

        def __enter__(self):
            F = tempfile.NamedTemporaryFile()
            self.prevfd = os.dup(sys.stderr.fileno())
            os.dup2(F.fileno(), sys.stderr.fileno())
            self.prev = sys.stderr
            sys.stderr = os.fdopen(self.prevfd, "w")
            return F

        def __exit__(self, exc_type, exc_value, traceback):
            os.dup2(self.prevfd, self.prev.fileno())
            sys.stderr = self.prev

    def setUp(self):
        """Make a temporary directory and a log file in it."""
        self.tempDir = tempfile.mkdtemp()
        self.outputFilename = os.path.join(self.tempDir, "log.out")

    def tearDown(self):
        """Remove the temporary directory."""
        shutil.rmtree(self.tempDir)

    def configure(self, configuration):
        """
        Create a configuration file in the temporary directory and populate
        it with the provided string.
        """
        log.configure_prop(configuration.format(self.outputFilename))

    def assertConfigOutput(self, configProperties, value):
        with TestLog.StderrCapture(self) as fd:
            self.configure(configProperties)
            fd.seek(0)
            log4cxxOutput = ""
            for line in fd:
               log4cxxOutput += line.decode('UTF-8').rstrip()
            self.assertEquals(log4cxxOutput, value)

    def assertValidMessage(self, event, message):
        ps = event.getPropertySet()
        self.assertEqual(ps.get("message"), message)

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testBrokerOption(self):
        testEnv = EventsEnvironment()
        topic = testEnv.getLoggingTopic()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr += "log4j.appender.EA.TOPIC="+topic+"\n"

        self.configure(confStr)

        recv = events.EventReceiver(testEnv.getBroker(), topic)
        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
            log.info("This is INFO")
            log.debug("This is DEBUG")
        log.MDCRemove("x")
        self.assertValidMessage(recv.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recv.receiveEvent(), "This is INFO")
        self.assertValidMessage(recv.receiveEvent(), "This is DEBUG")

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testBrokerPortOption(self):
        testEnv = EventsEnvironment()
        topic = testEnv.getLoggingTopic()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr += "log4j.appender.EA.PORT="+str(testEnv.getPort())+"\n"
        confStr += "log4j.appender.EA.TOPIC="+topic+"\n"

        self.configure(confStr)

        recv = events.EventReceiver(testEnv.getBroker(), topic)
        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
            log.info("This is INFO")
            log.debug("This is DEBUG")
        log.MDCRemove("x")
        self.assertValidMessage(recv.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recv.receiveEvent(), "This is INFO")
        self.assertValidMessage(recv.receiveEvent(), "This is DEBUG")

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testBrokerTopicOption(self):
        testEnv = EventsEnvironment()
        weirdTopic = testEnv.getLoggingTopic()+"_"+testEnv.getLoggingTopic()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr += "log4j.appender.EA.PORT="+str(testEnv.getPort())+"\n"
        confStr += "log4j.appender.EA.TOPIC="+weirdTopic+"\n"

        self.configure(confStr)

        recv = events.EventReceiver(testEnv.getBroker(), weirdTopic)
        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
            log.info("This is INFO")
            log.debug("This is DEBUG")
        log.MDCRemove("x")
        self.assertValidMessage(recv.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recv.receiveEvent(), "This is INFO")
        self.assertValidMessage(recv.receiveEvent(), "This is DEBUG")

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testBrokerAlternateTopicOption(self):
        testEnv = EventsEnvironment()
        topic = platform.node() + "_" + str(os.getpid())
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr += "log4j.appender.EA.PORT="+str(testEnv.getPort())+"\n"
        confStr += "log4j.appender.EA.TOPIC="+topic+"\n"

        self.configure(confStr)

        recv = events.EventReceiver(testEnv.getBroker(), topic)

        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
            log.info("This is INFO")
            log.debug("This is DEBUG")
        log.MDCRemove("x")
        self.assertValidMessage(recv.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recv.receiveEvent(), "This is INFO")
        self.assertValidMessage(recv.receiveEvent(), "This is DEBUG")


###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testMissingBroker(self):
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"

        # this should raise an exception because no broker is specified
        #self.assertRaises(ex.Exception, self.configure, confStr)
        self.assertConfigOutput(confStr, "log4cxx: Appender requires broker option to be specified")

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testBadBroker(self):
        testEnv = EventsEnvironment()
        badBroker = "badbroker.lsstcorp.org"
        port = testEnv.getPort()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+badBroker+"\n"

        #this should raise an exception because the broker couldn't be reached
        #self.assertRaises(ex.Exception, self.configure, confStr)
        self.assertConfigOutput(confStr, "log4cxx: Couldn't reach broker " + badBroker + " at port " + str(port))

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testBadPort(self):
        testEnv = EventsEnvironment()
        broker = testEnv.getBroker()
        port = 1
        testEnv = EventsEnvironment()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+broker+"\n"
        confStr += "log4j.appender.EA.PORT="+str(port)+"\n"

        self.assertConfigOutput(confStr, "log4cxx: Couldn't reach broker " + broker + " at port " + str(port))

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testRunidSelector(self):
        testEnv = EventsEnvironment()
        topic = testEnv.getLoggingTopic()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr += "log4j.appender.EA.RUNID="+str(os.getpid())+"\n";
        confStr += "log4j.appender.EA.TOPIC="+topic+"\n"

        self.configure(confStr)

        # receive for all events
        recvALL = events.EventReceiver(testEnv.getBroker(), topic)

        # receive for events for this runid
        recv = events.EventReceiver(testEnv.getBroker(), topic, "RUNID = '%s'" % str(os.getpid()))

        # send log messages
        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
            log.info("This is INFO")
            log.debug("This is DEBUG")
        log.MDCRemove("x")

        # make sure we got all the events we should have
        self.assertValidMessage(recv.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recv.receiveEvent(), "This is INFO")
        self.assertValidMessage(recv.receiveEvent(), "This is DEBUG")

        # make sure we didn't get any more than we should have
        ev = recv.receiveEvent(100)
        self.assertIsNone(ev)

        # reconfigure with a new run id
        confStr2 = "log4j.rootLogger=TRACE, EA\n"
        confStr2 += "log4j.appender.EA=EventAppender\n"
        confStr2 += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr2 += "log4j.appender.EA.RUNID="+"blah_"+str(os.getpid())+"\n";
        confStr2 += "log4j.appender.EA.TOPIC="+topic+"\n"
        self.configure(confStr2)

        # set up a receiver for the new run id
        recv2 = events.EventReceiver(testEnv.getBroker(), topic, "RUNID = 'blah_%s'" % str(os.getpid()))

        # send log messages
        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
            log.info("This is INFO")
            log.debug("This is DEBUG")
        log.MDCRemove("x")

        # make sure we didn't receive any events from another run id
        ev = recv.receiveEvent(100)
        self.assertIsNone(ev)

        # make sure we got all the events we should have
        self.assertValidMessage(recv2.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recv2.receiveEvent(), "This is INFO")
        self.assertValidMessage(recv2.receiveEvent(), "This is DEBUG")

        # make sure we didn't get any more than we should have
        ev = recv2.receiveEvent(100)
        self.assertIsNone(ev)

        # make sure we got all the events, for all messages on this topic
        self.assertValidMessage(recvALL.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recvALL.receiveEvent(), "This is INFO")
        self.assertValidMessage(recvALL.receiveEvent(), "This is DEBUG")

        self.assertValidMessage(recvALL.receiveEvent(), "This is TRACE")
        self.assertValidMessage(recvALL.receiveEvent(), "This is INFO")
        self.assertValidMessage(recvALL.receiveEvent(), "This is DEBUG")

        # make sure we didn't get any more than we should have
        ev = recvALL.receiveEvent(100)
        self.assertIsNone(ev)

###############################################################################

    @unittest.skipUnless(EventsEnvironment().validTestDomain(), "not within valid domain")
    def testHostNameAndProcessId(self):
        testEnv = EventsEnvironment()
        topic = testEnv.getLoggingTopic()
        confStr = "log4j.rootLogger=TRACE, EA\n"
        confStr += "log4j.appender.EA=EventAppender\n"
        confStr += "log4j.appender.EA.BROKER="+testEnv.getBroker()+"\n"
        confStr += "log4j.appender.EA.TOPIC="+topic+"\n"

        self.configure(confStr)

        recv = events.EventReceiver(testEnv.getBroker(), topic)
        log.MDC("x", 3)
        with log.LogContext("component"):
            log.trace("This is TRACE")
        log.MDCRemove("x")
        e = recv.receiveEvent()
        self.assertValidMessage(e, "This is TRACE")
        ps = e.getPropertySet()

        # the host name should be the same since we're sending and receiving
        # from the same host
        host = ps.get(events.LogEvent.ORIG_HOSTNAME)
        self.assertEqual(host, socket.gethostname())

        # the process id should be the same since we're sending and receiving
        # from the same process
        pid = ps.get(events.LogEvent.ORIG_PROCESSID)
        self.assertEqual(os.getpid(), pid)

        t = ps.get(events.LogEvent.TYPE)
        self.assertEqual(t, events.EventTypes.LOG)


###############################################################################

class AppenderMemoryTestCase(lsst.utils.tests.MemoryTestCase):
    pass

if __name__ == "__main__":
    lsst.utils.tests.init()
    unittest.main()
