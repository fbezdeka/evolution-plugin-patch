/*
 * Evolution patch module
 *
 * Copyright (c) Florian Bezdeka, 2025
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#include <glib-object.h>
#include <gmodule.h>

#include "xpatch-parser.h"

G_MODULE_EXPORT void e_module_load(GTypeModule *type_module)
{
	e_mail_parser_patch_as_text_xpatch_type_register(type_module);
}

G_MODULE_EXPORT void e_module_unload(GTypeModule *type_module) {}
