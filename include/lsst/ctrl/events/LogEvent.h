// -*- lsst-c++ -*-
/** \file LogEvent.h
  *
  * \ingroup events
  *
  * \brief defines the LogEvent class
  *
  * \author Stephen Pietrowicz, NCSA
  *
  */

#ifndef LSST_CTRL_EVENTS_LOGEVENT_H
#define LSST_CTRL_EVENTS_LOGEVENT_H

#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>

#include <stdlib.h>
#include <iostream>

#include "lsst/ctrl/events/Event.h"
#include "lsst/pex/logging/LogRecord.h"
#include "lsst/pex/policy.h"
#include "lsst/pex/logging/Component.h"
#include "lsst/utils/Utils.h"
#include "lsst/daf/base/PropertySet.h"

using lsst::daf::base::PropertySet;

namespace pexPolicy = lsst::pex::policy;
namespace pexLogging = lsst::pex::logging;

using namespace std;

namespace lsst {
namespace ctrl {
namespace events { 

/**
 * @brief Representation of an LSST Event
 */

class LogEvent : public Event
{
public:
    static const std::string COMMENT;
    static const std::string LEVEL;
    static const std::string LOG;

    LogEvent();
    LogEvent(const std::string& runid, const pexLogging::LogRecord& rec);
    LogEvent(cms::TextMessage *msg);

    ~LogEvent();

    void populateHeader(cms::TextMessage *msg);

    int getLevel();

    std::string getLog();
    vector<std::string> getComment();

private:
    static const std::string DELIMITER;

    virtual void setKeywords(PropertySet::Ptr psp) const;
    void _init();

    vector<std::string> _comment;
    int _level;
    std::string _log;

};
}
}
}


#endif /*end LSST_CTRL_EVENTS_LOGEVENT_H*/
