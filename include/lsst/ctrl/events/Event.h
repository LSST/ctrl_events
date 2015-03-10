// -*- lsst-c++ -*-

/* 
 * LSST Data Management System
 * Copyright 2008-2014  AURA/LSST.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <https://www.lsstcorp.org/LegalNotices/>.
 */

/** \file Event.h
  *
  * \ingroup events
  *
  * \brief defines the Event class
  *
  */

#ifndef LSST_CTRL_EVENTS_EVENT_H
#define LSST_CTRL_EVENTS_EVENT_H

#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/Message.h>

#include <stdlib.h>
#include <iostream>
#include <set>

#include "lsst/pex/logging/LogRecord.h"
#include "lsst/pex/logging/Component.h"
#include "lsst/utils/Utils.h"
#include "lsst/daf/base/PropertySet.h"
#include "boost/property_tree/ptree.hpp"

using lsst::daf::base::PropertySet;

namespace pexLogging = lsst::pex::logging;

using namespace std;

namespace lsst {
namespace ctrl {
namespace events { 

/**
 * @brief Representation of an LSST Event
 */

class Event
{
public:

    static const std::string TYPE;
    static const std::string EVENTTIME;
    static const std::string RUNID;
    static const std::string STATUS;
    static const std::string TOPIC;
    static const std::string PUBTIME;
    static const std::string UNINITIALIZED;

    Event();
    Event(PropertySet const& properties);
    Event(PropertySet const& properties, PropertySet const& filterable);

    Event(std::string const& runid, PropertySet::Ptr const properties);
    Event(std::string const& runid, PropertySet const& properties);
    Event(std::string const& runid, PropertySet const& properties, PropertySet const& filterable);
    Event(cms::TextMessage *msg);

    virtual ~Event();

    PropertySet::Ptr getPropertySet() const;

    std::string getPubDate();

    long long getPubTime();
    void setPubTime(long long t);

    long long getEventTime();
    void setEventTime(long long nsecs);

    void updateEventTime();

    std::string getEventDate();

    std::string getRunId();
    std::string getType();
    std::string getStatus();
    void setStatus(std::string status);

    void setTopic(std::string topic);
    std::string getTopic();

    vector<std::string> getFilterablePropertyNames();
    vector<std::string> getCustomPropertyNames();
    PropertySet::Ptr getCustomPropertySet() const;
    virtual void populateHeader(cms::TextMessage* msg) const;
    void marshall(cms::TextMessage *msg);

protected:
    PropertySet::Ptr _psp;
    PropertySet::Ptr _filterable;
    set<std::string> _keywords;
    void _init();
    void _constructor(std::string const& runid, PropertySet const& properties, PropertySet const& filterable);

    template<typename T>void add(std::string const& name, std::string const& tag, PropertySet const& properties, boost::property_tree::ptree& child);


private:
    std::string marshall(PropertySet const& properties);
    PropertySet::Ptr processTextMessage(cms::TextMessage *textMessage);
    PropertySet::Ptr unmarshall(std::string const& text);
    PropertySet::Ptr parsePropertySet(boost::property_tree::ptree child);
    bool addDataItem(std::string const& typeInfo, boost::property_tree::ptree& item, std::string const& key, PropertySet& properties);
};

}
}
}


#endif /*end LSST_CTRL_EVENTS_EVENT_H*/
