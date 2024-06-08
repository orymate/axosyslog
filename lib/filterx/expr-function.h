/*
 * Copyright (c) 2023 shifter
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As an additional exemption you are allowed to compile & link against the
 * OpenSSL libraries as published by the OpenSSL project. See the file
 * COPYING for details.
 *
 */

#ifndef FILTERX_EXPR_FUNCTION_H_INCLUDED
#define FILTERX_EXPR_FUNCTION_H_INCLUDED

#include "filterx/filterx-expr.h"
#include "filterx/filterx-object.h"
#include "plugin.h"

typedef FilterXObject *(*FilterXSimpleFunctionProto)(GPtrArray *);

void filterx_simple_function_argument_error(FilterXExpr *s, gchar *error_info, gboolean free_info);

typedef struct _FilterXFunction
{
  FilterXExpr super;
  gchar *function_name;
} FilterXFunction;

typedef struct _FilterXFunctionArgs FilterXFunctionArgs;
typedef struct _FilterXFunctionArg
{
  gchar *name;
  FilterXExpr *value;
  gboolean retrieved;
} FilterXFunctionArg;

typedef FilterXFunction *(*FilterXFunctionCtor)(const gchar *, FilterXFunctionArgs *, GError **);

#define FILTERX_FUNCTION_ERROR filterx_function_error_quark()
GQuark filterx_function_error_quark(void);

enum FilterXFunctionError
{
  FILTERX_FUNCTION_ERROR_FUNCTION_NOT_FOUND,
  FILTERX_FUNCTION_ERROR_CTOR_FAIL,
  FILTERX_FUNCTION_ERROR_UNEXPECTED_ARGS,
};

void filterx_function_init_instance(FilterXFunction *s, const gchar *function_name);
void filterx_function_free_method(FilterXFunction *s);

FilterXFunctionArg *filterx_function_arg_new(const gchar *name, FilterXExpr *value);
FilterXFunctionArgs *filterx_function_args_new(GList *args, GError **error);
guint64 filterx_function_args_len(FilterXFunctionArgs *self);
gboolean filterx_function_args_empty(FilterXFunctionArgs *self);
FilterXExpr *filterx_function_args_get_expr(FilterXFunctionArgs *self, guint64 index);
FilterXObject *filterx_function_args_get_object(FilterXFunctionArgs *self, guint64 index);
const gchar *filterx_function_args_get_literal_string(FilterXFunctionArgs *self, guint64 index, gsize *len);
gboolean filterx_function_args_is_literal_null(FilterXFunctionArgs *self, guint64 index);
FilterXExpr *filterx_function_args_get_named_expr(FilterXFunctionArgs *self, const gchar *name);
FilterXObject *filterx_function_args_get_named_object(FilterXFunctionArgs *self, const gchar *name, gboolean *exists);
const gchar *filterx_function_args_get_named_literal_string(FilterXFunctionArgs *self, const gchar *name,
                                                            gsize *len, gboolean *exists);
gboolean filterx_function_args_get_named_literal_boolean(FilterXFunctionArgs *self, const gchar *name,
                                                         gboolean *exists, gboolean *error);
gboolean filterx_function_args_check(FilterXFunctionArgs *self, GError **error);
void filterx_function_args_free(FilterXFunctionArgs *self);

FilterXExpr *filterx_function_lookup(GlobalConfig *cfg, const gchar *function_name, GList *args, GError **error);


#define FILTERX_SIMPLE_FUNCTION_PROTOTYPE(func_name) \
  gpointer                                                              \
  filterx_ ## func_name ## _construct(Plugin *self)

#define FILTERX_SIMPLE_FUNCTION_DECLARE(func_name) \
  FILTERX_SIMPLE_FUNCTION_PROTOTYPE(func_name);

/* helper macros for template function plugins */
#define FILTERX_SIMPLE_FUNCTION(func_name, call) \
  FILTERX_SIMPLE_FUNCTION_PROTOTYPE(func_name)   \
  {                                              \
    FilterXSimpleFunctionProto f = call;         \
    return (gpointer) f;                         \
  }

#define FILTERX_SIMPLE_FUNCTION_PLUGIN(func_name)      \
  {                                                    \
    .type = LL_CONTEXT_FILTERX_SIMPLE_FUNC,            \
    .name = # func_name,                                 \
    .construct = filterx_ ## func_name ## _construct,  \
  }

#endif
