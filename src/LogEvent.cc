// -*- lsst-c++ -*-
/** \file LogEvent.cc
  *
  * \brief Log Event implementation
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
#include "lsst/ctrl/events/LogEvent.h"
#include "lsst/ctrl/events/EventSystem.h"
#include "lsst/daf/base/PropertySet.h"
#include "lsst/pex/exceptions.h"

namespace pexExceptions = lsst::pex::exceptions;
namespace pexLogging = lsst::pex::logging;


using namespace std;

namespace lsst {
namespace ctrl {
namespace events {

const std::string LogEvent::COMMENT = "COMMENT";
const std::string LogEvent::LEVEL = "LEVEL";
const std::string LogEvent::LOG = "LOG";
const std::string LogEvent::DELIMITER = "|@|";

/** \brief Creates LogEvent which contains a PropertySet
  *
  */

LogEvent::LogEvent() : Event() {
    _init();
}


void LogEvent::_init() {
    _keywords.push_back(LogEvent::COMMENT);
    _keywords.push_back(LogEvent::LEVEL);
    _keywords.push_back(LogEvent::LOG);
}

LogEvent::LogEvent(cms::TextMessage *msg) : Event(msg) {
    _init();

    vector<std::string>results;

    std::string str = msg->getStringProperty(LogEvent::COMMENT);

    std::string::size_type cutAt;

    std::string::size_type delim_len = LogEvent::DELIMITER.length();
    while( (cutAt = str.find(LogEvent::DELIMITER)) != str.npos ) {
        if(cutAt > 0) {
            results.push_back(str.substr(0,cutAt));
        }
        str = str.substr(cutAt+delim_len);
    }
    if(str.length() > 0) {
        results.push_back(str);
    }
    _comment = results;
    _level = msg->getIntProperty(LogEvent::LEVEL);
    _log = msg->getStringProperty(LogEvent::LOG);
}

void LogEvent::setKeywords(PropertySet::Ptr psp) const {

    Event::setKeywords(psp);

    psp->set(LogEvent::COMMENT, _comment);
    psp->set(LogEvent::LEVEL, _level);
    psp->set(LogEvent::LOG, _log);
}

LogEvent::LogEvent( const std::string& runId, const pexLogging::LogRecord& rec) : Event(runId, rec.getProperties()) {
    _init();


    const PropertySet& ps = rec.getProperties();

    _type = EventTypes::LOG;


    _level = rec.getImportance();
    if (!ps.exists(LogEvent::LOG))
        _log = "default";
    else
        _log = ps.getAsString(LogEvent::LOG);

    if (!ps.exists(LogEvent::LEVEL))
        _level = -1;
    else
        _level = ps.getAsInt(LogEvent::LEVEL);

    if (!ps.exists(LogEvent::COMMENT)) {
        std::vector<std::string> commentArray = ps.getArray<std::string>(LogEvent::COMMENT);
    
        _comment = commentArray;
    } else
        _comment = std::vector<std::string>();

}

void LogEvent::populateHeader(cms::TextMessage* msg) {
    Event::populateHeader(msg);

    std::vector<std::string> vec  = _comment;
    std::ostringstream comment;
    std::vector<std::string>::iterator iter;
    for (iter = vec.begin(); iter != vec.end(); iter++) {
        comment << *iter << LogEvent::DELIMITER;
    }

    msg->setStringProperty(LogEvent::COMMENT, comment.str());
    msg->setIntProperty(LogEvent::LEVEL, _level);
    msg->setStringProperty(LogEvent::LOG, _log);
}

std::vector<std::string> LogEvent::getComment() {
    return _comment;
}

int LogEvent::getLevel() {
    return _level;
}

std::string LogEvent::getLog() {
    return _log;
}

/** \brief destructor
  */
LogEvent::~LogEvent() {
}

}
}
}
