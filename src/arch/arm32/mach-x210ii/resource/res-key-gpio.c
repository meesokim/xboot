/*
 * resource/res-key-gpio.c
 *
 * Copyright(c) 2007-2014 Jianjun Jiang <8192542@qq.com>
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

#include <xboot.h>
#include <input/key-gpio.h>
#include <s5pv210/reg-gpio.h>

static struct gpio_button_t buttons[] = {
	{
		.key = KEY_UP,
		.gpio = S5PV210_GPH2(0),
		.active_low = 1,
	}, {
		.key = KEY_DOWN,
		.gpio = S5PV210_GPH0(3),
		.active_low = 1,
	}, {
		.key = KEY_LEFT,
		.gpio = S5PV210_GPH0(2),
		.active_low = 1,
	}, {
		.key = KEY_RIGHT,
		.gpio = S5PV210_GPH2(1),
		.active_low = 1,
	}, {
		.key = KEY_MENU,
		.gpio = S5PV210_GPH2(3),
		.active_low = 1,
	}, {
		.key = KEY_ENTER,
		.gpio = S5PV210_GPH2(2),
		.active_low = 1,
	}, {
		.key = KEY_POWER,
		.gpio = S5PV210_GPH0(1),
		.active_low = 1,
	}
};

static struct key_gpio_data_t key_gpio_data = {
	.buttons	= buttons,
	.nbutton	= ARRAY_SIZE(buttons),
};

static struct resource_t res_key_gpio = {
	.name		= "key-gpio",
	.id			= -1,
	.data		= &key_gpio_data,
};

static __init void resource_key_gpio_init(void)
{
	if(register_resource(&res_key_gpio))
		LOG("Register resource %s:'%s.%d'", res_key_gpio.mach, res_key_gpio.name, res_key_gpio.id);
	else
		LOG("Failed to register resource %s:'%s.%d'", res_key_gpio.mach, res_key_gpio.name, res_key_gpio.id);
}

static __exit void resource_key_gpio_exit(void)
{
	if(unregister_resource(&res_key_gpio))
		LOG("Unregister resource %s:'%s.%d'", res_key_gpio.mach, res_key_gpio.name, res_key_gpio.id);
	else
		LOG("Failed to unregister resource %s:'%s.%d'", res_key_gpio.mach, res_key_gpio.name, res_key_gpio.id);
}

resource_initcall(resource_key_gpio_init);
resource_exitcall(resource_key_gpio_exit);
