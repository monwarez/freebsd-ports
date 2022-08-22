--- dlls/winex11.drv/vulkan.c.orig	2022-07-29 20:03:04 UTC
+++ dlls/winex11.drv/vulkan.c
@@ -35,6 +35,7 @@
 
 #include "wine/debug.h"
 #include "x11drv.h"
+#include "xcomposite.h"
 
 #define VK_NO_PROTOTYPES
 #define WINE_VK_HOST
@@ -50,6 +51,7 @@ WINE_DECLARE_DEBUG_CHANNEL(fps);
 static pthread_mutex_t vulkan_mutex;
 
 static XContext vulkan_hwnd_context;
+static XContext vulkan_swapchain_context;
 
 #define VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR 1000004000
 
@@ -61,6 +63,9 @@ struct wine_vk_surface
     struct list entry;
     Window window;
     VkSurfaceKHR surface; /* native surface */
+    VkPresentModeKHR present_mode;
+    BOOL offscreen; /* drawable is offscreen */
+    HDC hdc;
     HWND hwnd;
     DWORD hwnd_thread_id;
 };
@@ -74,6 +79,7 @@ typedef struct VkXlibSurfaceCreateInfoKHR
     Window window;
 } VkXlibSurfaceCreateInfoKHR;
 
+static VKResult (*pvkAcquireNextImageKHR)(VkDevice, VkSwapchainKHR, uint64_t, VkSemaphore, VkFence, uint32_t *);
 static VkResult (*pvkCreateInstance)(const VkInstanceCreateInfo *, const VkAllocationCallbacks *, VkInstance *);
 static VkResult (*pvkCreateSwapchainKHR)(VkDevice, const VkSwapchainCreateInfoKHR *, const VkAllocationCallbacks *, VkSwapchainKHR *);
 static VkResult (*pvkCreateXlibSurfaceKHR)(VkInstance, const VkXlibSurfaceCreateInfoKHR *, const VkAllocationCallbacks *, VkSurfaceKHR *);
