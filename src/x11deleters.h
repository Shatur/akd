/*
 *  Copyright © 2019-2020 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Crow Translate is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef X11DELETERS_H
#define X11DELETERS_H

#include <type_traits>
#include <X11/Xlib.h>

template<auto Func>
using Deleter = std::integral_constant<std::decay_t<decltype(Func)>, Func>;

using DisplayDeleter = Deleter<XCloseDisplay>;
using XlibDeleter = Deleter<XFree>;


#endif // X11DELETERS_H
