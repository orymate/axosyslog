/*
 * Copyright (c) 2017 Balabit
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#include "filterx-parse-xml.h"
#include "filterx/expr-function.h"

#include "cfg-parser.h"
#include "plugin.h"
#include "plugin-types.h"

extern CfgParser xml_parser;

static Plugin xml_plugins[] =
{
  {
    .type = LL_CONTEXT_PARSER,
    .name = "xml",
    .parser = &xml_parser,
  },
  {
    .type = LL_CONTEXT_PARSER,
    .name = "windows-eventlog-xml-parser",
    .parser = &xml_parser,
  },
  FILTERX_GENERATOR_FUNCTION_PLUGIN(parse_xml),
};

gboolean
xml_module_init(PluginContext *context, CfgArgs *args)
{
  plugin_register(context, xml_plugins, G_N_ELEMENTS(xml_plugins));
  return TRUE;
}

const ModuleInfo module_info =
{
  .canonical_name = "xml",
  .version = SYSLOG_NG_VERSION,
  .description = "Parses xml into messages",
  .core_revision = SYSLOG_NG_SOURCE_REVISION,
  .plugins = xml_plugins,
  .plugins_len = G_N_ELEMENTS(xml_plugins),
};
