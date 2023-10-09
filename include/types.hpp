/* types.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>
#include <optional>

namespace mdml {

template<typename T>
using reference = std::reference_wrapper<T>;

template<typename T>
using optional_reference = std::optional<std::reference_wrapper<T>>;

template<typename T>
using const_reference = std::reference_wrapper<const T>;

template<typename T>
using opt_const_reference = std::optional<const_reference<const T>>;

}

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
