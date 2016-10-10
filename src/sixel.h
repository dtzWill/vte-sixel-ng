/*
 * Copyright (C) Hayaki Saito
 * originally written by kmiya@cluti (https://github.com/saitoha/sixel/blob/master/fromsixel.c)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#define DECSIXEL_PARAMS_MAX 16
#define DECSIXEL_PALETTE_MAX 1024
#define DECSIXEL_PARAMVALUE_MAX 65535
#define DECSIXEL_WIDTH_MAX 4096
#define DECSIXEL_HEIGHT_MAX 4096

typedef unsigned short sixel_color_no_t;
typedef struct sixel_image_buffer {
	sixel_color_no_t *data;
	int width;
	int height;
	int palette[DECSIXEL_PALETTE_MAX];
	sixel_color_no_t ncolors;
	int palette_modified;
	int use_private_register;
} sixel_image_t;

typedef enum parse_state {
	PS_ESC        = 1,  /* ESC */
	PS_DCS        = 2,  /* DCS */
	PS_DECSIXEL   = 3,  /* DECSIXEL body part ", $, -, ? ... ~ */
	PS_DECGRA     = 4,  /* DECGRA Set Raster Attributes " Pan; Pad; Ph; Pv */
	PS_DECGRI     = 5,  /* DECGRI Graphics Repeat Introducer ! Pn Ch */
	PS_DECGCI     = 6,  /* DECGCI Graphics Color Introducer # Pc; Pu; Px; Py; Pz */
} parse_state_t;

typedef struct parser_context {
	parse_state_t state;
	int pos_x;
	int pos_y;
	int max_x;
	int max_y;
	int attributed_pan;
	int attributed_pad;
	int attributed_ph;
	int attributed_pv;
	int repeat_count;
	int color_index;
	int bgindex;
	int param;
	int nparams;
	int params[DECSIXEL_PARAMS_MAX];
	sixel_image_t image;
} sixel_state_t;

int sixel_parser_init(sixel_state_t *st, int fgcolor, int bgcolor, int use_private_register);
int sixel_parser_parse(sixel_state_t *st, unsigned char *p, size_t len);
int sixel_parser_set_default_color(sixel_state_t *st);
int sixel_parser_finalize(sixel_state_t *st, unsigned char *pixels);
void sixel_parser_deinit(sixel_state_t *st);
