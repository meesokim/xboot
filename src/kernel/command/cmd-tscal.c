/*
 * kernel/command/cmd-tscal.c
 *
 * Copyright(c) 2007-2017 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cairo-xboot.h>
#include <cairo-ft.h>
#include <shell/ctrlc.h>
#include <input/input.h>
#include <input/keyboard.h>
#include <command/command.h>

static void usage(void)
{
	printf("usage:\r\n");
	printf("    tscal <input> [framebuffer]\r\n");
}

struct tscal_t {
	int x[5], xfb[5];
	int y[5], yfb[5];
	int a[7];
} ;

static int perform_calibration(struct tscal_t * cal)
{
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;
	int j;

	n = x = y = x2 = y2 = xy = 0;
	for(j = 0; j < 5; j++)
	{
		n += 1.0;
		x += (float)cal->x[j];
		y += (float)cal->y[j];
		x2 += (float)(cal->x[j]*cal->x[j]);
		y2 += (float)(cal->y[j]*cal->y[j]);
		xy += (float)(cal->x[j]*cal->y[j]);
	}

	det = n * (x2 * y2 - xy * xy) + x * (xy * y - x * y2) + y * (x * xy - y * x2);
	if(det < 0.1 && det > -0.1)
		return 0;

	a = (x2 * y2 - xy * xy) / det;
	b = (xy * y - x * y2) / det;
	c = (x * xy - y * x2) / det;
	e = (n * y2 - y * y) / det;
	f = (x * y - n * xy) / det;
	i = (n * x2 - x * x) / det;

	z = zx = zy = 0;
	for(j = 0; j < 5; j++)
	{
		z += (float)cal->xfb[j];
		zx += (float)(cal->xfb[j] * cal->x[j]);
		zy += (float)(cal->xfb[j] * cal->y[j]);
	}

	cal->a[0] = (int)((a * z + b * zx + c * zy) * (scaling));
	cal->a[1] = (int)((b * z + e * zx + f * zy) * (scaling));
	cal->a[2] = (int)((c * z + f * zx + i * zy) * (scaling));

	z = zx = zy = 0;
	for(j = 0; j < 5; j++)
	{
		z += (float)cal->yfb[j];
		zx += (float)(cal->yfb[j] * cal->x[j]);
		zy += (float)(cal->yfb[j] * cal->y[j]);
	}

	cal->a[3] = (int)((a * z + b * zx + c * zy) * (scaling));
	cal->a[4] = (int)((b * z + e * zx + f * zy) * (scaling));
	cal->a[5] = (int)((c * z + f * zx + i * zy) * (scaling));

	cal->a[6] = (int)scaling;
	return 1;
}

static void cairo_draw_point(cairo_t * cr, int x, int y)
{
	cairo_save(cr);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_set_source_rgb(cr, 1, 0.5, 0);
	cairo_set_line_width(cr, 2);
	cairo_move_to(cr, x - 15, y);
	cairo_line_to(cr, x + 15, y);
	cairo_move_to(cr, x, y - 15);
	cairo_line_to(cr, x, y + 15);
	cairo_stroke(cr);
	cairo_restore(cr);
}

static void cairo_draw_string(cairo_t * cr, int x, int y, const char * title)
{
	cairo_save(cr);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);
	cairo_restore(cr);

	cairo_save(cr);
	cairo_set_font_size(cr, 24);
	cairo_set_source_rgb(cr, 1, 0.5, 0);
	cairo_move_to(cr, x, y);
	cairo_show_text(cr, title);
	cairo_restore(cr);
}

static int do_tscal(int argc, char ** argv)
{
	struct input_t * input;
	struct fb_t * fb;
	struct tscal_t cal;
	struct event_t e;
	cairo_surface_t * cs;
	cairo_t * cr;
	char buffer[256];
	int c[7] = {1, 0, 0, 0, 1, 0, 1};
	int width, height;
	int index;

	if(argc < 2)
	{
		usage();
		return -1;
	}

	input = search_input(argv[1]);
	if(!input || input->type != INPUT_TYPE_TOUCHSCREEN)
	{
		printf("The input device '%s' is not a touchscreen\r\n", argv[1]);
		usage();
		return -1;
	}

	fb = (argc > 2) ? search_fb(argv[2]) : search_first_fb();
	if(!fb)
	{
		printf("No framebuffer device\r\n");
		usage();
		return -1;
	}

	if(input_ioctl(input, INPUT_IOCTL_TOUCHSCEEN_SET_CALIBRATION, c) < 0)
	{
		printf("The input device '%s' can't support set calibration\r\n", argv[1]);
		return -1;
	}

	cs = cairo_xboot_surface_create(fb, fb->alone);
	cr = cairo_create(cs);
	width = cairo_image_surface_get_width(cs);
	height = cairo_image_surface_get_height(cs);
	cal.xfb[0] = 50;
	cal.yfb[0] = 50;
	cal.xfb[1] = width - 50;
	cal.yfb[1] = 50;
	cal.xfb[2] = width - 50;
	cal.yfb[2] = height - 50;
	cal.xfb[3] = 50;
	cal.yfb[3] = height - 50;
	cal.xfb[4] = width / 2;
	cal.yfb[4] = height / 2;
	index = 0;

	while(pump_event(runtime_get()->__event_base, &e));
	cairo_draw_point(cr, cal.xfb[index], cal.yfb[index]);
	cairo_xboot_surface_present(cs);

	while(1)
	{
		if(ctrlc())
			break;

		if(pump_event(runtime_get()->__event_base, &e))
		{
			if(e.type == EVENT_TYPE_KEY_UP)
			{
				if(e.e.key_up.key == KEY_ESC)
					break;
			}
			else if(e.type == EVENT_TYPE_TOUCH_END)
			{
				cal.x[index] = e.e.touch_end.x;
				cal.y[index] = e.e.touch_end.y;
				if(++index >= 5)
				{
					if(perform_calibration(&cal))
					{
						sprintf(buffer, "[%d, %d, %d, %d, %d, %d, %d]", cal.a[1], cal.a[2], cal.a[0], cal.a[4], cal.a[5], cal.a[3], cal.a[6]);
						c[0] = cal.a[1];
						c[1] = cal.a[2];
						c[2] = cal.a[0];
						c[3] = cal.a[4];
						c[4] = cal.a[5];
						c[5] = cal.a[3];
						c[6] = cal.a[6];
						input_ioctl(input, INPUT_IOCTL_TOUCHSCEEN_SET_CALIBRATION, c);
					}
					else
					{
						sprintf(buffer, "%s", "calibration failed");
					}
					cairo_draw_string(cr, 50, height / 2, buffer);
					cairo_xboot_surface_present(cs);
					printf("%s\r\n", buffer);
					break;
				}
				cairo_draw_point(cr, cal.xfb[index], cal.yfb[index]);
				cairo_xboot_surface_present(cs);
			}
		}
	}
	cairo_destroy(cr);
	cairo_surface_destroy(cs);

	return 0;
}

static struct command_t cmd_tscal = {
	.name	= "tscal",
	.desc	= "graphical touchscreen calibration",
	.usage	= usage,
	.exec	= do_tscal,
};

static __init void tscal_cmd_init(void)
{
	register_command(&cmd_tscal);
}

static __exit void tscal_cmd_exit(void)
{
	unregister_command(&cmd_tscal);
}

command_initcall(tscal_cmd_init);
command_exitcall(tscal_cmd_exit);
