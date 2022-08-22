--- dlls/winex11.drv/x11drv_main.c.orig	2022-07-29 20:03:04 UTC
+++ dlls/winex11.drv/x11drv_main.c
@@ -52,6 +52,7 @@
 #include "x11drv.h"
 #include "winreg.h"
 #include "xcomposite.h"
+#include "xfixes.h"
 #include "wine/server.h"
 #include "wine/debug.h"
 #include "wine/list.h"
@@ -72,6 +73,7 @@ Window root_window;
 BOOL usexvidmode = TRUE;
 BOOL usexrandr = TRUE;
 BOOL usexcomposite = TRUE;
+BOOL use_xfixes = FALSE;
 BOOL use_xkb = TRUE;
 BOOL use_take_focus = TRUE;
 BOOL use_primary_selection = FALSE;
@@ -89,6 +91,7 @@ BOOL shape_layered_windows = TRUE;
 int copy_default_colors = 128;
 int alloc_system_colors = 256;
 int xrender_error_base = 0;
+int xfixes_event_base = 0;
 char *process_name = NULL;
 WNDPROC client_foreign_window_proc = NULL;
 
@@ -596,6 +599,64 @@ sym_not_found:
 }
 #endif /* defined(SONAME_LIBXCOMPOSITE) */
 
+#ifdef SONAME_LIBXFIXES
+
+#define MAKE_FUNCPTR(f) typeof(f) * p##f;
+MAKE_FUNCPTR(XFixesQueryExtension)
+MAKE_FUNCPTR(XFixesQueryVersion)
+MAKE_FUNCPTR(XFixesCreateRegion)
+MAKE_FUNCPTR(XFixesCreateRegionFromGC)
+MAKE_FUNCPTR(XFixesSelectSelectionInput)
+#undef MAKE_FUNCPTR
+
+static void x11drv_load_xfixes(void)
+{
+    int event, error, major = 3, minor = 0;
+    void *xfixes;
+
+    if (!(xfixes = dlopen(SONAME_LIBXFIXES, RTLD_NOW)))
+    {
+        WARN("Xfixes library %s not found, disabled.\n", SONAME_LIBXFIXES);
+        return;
+    }
+
+#define LOAD_FUNCPTR(f) \
+    if (!(p##f = dlsym(xfixes, #f)))                          \
+    {                                                         \
+        WARN("Xfixes function %s not found, disabled\n", #f); \
+        dlclose(xfixes);                                      \
+        return;                                               \
+    }
+    LOAD_FUNCPTR(XFixesQueryExtension)
+    LOAD_FUNCPTR(XFixesQueryVersion)
+    LOAD_FUNCPTR(XFixesCreateRegion)
+    LOAD_FUNCPTR(XFixesCreateRegionFromGC)
+    LOAD_FUNCPTR(XFixesSelectSelectionInput)
+#undef LOAD_FUNCPTR
+
+    if (!pXFixesQueryExtension(gdi_display, &event, &error))
+    {
+        WARN("Xfixes extension not found, disabled.\n");
+        dlclose(xfixes);
+        return;
+    }
+
+    if (!pXFixesQueryVersion(gdi_display, &major, &minor) ||
+        major < 2)
+    {
+        WARN("Xfixes version 2.0 not found, disabled.\n");
+        dlclose(xfixes);
+        return;
+    }
+
+    TRACE("Xfixes, error %d, event %d, version %d.%d found\n",
+          error, event, major, minor);
+    use_xfixes = TRUE;
+    xfixes_event_base = event;
+}
+#endif /* SONAME_LIBXFIXES */
+
+
 static void init_visuals( Display *display, int screen )
 {
     int count;
@@ -701,6 +762,9 @@ static NTSTATUS x11drv_init( void *arg )
     X11DRV_XF86VM_Init();
     /* initialize XRandR */
     X11DRV_XRandR_Init();
+#ifdef SONAME_LIBXFIXES
+    x11drv_load_xfixes();
+#endif
 #ifdef SONAME_LIBXCOMPOSITE
     X11DRV_XComposite_Init();
 #endif
