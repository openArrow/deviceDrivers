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
	{ 0x3ec8886f, "param_ops_int" },
	{ 0x6192dc94, "cdev_del" },
	{ 0x37a0cba, "kfree" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x3cd89706, "cdev_add" },
	{ 0x79ef1f3b, "cdev_init" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x5e09ca75, "complete" },
	{ 0x4f6b400b, "_copy_from_user" },
	{ 0x5a34a45c, "__kmalloc" },
	{ 0xa963fd01, "kmem_cache_alloc_trace" },
	{ 0xac283e02, "kmalloc_caches" },
	{ 0x4f8b5ddb, "_copy_to_user" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x83636ee3, "wait_for_completion" },
	{ 0x27e1a049, "printk" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "82C64E4EADF77003CA68C4B");
