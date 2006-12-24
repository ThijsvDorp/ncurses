/****************************************************************************
 * Copyright (c) 2006 Free Software Foundation, Inc.                        *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Thomas E. Dickey                                                *
 ****************************************************************************/

#include <curses.priv.h>

#include <ctype.h>

#include <tic.h>
#include <term_entry.h>

MODULE_ID("$Id: entries.c,v 1.2 2006/12/24 00:58:33 tom Exp $")

/****************************************************************************
 *
 * Entry queue handling
 *
 ****************************************************************************/
/*
 *  The entry list is a doubly linked list with NULLs terminating the lists:
 *
 *	  ---------   ---------   ---------
 *	  |       |   |       |   |       |   offset
 *        |-------|   |-------|   |-------|
 *	  |   ----+-->|   ----+-->|  NULL |   next
 *	  |-------|   |-------|   |-------|
 *	  |  NULL |<--+----   |<--+----   |   last
 *	  ---------   ---------   ---------
 *	      ^                       ^
 *	      |                       |
 *	      |                       |
 *	   _nc_head                _nc_tail
 */

NCURSES_EXPORT_VAR(ENTRY *) _nc_head = 0;
NCURSES_EXPORT_VAR(ENTRY *) _nc_tail = 0;

NCURSES_EXPORT(void)
_nc_free_entry(ENTRY * headp, TERMTYPE *tterm)
/* free the allocated storage consumed by the given list entry */
{
    ENTRY *ep;

    if ((ep = _nc_delink_entry(headp, tterm)) != 0) {
	free(ep);
    }
}

NCURSES_EXPORT(void)
_nc_free_entries(ENTRY * headp)
/* free the allocated storage consumed by list entries */
{
    (void) headp;		/* unused - _nc_head is altered here! */

    while (_nc_head != 0) {
	_nc_free_termtype(&(_nc_head->tterm));
    }
}

NCURSES_EXPORT(ENTRY *)
_nc_delink_entry(ENTRY * headp, TERMTYPE *tterm)
/* delink the allocated storage for the given list entry */
{
    ENTRY *ep, *last;

    for (last = 0, ep = headp; ep != 0; last = ep, ep = ep->next) {
	if (&(ep->tterm) == tterm) {
	    if (last != 0) {
		last->next = ep->next;
	    }
	    if (ep == _nc_head) {
		_nc_head = ep->next;
	    }
	    if (ep == _nc_tail) {
		_nc_tail = last;
	    }
	    break;
	}
    }
    return ep;
}
