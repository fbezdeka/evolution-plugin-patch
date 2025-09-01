/*
 * Evolution patch module
 *
 * Copyright (c) Florian Bezdeka, 2025
 *
 * SPDX-License-Identifier: LGPL-3.0-only
 */

#include <em-format/e-mail-extension-registry.h>
#include <em-format/e-mail-parser-extension.h>
#include <em-format/e-mail-part-utils.h>
#include <em-format/e-mail-part.h>

#define RECURSION_PREVENTOR ".patch-highlighted"

typedef EMailParserExtension EMailParserPatchAsTextXPatch;
typedef EMailParserExtensionClass EMailParserPatchAsTextXPatchClass;

typedef EExtension EMailParserPatchAsTextXPatchLoader;
typedef EExtensionClass EMailParserPatchAsTextXPatchLoaderClass;

GType e_mail_parser_patch_as_text_xpatch_get_type(void);

G_DEFINE_DYNAMIC_TYPE(EMailParserPatchAsTextXPatch,
		      e_mail_parser_patch_as_text_xpatch,
		      E_TYPE_MAIL_PARSER_EXTENSION)

static gboolean parse(EMailParserExtension *extension,
		      EMailParser *parser,
		      CamelMimePart *part,
		      GString *part_id,
		      GCancellable *cancellable,
		      GQueue *out_mail_parts)
{
	EMailPartList *part_list;
	CamelMimeMessage *msg;
	CamelContentType *ct;
	gboolean handled;
	gint len;

	/* Prevent recursion */
	if (strstr(part_id->str, RECURSION_PREVENTOR))
		return FALSE;

	ct = camel_mime_part_get_content_type(part);
	if (!camel_content_type_is(ct, "text", "plain"))
		return FALSE;

	part_list =
	    e_mail_parser_ref_part_list_for_operation(parser, cancellable);
	if (!part_list)
		return FALSE;

	msg = e_mail_part_list_get_message(part_list);
	if (!msg || !camel_mime_message_get_subject(msg) ||
	    !strstr(camel_mime_message_get_subject(msg), "[PATCH")) {
		g_object_unref(part_list);
		return FALSE;
	}

	len = part_id->len;
	g_string_append(part_id, RECURSION_PREVENTOR);

	/* changing the content type is a nasty hack, which modifies the message
	 */
	if (camel_content_type_param(ct, "charset")) {
		gchar *tmp =
		    g_strdup_printf("text/x-patch; charset=\"%s\"",
				    camel_content_type_param(ct, "charset"));
		camel_mime_part_set_content_type(part, tmp);
		g_free(tmp);
	} else {
		camel_mime_part_set_content_type(part, "text/x-patch");
	}

	handled = e_mail_parser_parse_part_as(
	    parser, part, part_id, "text/x-patch", cancellable, out_mail_parts);

	g_string_truncate(part_id, len);
	g_object_unref(part_list);

	return handled;
}

static void
e_mail_parser_patch_as_text_xpatch_class_init(EMailParserExtensionClass *klass)
{
	static const gchar *mime_types[] = {"text/plain", NULL};

	klass->mime_types = mime_types;
	klass->priority = G_PRIORITY_HIGH;
	klass->parse = parse;
}

void e_mail_parser_patch_as_text_xpatch_class_finalize(
    EMailParserExtensionClass *class)
{
}

static void
e_mail_parser_patch_as_text_xpatch_init(EMailParserExtension *extension)
{
}

void e_mail_parser_patch_as_text_xpatch_type_register(GTypeModule *type_module)
{
	e_mail_parser_patch_as_text_xpatch_register_type(type_module);
}
