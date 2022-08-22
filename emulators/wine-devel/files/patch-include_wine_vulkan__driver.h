--- include/wine/vulkan_driver.h.orig	2022-07-29 20:03:04 UTC
+++ include/wine/vulkan_driver.h
@@ -21,6 +21,8 @@ struct vulkan_funcs
      * needs to provide. Other function calls will be provided indirectly by dispatch
      * tables part of dispatchable Vulkan objects such as VkInstance or vkDevice.
      */
+    VkResult (*p_vkAcquireNextImage2KHR)(VkDevice, const VkAcquireNextImageInfoKHR *, uint32_t *);
+    VkResult (*p_vkAcquireNextImageKHR)(VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t *);
     VkResult (*p_vkCreateInstance)(const VkInstanceCreateInfo *, const VkAllocationCallbacks *, VkInstance *);
     VkResult (*p_vkCreateSwapchainKHR)(VkDevice, const VkSwapchainCreateInfoKHR *, const VkAllocationCallbacks *, VkSwapchainKHR *);
     VkResult (*p_vkCreateWin32SurfaceKHR)(VkInstance, const VkWin32SurfaceCreateInfoKHR *, const VkAllocationCallbacks *, VkSurfaceKHR *);
@@ -55,6 +57,10 @@ static inline void *get_vulkan_driver_device_proc_addr
 
     name += 2;
 
+    if (!strcmp(name, "AcquireNextImage2KHR"))
+        return vulkan_funcs->p_vkAcquireNextImage2KHR;
+    if (!strcmp(name, "AcquireNextImageKHR"))
+        return vulkan_funcs->p_vkAcquireNextImageKHR;
     if (!strcmp(name, "CreateSwapchainKHR"))
         return vulkan_funcs->p_vkCreateSwapchainKHR;
     if (!strcmp(name, "DestroySwapchainKHR"))
