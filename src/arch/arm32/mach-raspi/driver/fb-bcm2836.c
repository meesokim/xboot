/*
 * driver/fb-bcm2836.c
 *
 * Copyright(c) 2007-2020 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>
#include <framebuffer/framebuffer.h>
#include <bcm2836-mbox.h>

struct fb_bcm2836_pdata_t {
	int width;
	int height;
	int pwidth;
	int pheight;
	int index;
	void * vram[2];
	int brightness;
};

static void fb_setbl(struct framebuffer_t * fb, int brightness)
{
	struct fb_bcm2836_pdata_t * pdat = (struct fb_bcm2836_pdata_t *)fb->priv;
	pdat->brightness = brightness;
}

static int fb_getbl(struct framebuffer_t * fb)
{
	struct fb_bcm2836_pdata_t * pdat = (struct fb_bcm2836_pdata_t *)fb->priv;
	return pdat->brightness;
}

static struct surface_t * fb_create(struct framebuffer_t * fb)
{
	struct fb_bcm2836_pdata_t * pdat = (struct fb_bcm2836_pdata_t *)fb->priv;
	return surface_alloc(pdat->width, pdat->height, NULL);
}

static void fb_destroy(struct framebuffer_t * fb, struct surface_t * s)
{
	surface_free(s);
}

static void fb_present(struct framebuffer_t * fb, struct surface_t * s, struct region_list_t * rl)
{
	struct fb_bcm2836_pdata_t * pdat = (struct fb_bcm2836_pdata_t *)fb->priv;

	pdat->index = (pdat->index + 1) & 0x1;
	memcpy(pdat->vram[pdat->index], s->pixels, s->pixlen);
	bcm2836_mbox_fb_present(0, pdat->index ? pdat->height : 0);
}

static struct device_t * fb_bcm2836_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct fb_bcm2836_pdata_t * pdat;
	struct framebuffer_t * fb;
	struct device_t * dev;

	pdat = malloc(sizeof(struct fb_bcm2836_pdata_t));
	if(!pdat)
		return NULL;

	fb = malloc(sizeof(struct framebuffer_t));
	if(!fb)
	{
		free(pdat);
		return NULL;
	}

	pdat->width = dt_read_int(n, "width", 640);
	pdat->height = dt_read_int(n, "height", 480);
	pdat->pwidth = dt_read_int(n, "physical-width", 216);
	pdat->pheight = dt_read_int(n, "physical-height", 135);
	pdat->index = 0;
	pdat->vram[0] = bcm2836_mbox_fb_alloc(pdat->width, pdat->height, 4 * 8, 2);
	pdat->vram[1] = pdat->vram[0] + (pdat->width * pdat->height * 4);
	pdat->brightness = 0;

	fb->name = alloc_device_name(dt_read_name(n), dt_read_id(n));
	fb->width = pdat->width;
	fb->height = pdat->height;
	fb->pwidth = pdat->pwidth;
	fb->pheight = pdat->pheight;
	fb->setbl = fb_setbl;
	fb->getbl = fb_getbl;
	fb->create = fb_create;
	fb->destroy = fb_destroy;
	fb->present = fb_present;
	fb->priv = pdat;

	if(!(dev = register_framebuffer(fb, drv)))
	{
		free_device_name(fb->name);
		free(fb->priv);
		free(fb);
		return NULL;
	}
	return dev;
}

static void fb_bcm2836_remove(struct device_t * dev)
{
	struct framebuffer_t * fb = (struct framebuffer_t *)dev->priv;

	if(fb)
	{
		unregister_framebuffer(fb);
		free_device_name(fb->name);
		free(fb->priv);
		free(fb);
	}
}

static void fb_bcm2836_suspend(struct device_t * dev)
{
}

static void fb_bcm2836_resume(struct device_t * dev)
{
}

static struct driver_t fb_bcm2836 = {
	.name		= "fb-bcm2836",
	.probe		= fb_bcm2836_probe,
	.remove		= fb_bcm2836_remove,
	.suspend	= fb_bcm2836_suspend,
	.resume		= fb_bcm2836_resume,
};

static __init void fb_bcm2836_driver_init(void)
{
	register_driver(&fb_bcm2836);
}

static __exit void fb_bcm2836_driver_exit(void)
{
	unregister_driver(&fb_bcm2836);
}

driver_initcall(fb_bcm2836_driver_init);
driver_exitcall(fb_bcm2836_driver_exit);
