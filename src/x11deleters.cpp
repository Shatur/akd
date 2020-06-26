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

#include "x11deleters.h"

#include <cstdio>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

void freeVarDefsWithoutLayout(XkbRF_VarDefsRec *varDefs)
{
    // Layout member is NOT deleted
    if (varDefs->model)
        XFree(varDefs->model);
    if (varDefs->variant)
        XFree(varDefs->variant);
    if (varDefs->options)
        XFree(varDefs->options);
    if (varDefs->extra_names)
        XFree(varDefs->extra_names);
    if (varDefs->extra_values)
        XFree(varDefs->extra_values);
}

void freeVarDefs(XkbRF_VarDefsRec *varDefs)
{
    freeVarDefsWithoutLayout(varDefs);
    if (varDefs->layout)
        XFree(varDefs->layout);
}
