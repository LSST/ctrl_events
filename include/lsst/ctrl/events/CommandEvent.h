// -*- lsst-c++ -*-
/** \file CommandEvent.h
  *
  * \ingroup events
  *
  * \brief defines the CommandEvent class
  *
  * \author Stephen Pietrowicz, NCSA
  *
  */

#ifndef LSST_CTRL_EVENTS_COMMANDEVENT_H
#define LSST_CTRL_EVENTS_COMMANDEVENT_H

#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/BytesMessage.h>

#include <stdlib.h>
#include <iostream>

#include "lsst/ctrl/events/Event.h"
#include "lsst/pex/policy.h"
#include "lsst/pex/logging/Component.h"
#include "lsst/utils/Utils.h"
#include "lsst/daf/base/PropertySet.h"

using lsst::daf::base::PropertySet;

using namespace std;

namespace lsst {
namespace ctrl {
namespace events { 

/**
 * @brief Representation of an LSST Event
 */

class CommandEvent : public Event
{
public:
    static const std::string ORIGINATORID;
    static const std::string ORIG_LOCALID;
    static const std::string ORIG_PROCESSID;
    static const std::string ORIG_IPID;

    static const std::string DESTINATIONID;
    static const std::string DEST_LOCALID;
    static const std::string DEST_PROCESSID;
    static const std::string DEST_IPID;

    CommandEvent();
    CommandEvent(const std::string& runid, unsigned long originatorId, unsigned long destinationId, const PropertySet& ps);
    CommandEvent(const std::string& runid, unsigned long originatorId, unsigned long destinationId, const PropertySet::Ptr psp);
    CommandEvent(cms::TextMessage *msg);
    virtual void populateHeader(cms::TextMessage *msg);

    ~CommandEvent();

    unsigned long getOriginatorId();
    unsigned short getOriginatorLocalId();
    unsigned short getOriginatorProcessId();
    unsigned int getOriginatorIPId();

    unsigned long getDestinationId();
    unsigned short getDestinationLocalId();
    unsigned short getDestinationProcessId();
    unsigned int getDestinationIPId();

protected:
    void _constructor(const std::string& runId, unsigned long originatorId, unsigned long destinationId, const PropertySet& ps);

private:
    virtual void setKeywords(PropertySet::Ptr psp) const;
    void _init();


    unsigned long _originatorId;      // long value of pid localid and hostid combined.
    unsigned short _orig_localId;    // created by EventSystem
    unsigned short _orig_processId;        // process id 
    unsigned int _orig_IPId;     // hex value of ip addr

    unsigned long _destinationId;     // long value of pid localid and hostid combined.
    unsigned short _dest_localId;    // created by EventSystem
    unsigned short _dest_processId;        // process id 
    unsigned int _dest_IPId;     // hex value of ip addr

};
}
}
}


#endif /*end LSST_CTRL_EVENTS_COMMANDEVENT_H*/
