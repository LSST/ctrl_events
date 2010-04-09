// -*- lsst-c++ -*-
/** \file CommandEvent.cc
  *
  * \brief Command Event implementation
  *
  * \ingroup ctrl/events
  *
  * \author Stephen R. Pietrowicz, NCSA
  *
  */
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cstring>

#include "lsst/ctrl/events/EventTypes.h"
#include "lsst/ctrl/events/Event.h"
#include "lsst/ctrl/events/CommandEvent.h"
#include "lsst/ctrl/events/EventSystem.h"
#include "lsst/daf/base/DateTime.h"
#include "lsst/daf/base/PropertySet.h"
#include "lsst/pex/exceptions.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <time.h>

#include <activemq/core/ActiveMQConnectionFactory.h>


using namespace std;

namespace lsst {
namespace ctrl {
namespace events {

const std::string CommandEvent::ORIGINATORID = "ORIGINATORID";
const std::string CommandEvent::ORIG_LOCALID = "ORIG_LOCALID";
const std::string CommandEvent::ORIG_PROCESSID = "ORIG_PROCESSID";
const std::string CommandEvent::ORIG_IPID = "ORIG_IPID";

const std::string CommandEvent::DESTINATIONID = "DESTINATIONID";
const std::string CommandEvent::DEST_LOCALID = "DEST_LOCALID";
const std::string CommandEvent::DEST_PROCESSID = "DEST_PROCESSID";
const std::string CommandEvent::DEST_IPID = "DEST_IPID";

/** \brief Creates CommandEvent which contains a PropertySet
  *
  */

CommandEvent::CommandEvent() : Event() {
    _init();
}


void CommandEvent::_init() {
    _keywords.insert(ORIGINATORID);
    _keywords.insert(ORIG_LOCALID);
    _keywords.insert(ORIG_PROCESSID);
    _keywords.insert(ORIG_IPID);

    _keywords.insert(DESTINATIONID);
    _keywords.insert(DEST_LOCALID);
    _keywords.insert(DEST_PROCESSID);
    _keywords.insert(DEST_IPID);
}

CommandEvent::CommandEvent(cms::TextMessage *msg) : Event(msg) {
    _init();


    _psp->set(ORIGINATORID, (unsigned long)msg->getLongProperty(ORIGINATORID));
    _psp->set(ORIG_LOCALID, (unsigned short)msg->getShortProperty(ORIG_LOCALID));
    _psp->set(ORIG_PROCESSID, (unsigned short)msg->getShortProperty(ORIG_PROCESSID));
    _psp->set(ORIG_IPID, (unsigned int)msg->getIntProperty(ORIG_IPID));

    _psp->set(DESTINATIONID, (unsigned long)msg->getLongProperty(DESTINATIONID));
    _psp->set(DEST_LOCALID, (unsigned short)msg->getShortProperty(DEST_LOCALID));
    _psp->set(DEST_PROCESSID, (unsigned short)msg->getShortProperty(DEST_PROCESSID));
    _psp->set(DEST_IPID, (unsigned int)msg->getIntProperty(DEST_IPID));

}

CommandEvent::CommandEvent( const std::string& runId, const unsigned long originatorId, const unsigned long destinationId, const PropertySet::Ptr psp) : Event(runId, *psp) {
    _constructor(runId, originatorId, destinationId, *psp);
}

CommandEvent::CommandEvent( const std::string& runId, const unsigned long originatorId, const unsigned long destinationId, const PropertySet& ps) : Event(runId, ps) {
    _constructor(runId, originatorId, destinationId, ps);
}

void CommandEvent::_constructor( const std::string& runId, const unsigned long originatorId, const unsigned long destinationId, const PropertySet& ps) {
    _init();

    EventSystem eventSystem = EventSystem().getDefaultEventSystem();

    //_originatorId = eventSystem.createOriginatorId();
    _psp->set(ORIGINATORID, originatorId);
    _psp->set(ORIG_LOCALID, eventSystem.extractLocalId(originatorId));
    _psp->set(ORIG_PROCESSID, eventSystem.extractProcessId(originatorId));
    _psp->set(ORIG_IPID, eventSystem.extractHostId(originatorId));

    _psp->set(DESTINATIONID, destinationId);
    _psp->set(DEST_LOCALID, eventSystem.extractLocalId(destinationId));
    _psp->set(DEST_PROCESSID, eventSystem.extractProcessId(destinationId));
    _psp->set(DEST_IPID, eventSystem.extractHostId(destinationId));

    _psp->set(TYPE, EventTypes::COMMAND);

}

void CommandEvent::populateHeader(cms::TextMessage* msg) {
    Event::populateHeader(msg);

    msg->setLongProperty(ORIGINATORID, _psp->get<unsigned long>(ORIGINATORID));
    msg->setShortProperty(ORIG_LOCALID, _psp->get<unsigned short>(ORIG_LOCALID));
    msg->setShortProperty(ORIG_PROCESSID, _psp->get<unsigned short>(ORIG_PROCESSID));
    msg->setIntProperty(ORIG_IPID, _psp->get<unsigned int>(ORIG_IPID));

    msg->setLongProperty(DESTINATIONID, _psp->get<unsigned long>(DESTINATIONID));
    msg->setShortProperty(DEST_LOCALID, _psp->get<unsigned short>(DEST_LOCALID));
    msg->setShortProperty(DEST_PROCESSID, _psp->get<unsigned short>(DEST_PROCESSID));
    msg->setIntProperty(DEST_IPID, _psp->get<unsigned int>(DEST_IPID));
}

unsigned long CommandEvent::getOriginatorId() { 
    return _psp->get<unsigned long>(ORIGINATORID);
}

unsigned short CommandEvent::getOriginatorLocalId() { 
    return _psp->get<unsigned short>(ORIG_LOCALID);
}

unsigned short CommandEvent::getOriginatorProcessId() { 
    return _psp->get<unsigned short>(ORIG_PROCESSID);
}

unsigned int CommandEvent::getOriginatorIPId() {
     return _psp->get<unsigned int>(ORIG_IPID);
}

unsigned long CommandEvent::getDestinationId() { 
    return _psp->get<unsigned long>(DESTINATIONID); 
}

unsigned short CommandEvent::getDestinationLocalId() {
     return _psp->get<unsigned short>(DEST_LOCALID);
}

unsigned short CommandEvent::getDestinationProcessId() { 
    return _psp->get<unsigned short>(DEST_PROCESSID);
}

unsigned int CommandEvent::getDestinationIPId() {
     return _psp->get<unsigned int>(DEST_IPID); 
}

/** \brief destructor
  */
CommandEvent::~CommandEvent() {
}

}
}
}
