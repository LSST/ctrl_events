/*
 * LSST Data Management System
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 * See the COPYRIGHT file
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
#include <memory>

#include "pybind11/pybind11.h"

#include "lsst/daf/base/PropertySet.h"
#include "lsst/ctrl/events/LogEvent.h"
#include "lsst/ctrl/events/StatusEvent.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_logEvent) {
    py::module::import("lsst.ctrl.events._statusEvent");

    py::module mod("_logEvent", "Python wrapper for LogEvent");

    py::class_<LogEvent, std::shared_ptr<LogEvent>, StatusEvent> cls(mod, "LogEvent");

    cls.attr("LEVEL") = py::cast(LogEvent::LEVEL);
    cls.attr("LOGGER") = py::cast(LogEvent::LOGGER);
    cls.attr("MESSAGE") = py::cast(LogEvent::MESSAGE);
    cls.attr("TIMESTAMP") = py::cast(LogEvent::TIMESTAMP);
    cls.attr("THREADNAME") = py::cast(LogEvent::THREADNAME);
    cls.attr("FILENAME") = py::cast(LogEvent::FILENAME);
    cls.attr("CLASSNAME") = py::cast(LogEvent::CLASSNAME);
    cls.attr("METHODNAME") = py::cast(LogEvent::METHODNAME);
    cls.attr("LINENUMBER") = py::cast(LogEvent::LINENUMBER);
    cls.attr("LOCATION") = py::cast(LogEvent::LOCATION);
    cls.attr("LOGGING_TOPIC") = py::cast(LogEvent::LOGGING_TOPIC);

    cls.def(py::init<>());
    cls.def(py::init<LocationId const &, PropertySet const &>(), "originator"_a, "properties"_a);
    // do not wrap the constructor that takes an activemqcpp cms::TextMessage
    // and activemqcpp is not wrapped

    cls.def("getLevel", &LogEvent::getLevel);
    cls.def("getLoggingTopic", &LogEvent::getLoggingTopic);
    cls.def("getLogger", &LogEvent::getLogger);
    // do not wrap `populateHeader` because it takes an activemqcpp cms::TextMessage
    // and activemqcpp is not wrapped

    return mod.ptr();
}

}  // events
}  // ctrl
}  // lsst
