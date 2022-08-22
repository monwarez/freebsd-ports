--- dlls/winemac.drv/vulkan.c.orig	2022-07-29 20:03:04 UTC
+++ dlls/winemac.drv/vulkan.c
@@ -591,6 +591,8 @@ static VkSurfaceKHR macdrv_wine_get_native_surface(VkS
 
 static const struct vulkan_funcs vulkan_funcs =
 {
+    NULL,
+    NULL,
     macdrv_vkCreateInstance,
     macdrv_vkCreateSwapchainKHR,
     macdrv_vkCreateWin32SurfaceKHR,