@@ -94,6 +100,9 @@ static VkResult (*pvkGetPhysicalDeviceSurfaceSupportKH
 static VkBool32 (*pvkGetPhysicalDeviceXlibPresentationSupportKHR)(VkPhysicalDevice, uint32_t, Display *, VisualID);
 static VkResult (*pvkGetSwapchainImagesKHR)(VkDevice, VkSwapchainKHR, uint32_t *, VkImage *);
 static VkResult (*pvkQueuePresentKHR)(VkQueue, const VkPresentInfoKHR *);
+static VkResult (*pvkWaitForFences)(VkDevice device, uint32_t fenceCount, const VkFence *pFences, VkBool32 waitAll, uint64_t timeout);
+static VkResult (*pvkCreateFence)(VkDevice device, const VkFenceCreateInfo *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkFence *pFence);
+static void (*pvkDestroyFence)(VkDevice device, VkFence fence, const VkAllocationCallbacks *pAllocator);
 
 static void *X11DRV_get_vk_device_proc_addr(const char *name);
 static void *X11DRV_get_vk_instance_proc_addr(VkInstance instance, const char *name);
@@ -117,6 +126,7 @@ static void wine_vk_init(void)
 
 #define LOAD_FUNCPTR(f) if (!(p##f = dlsym(vulkan_handle, #f))) goto fail
 #define LOAD_OPTIONAL_FUNCPTR(f) p##f = dlsym(vulkan_handle, #f)
+    LOAD_FUNCPTR(vkAcquireNextImageKHR);
     LOAD_FUNCPTR(vkCreateInstance);
     LOAD_FUNCPTR(vkCreateSwapchainKHR);
     LOAD_FUNCPTR(vkCreateXlibSurfaceKHR);
@@ -137,10 +147,14 @@ static void wine_vk_init(void)
     LOAD_FUNCPTR(vkQueuePresentKHR);
     LOAD_OPTIONAL_FUNCPTR(vkGetDeviceGroupSurfacePresentModesKHR);
     LOAD_OPTIONAL_FUNCPTR(vkGetPhysicalDevicePresentRectanglesKHR);
+    LOAD_FUNCPTR(vkWaitForFences);
+    LOAD_FUNCPTR(vkCreateFence);
+    LOAD_FUNCPTR(vkDestroyFence);
 #undef LOAD_FUNCPTR
 #undef LOAD_OPTIONAL_FUNCPTR
 
     vulkan_hwnd_context = XUniqueContext();
+    vulkan_swapchain_context = XUniqueContext();
     return;
 
 fail:
@@ -223,17 +237,55 @@ static void wine_vk_surface_release(struct wine_vk_sur
 void wine_vk_surface_destroy(HWND hwnd)
 {
     struct wine_vk_surface *surface;
+    HDC hdc = 0;
     pthread_mutex_lock(&vulkan_mutex);
     if (!XFindContext(gdi_display, (XID)hwnd, vulkan_hwnd_context, (char **)&surface))
     {
+	hdc = surface->hdc;
         surface->hwnd_thread_id = 0;
-        surface->hwnd = NULL;
+        surface->hwnd = 0;
+	surface->hdc = 0;
         wine_vk_surface_release(surface);
     }
     XDeleteContext(gdi_display, (XID)hwnd, vulkan_hwnd_context);
     pthread_mutex_unlock(&vulkan_mutex);
+    if (hdc) ReleaseDC(hwnd, hdc);
 }
 
+static BOOL wine_vk_surface_set_offscreen(struct wine_vk_surface *surface, BOOL offscreen)
+{
+#ifdef SONAME_LIBXCOMPOSITE
+    if (usexcomposite)
+    {
+        if (!surface->offscreen && offscreen)
+        {
+            FIXME("Redirecting vulkan surface offscreen, expect degraded performance.\n");
+            pXCompositeRedirectWindow(gdi_display, surface->window, CompositeRedirectManual);
+        }
+        else if (surface->offscreen && !offscreen)
+        {
+            FIXME("Putting vulkan surface back onscreen, expect standard performance.\n");
+            pXCompositeUnredirectWindow(gdi_display, surface->window, CompositeRedirectManual);
+        }
+        surface->offscreen = offscreen;
+        return TRUE;
+    }
+#endif
+
+    if (offscreen) FIXME("Application requires child window rendering, which is not implemented yet!\n");
+    surface->offscreen = offscreen;
+    return !offscreen;
+}
+
+void sync_vk_surface(HWND hwnd, BOOL known_child)
+{
+    struct wine_vk_surface *surface;
+    pthread_mutex_lock(&vulkan_mutex);
+    if (!XFindContext(gdi_display, (XID)hwnd, vulkan_hwnd_context, (char **)&surface))
+        wine_vk_surface_set_offscreen(surface, known_child);
+    pthread_mutex_unlock(&vulkan_mutex);
+}
+
 void vulkan_thread_detach(void)
 {
     struct wine_vk_surface *surface, *next;
@@ -280,12 +332,77 @@ static VkResult X11DRV_vkCreateInstance(const VkInstan
     return res;
 }
 
+static VkResult X11DRV_vkAcquireNextImageKHR(VkDevice device,
+        VkSwapchainKHR swapchain, uint64_t timeout, VkSemaphore semaphore,
+        VkFence fence, uint32_t *image_index)
+{
+    static int once;
+    struct x11drv_escape_present_drawable escape;
+    struct wine_vk_surface *surface = NULL;
+    VkResult result;
+    VkFence orig_fence;
+    BOOL wait_fence = FALSE;
+    HDC hdc = 0;
+
+    pthread_mutex_lock(&vulkan_mutex);
+    if (!XFindContext(gdi_display, (XID)swapchain, vulkan_swapchain_context, (char **)&surface))
+    {
+        wine_vk_surface_grab(surface);
+        hdc = surface->hdc;
+    }
+    pthread_mutex_unlock(&vulkan_mutex);
+
+    if (!surface || !surface->offscreen)
+        wait_fence = FALSE;
+    else if (surface->present_mode == VK_PRESENT_MODE_MAILBOX_KHR ||
+             surface->present_mode == VK_PRESENT_MODE_FIFO_KHR)
+        wait_fence = TRUE;
+
+    orig_fence = fence;
+    if (wait_fence && !fence)
+    {
+        VkFenceCreateInfo create_info;
+        create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
+        create_info.pNext = NULL;
+        create_info.flags = 0;
+        pvkCreateFence(device, &create_info, NULL, &fence);
+    }
+
+
+    result = pvkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, image_index);
+    if (result == VK_SUCCESS && hdc && surface && surface->offscreen)
+    {
+        if (wait_fence) pvkWaitForFences(device, 1, &fence, 0, timeout);
+        escape.code = X11DRV_PRESENT_DRAWABLE;
+        escape.drawable = surface->window;
+        escape.flush = TRUE;
+        NtGdiExtEscape(hdc, NULL, 0, X11DRV_ESCAPE, sizeof(escape), (char *)&escape, 0, NULL);
+        if (surface->present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
+            if (once++) FIXME("Application requires child window rendering with mailbox present mode, expect possible tearing!\n");
+    }
+
+    if (fence != orig_fence) pvkDestroyFence(device, fence, NULL);
+    if (surface) wine_vk_surface_release(surface);
+    return result;
+}
+
+static VkResult X11DRV_vkAcquireNextImage2KHR(VkDevice device,
+        const VkAcquireNextImageInfoKHR *acquire_info, uint32_t *image_index)
+{
+    static int once;
+    if (!once++) FIXME("Emulating vkGetPhysicalDeviceSurfaceCapabilities2KHR with vkGetPhysicalDeviceSurfaceCapabilitiesKHR, pNext is ignored.\n");
+    return X11DRV_vkAcquireNextImageKHR(device, acquire_info->swapchain, acquire_info->timeout, acquire_info->semaphore, acquire_info->fence, image_index);
+}
+
+
 static VkResult X11DRV_vkCreateSwapchainKHR(VkDevice device,
         const VkSwapchainCreateInfoKHR *create_info,
         const VkAllocationCallbacks *allocator, VkSwapchainKHR *swapchain)
 {
     struct wine_vk_surface *x11_surface = surface_from_handle(create_info->surface);
     VkSwapchainCreateInfoKHR create_info_host;
+    VkResult result;
+
     TRACE("%p %p %p %p\n", device, create_info, allocator, swapchain);
 
     if (allocator)
@@ -297,7 +414,23 @@ static VkResult X11DRV_vkCreateSwapchainKHR(VkDevice d
     create_info_host = *create_info;
     create_info_host.surface = x11_surface->surface;
 
-    return pvkCreateSwapchainKHR(device, &create_info_host, NULL /* allocator */, swapchain);
+    /* force fifo when running offscreen so the acquire fence is more likely to be vsynced */
+    if (x11_surface->offscreen && create_info->presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
+        create_info_host.presentMode = VK_PRESENT_MODE_FIFO_KHR;
+    x11_surface->present_mode = create_info->presentMode;
+
+
+    result = pvkCreateSwapchainKHR(device, &create_info_host, NULL /* allocator */, swapchain);
+
+    if (result == VK_SUCCESS)
+    {
+	EnterCRiticalSection(&context_section);
+	XSaveContext(gdi_display, (XID)(*swapchain), vulkan_swapchain_context,
+		(char*)wine_vk_surface_grab(x11_surface));
+	LeaveCriticalSection(&context_section);
+    }
+
+    return result;
 }
 
 static VkResult X11DRV_vkCreateWin32SurfaceKHR(VkInstance instance,
@@ -313,13 +446,6 @@ static VkResult X11DRV_vkCreateWin32SurfaceKHR(VkInsta
     if (allocator)
         FIXME("Support for allocation callbacks not implemented yet\n");
 
-    /* TODO: support child window rendering. */
-    if (create_info->hwnd && NtUserGetAncestor(create_info->hwnd, GA_PARENT) != NtUserGetDesktopWindow())
-    {
-        FIXME("Application requires child window rendering, which is not implemented yet!\n");
-        return VK_ERROR_INCOMPATIBLE_DRIVER;
-    }
-
     x11_surface = calloc(1, sizeof(*x11_surface));
     if (!x11_surface)
         return VK_ERROR_OUT_OF_HOST_MEMORY;
@@ -328,6 +454,7 @@ static VkResult X11DRV_vkCreateWin32SurfaceKHR(VkInsta
     x11_surface->hwnd = create_info->hwnd;
     if (x11_surface->hwnd)
     {
+        x11_surface->hdc = NtUserGetDCEx(x11_surface->hwnd, 0, DCX_USESTYLE);
         x11_surface->window = create_client_window(create_info->hwnd, &default_visual);
         x11_surface->hwnd_thread_id = NtUserGetWindowThread(x11_surface->hwnd, NULL);
     }
@@ -345,6 +472,17 @@ static VkResult X11DRV_vkCreateWin32SurfaceKHR(VkInsta
         goto err;
     }
 
+    if (create_info->hwnd && (NtUserGetWindowRelative(create_info->hwnd, GW_CHILD) ||
+                              NtUserGetAncestor(create_info->hwnd, GA_PARENT) != NtUserGetDesktopWindow()))
+    {
+        if (!wine_vk_surface_set_offscreen(x11_surface, TRUE))
+        {
+            res = VK_ERROR_INCOMPATIBLE_DRIVER;
+            goto err;
+        }
+    }
+
+
     create_info_host.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
     create_info_host.pNext = NULL;
     create_info_host.flags = 0; /* reserved */
@@ -409,12 +547,22 @@ static void X11DRV_vkDestroySurfaceKHR(VkInstance inst
 static void X11DRV_vkDestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain,
          const VkAllocationCallbacks *allocator)
 {
+    struct wine_vk_surface *surface;
+
     TRACE("%p, 0x%s %p\n", device, wine_dbgstr_longlong(swapchain), allocator);
 
     if (allocator)
         FIXME("Support for allocation callbacks not implemented yet\n");
 
     pvkDestroySwapchainKHR(device, swapchain, NULL /* allocator */);
+
+    EnterCriticalSection(&context_section);
+    if (!XFindContext(gdi_display, (XID)swapchain, vulkan_swapchain_context, (char **)&surface))
+    {
+	wine_vk_surface_release(surface);
+    }
+    XDeleteContext(gdi_display, (XID)swapchain, vulkan_swapchain_context);
+    LeaveCriticalSection(&context_section);
 }
 
 static VkResult X11DRV_vkEnumerateInstanceExtensionProperties(const char *layer_name,
@@ -683,6 +831,8 @@ static VkSurfaceKHR X11DRV_wine_get_native_surface(VkS
 
 static const struct vulkan_funcs vulkan_funcs =
 {
+    X11DRV_vkAcquireNextImage2KHR,
+    X11DRV_vkAcquireNextImageKHR,
     X11DRV_vkCreateInstance,
     X11DRV_vkCreateSwapchainKHR,
     X11DRV_vkCreateWin32SurfaceKHR,
