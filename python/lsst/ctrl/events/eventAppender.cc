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
#include <pybind11/pybind11.h>

// TODO decide if we need this wrapper; to use it requires wrapping
// log4cxx::helpers::Pool and spi::LoggingEventPtr, whatever that is
#include "lsst/ctrl/events/EventAppender.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_eventAppender) {
    py::module mod("_eventAppender", "Python wrapper for _eventAppender library");

    py::class_<EventAppender, std::shared_ptr<EventAppender>> cls(mod, "EventAppender");

    cls.def(py::init<>());

    // TODO wrap 
    cls.def("setOption", &EventAppender::setOption, "option"_a, "value"_a);
    cls.def("activateOptions", &EventAppender::activateOptions, "pool"_a);
    cls.def("append", &EventAppender::append, "event"_a, "pool"_a);
    cls.def("close", &EventAppender::close);
    cls.def("requiresLayout", &EventAppender::requiresLayout);

    return mod.ptr();
}

}}} // lsst::ctrl::events
