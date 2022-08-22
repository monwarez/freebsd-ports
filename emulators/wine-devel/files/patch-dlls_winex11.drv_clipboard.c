--- dlls/winex11.drv/clipboard.c.orig	2022-08-22 17:01:06 UTC
+++ dlls/winex11.drv/clipboard.c
@@ -83,6 +83,7 @@
 #include "ntstatus.h"
 #define WIN32_NO_STATUS
 #include "x11drv.h"
+#include "xfixes.h"
 
 #ifdef HAVE_X11_EXTENSIONS_XFIXES_H
 #include <X11/extensions/Xfixes.h>
@@ -199,7 +200,6 @@ static UINT rendered_formats;
 static ULONG last_clipboard_update;
 static struct clipboard_format **current_x11_formats;
 static unsigned int nb_current_x11_formats;
-static BOOL use_xfixes;
 
 Display *clipboard_display = NULL;
 
@@ -2170,28 +2170,6 @@ static BOOL selection_notify_event( HWND hwnd, XEvent 
 static void xfixes_init(void)
 {
 #ifdef SONAME_LIBXFIXES
-    typeof(XFixesSelectSelectionInput) *pXFixesSelectSelectionInput;
-    typeof(XFixesQueryExtension) *pXFixesQueryExtension;
-    typeof(XFixesQueryVersion) *pXFixesQueryVersion;
-
-    int event_base, error_base;
-    int major = 3, minor = 0;
-    void *handle;
-
-    handle = dlopen(SONAME_LIBXFIXES, RTLD_NOW);
-    if (!handle) return;
-
-    pXFixesQueryExtension = dlsym(handle, "XFixesQueryExtension");
-    if (!pXFixesQueryExtension) return;
-    pXFixesQueryVersion = dlsym(handle, "XFixesQueryVersion");
-    if (!pXFixesQueryVersion) return;
-    pXFixesSelectSelectionInput = dlsym(handle, "XFixesSelectSelectionInput");
-    if (!pXFixesSelectSelectionInput) return;
-
-    if (!pXFixesQueryExtension(clipboard_display, &event_base, &error_base))
-        return;
-    pXFixesQueryVersion(clipboard_display, &major, &minor);
-    use_xfixes = (major >= 1);
     if (!use_xfixes) return;
 
     pXFixesSelectSelectionInput(clipboard_display, import_window, x11drv_atom(CLIPBOARD),
@@ -2205,7 +2183,7 @@ static void xfixes_init(void)
                 XFixesSelectionWindowDestroyNotifyMask |
                 XFixesSelectionClientCloseNotifyMask);
     }
-    X11DRV_register_event_handler(event_base + XFixesSelectionNotify,
+    X11DRV_register_event_handler(xfixes_event_base + XFixesSelectionNotify,
             selection_notify_event, "XFixesSelectionNotify");
     TRACE("xfixes succesully initialized\n");
 #else
