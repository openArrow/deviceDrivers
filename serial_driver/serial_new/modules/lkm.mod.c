#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x41572473, "module_layout" },
	{ 0xcc662366, "destroy_workqueue" },
	{ 0x13d456e1, "flush_workqueue" },
	{ 0x6192dc94, "cdev_del" },
	{ 0x37a0cba, "kfree" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0xa963fd01, "kmem_cache_alloc_trace" },
	{ 0xac283e02, "kmalloc_caches" },
	{ 0xa74e931b, "__alloc_workqueue_key" },
	{ 0x3cd89706, "cdev_add" },
	{ 0x79ef1f3b, "cdev_init" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0x1fedf0f4, "__request_region" },
	{ 0x7c61340c, "__release_region" },
	{ 0xa8a6f639, "__check_region" },
	{ 0xff7559e4, "ioport_resource" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x4f8b5ddb, "_copy_to_user" },
	{ 0xf9a482f9, "msleep" },
	{ 0x4f6b400b, "_copy_from_user" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0xb00ccc33, "finish_wait" },
	{ 0xe75663a, "prepare_to_wait" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x3970f45b, "queue_work" },
	{ 0x23287643, "current_task" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x1000e51, "schedule" },
	{ 0x27e1a049, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "46115E69D15309AC5B3F6AF");
