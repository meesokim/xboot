#include <xboot.h>
#include <command/command.h>
#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include <Adafruit_SSD1306.h>

#ifdef __cplusplus
extern "C"
{
#endif

static void usage(void)
{
	printf("usage:\r\n");
	printf("    oled [-b|-w|-l|-q] address value [-c count]\r\n");
}

static int do_oled(int argc, char ** argv)
{
	if(argc < 3)
	{
        I2C i2c;
        Adafruit_SSD1306_I2c oled(i2c, 1, 0x3c);
        oled.begin(SSD1306_SWITCHCAPVCC);
        oled.clearDisplay();
        oled.drawPixel(10, 10, WHITE);
        oled.display();
		usage();
        cairo_surface_t *surface;
        cairo_t *cr;

        surface = cairo_image_surface_create (CAIRO_FORMAT_RGB16_565, 128, 32);
        cr = cairo_create (surface);
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); 
        cairo_set_line_width(cr, 0.5);
        cairo_move_to(cr, 0, 0);
        cairo_line_to(cr, 32, 32);
        cairo_select_font_face (cr, "cairo :monospace",
                    CAIRO_FONT_SLANT_NORMAL,
                    CAIRO_FONT_WEIGHT_NORMAL);

        cairo_set_font_size(cr, 13);
        cairo_move_to(cr, 0, 0);
        cairo_show_text(cr, "Most relationships seem so transitory");        
        cairo_line_to(cr, 10, 10);
        cairo_surface_write_to_png(surface, "/storage/test.png");
		return -1;
	}

    FILE *f = fopen(argv[1], "rb");
    char str[256];
    fread(str, 256, 1, f);
    fclose(f);
	printf("oled:%s\r\n", str);

	return 0;
}

static struct command_t cmd_oled = {
	.name	= "oled",
	.desc	= "oled_display out",
	.usage	= usage,
	.exec	= do_oled,
};

static __init void oled_cmd_init(void)
{
	register_command(&cmd_oled);
}

static __exit void oled_cmd_exit(void)
{
	unregister_command(&cmd_oled);
}

command_initcall(oled_cmd_init);
command_exitcall(oled_cmd_exit);

#ifdef __cplusplus
}
#endif