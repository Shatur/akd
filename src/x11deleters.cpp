#include "x11deleters.h"

#include <cstdio>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

void freeVarDefsComponents(XkbRF_VarDefsRec *varDefs)
{
    // layout member is NOT deleted because it replaced with string pointer
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
