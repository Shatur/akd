/*
 *  Copyright © 2019-2021 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Advanced Keyboard Daemon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Advanced Keyboard Daemon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Advanced Keyboard Daemon. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef X11DELETERS_H
#define X11DELETERS_H

#include <type_traits>

#include <X11/Xlib.h>

using XkbRF_VarDefsRec = class _XkbRF_VarDefs;
void freeVarDefsWithoutLayout(XkbRF_VarDefsRec *varDefs);
void freeVarDefs(XkbRF_VarDefsRec *varDefs);

template<auto Func>
using Deleter = std::integral_constant<std::decay_t<decltype(Func)>, Func>;

using DisplayDeleter = Deleter<XCloseDisplay>;
using XlibDeleter = Deleter<XFree>;
using VarDefsWithoutLayoutDeleter = Deleter<freeVarDefsWithoutLayout>;
using VarDefsDeleter = Deleter<freeVarDefs>;

#endif // X11DELETERS_H
