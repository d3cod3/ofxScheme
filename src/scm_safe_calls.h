/* GDB/Scheme support for safe calls into the Guile interpreter.
   Copyright (C) 2014-2018 Free Software Foundation, Inc.
   This file is part of GDB.
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <libguile.h>


/* Struct to marshall args to scscm_safe_call_body.  */
struct c_data
{
  const char *(*func) (void *);
  void *data;
  /* An error message or NULL for success.  */
  const char *result;
};

/* Struct to marshall args through gdbscm_with_catch.  */
struct with_catch_data
{
  scm_t_catch_body func;
  void *data;
  scm_t_catch_handler unwind_handler;
  scm_t_catch_handler pre_unwind_handler;
  SCM stack;
  SCM catch_result;
};

struct eval_scheme_string_data
{
  const char *string;
  int display_result;
};

/* The "body" argument to scm_i_with_continuation_barrier.
   Invoke the user-supplied function.  */
static SCM
scscm_safe_call_body (void *d)
{
  struct c_data *data = (struct c_data *) d;
  data->result = data->func (data->data);
  return SCM_UNSPECIFIED;
}
/* A "pre-unwind handler" to scm_c_catch that prints the exception
   according to "set guile print-stack".  */
static SCM
scscm_printing_pre_unwind_handler (void *data, SCM key, SCM args)
{
  SCM stack = scm_make_stack (SCM_BOOL_T, scm_list_1 (scm_from_int (2)));
  return SCM_UNSPECIFIED;
}
/* A no-op unwind handler.  */
static SCM
scscm_nop_unwind_handler (void *data, SCM key, SCM args)
{
  return SCM_UNSPECIFIED;
}

static void *
gdbscm_with_catch (void *data)
{
  struct with_catch_data *d = (struct with_catch_data *) data;
  d->catch_result
    = scm_c_catch (SCM_BOOL_T,
		   d->func, d->data,
		   d->unwind_handler, d,
		   d->pre_unwind_handler, d);

  return NULL;
}
/* A wrapper around scm_with_guile that prints backtraces and exceptions
   according to "set guile print-stack".
   The result if NULL if no exception occurred, otherwise it is a statically
   allocated error message (caller must *not* free).  */
const char *
gdbscm_with_guile (const char *(*func) (void *), void *data)
{
  struct c_data c_data;
  struct with_catch_data catch_data;
  c_data.func = func;
  c_data.data = data;
  /* Set this now in case an exception is thrown.  */
  c_data.result = "Error while executing Scheme code.";
  catch_data.func = scscm_safe_call_body;
  catch_data.data = &c_data;
  catch_data.unwind_handler = scscm_nop_unwind_handler;
  catch_data.pre_unwind_handler = scscm_printing_pre_unwind_handler;
  catch_data.stack = SCM_BOOL_F;
  catch_data.catch_result = SCM_UNSPECIFIED;
  scm_with_guile (gdbscm_with_catch, &catch_data);
  return c_data.result;
}

/* Utilities for safely evaluating a Scheme expression string.  */

/* Wrapper to eval a C string in the Guile interpreter.
   This is passed to gdbscm_with_guile.  */
static const char *
scscm_eval_scheme_string (void *datap)
{
  struct eval_scheme_string_data *data = (struct eval_scheme_string_data *) datap;
  SCM result = scm_c_eval_string (data->string);
  if (data->display_result && !scm_is_eq (result, SCM_UNSPECIFIED))
    {
      SCM port = scm_current_output_port ();
      scm_write (result, port);
      scm_newline (port);
    }
  /* If we get here the eval succeeded.  */
  return NULL;
}
/* Safe Evaluate EXPR in the Guile interpreter. */
const char * gdbscm_safe_eval_string (const char *string, int display_result=0)
{
  struct eval_scheme_string_data data = { string, display_result };
  return gdbscm_with_guile (scscm_eval_scheme_string, (void *) &data);
}
