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

#include "lsst/ctrl/events/Event.h"
#include "lsst/ctrl/events/Receiver.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace ctrl {
namespace events {

PYBIND11_PLUGIN(_receiver) {
    py::module::import("lsst.ctrl.events._event");

    py::module mod("_receiver", "Python wrapper for Receiver");

    py::class_<Receiver, std::shared_ptr<Receiver>> cls(mod, "Receiver");

    // do not wrap the constructor because class is pure virtual

    cls.def("receiveEvent", (std::shared_ptr<Event>(Receiver::*)()) & Receiver::receiveEvent);
    cls.def("receiveEvent", (std::shared_ptr<Event>(Receiver::*)(long)) & Receiver::receiveEvent,
            "timeout"_a);
    // do not wrap getDestinationPropertyName because it is pure virtual
    cls.def("getDestinationName", &Receiver::getDestinationName);
    cls.def("getSelector", &Receiver::getSelector);

    return mod.ptr();
}

}}} // lsst::ctrl::events
