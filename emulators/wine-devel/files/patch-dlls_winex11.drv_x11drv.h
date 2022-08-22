--- dlls/winex11.drv/x11drv.h.orig	2022-07-29 20:03:04 UTC
+++ dlls/winex11.drv/x11drv.h
@@ -333,7 +333,7 @@ enum x11drv_escape_codes
     X11DRV_GET_DRAWABLE,     /* get current drawable for a DC */
     X11DRV_START_EXPOSURES,  /* start graphics exposures */
     X11DRV_END_EXPOSURES,    /* end graphics exposures */
-    X11DRV_FLUSH_GL_DRAWABLE /* flush changes made to the gl drawable */
+    X11DRV_PRESENT_DRAWABLE /* flush changes made to the gl drawable */
 };
 
 struct x11drv_escape_set_drawable
@@ -352,10 +352,10 @@ struct x11drv_escape_get_drawable
     int                      pixel_format; /* internal GL pixel format */
 };
 
-struct x11drv_escape_flush_gl_drawable
+struct x11drv_escape_present_drawable
 {
-    enum x11drv_escape_codes code;         /* escape code (X11DRV_FLUSH_GL_DRAWABLE) */
-    Drawable                 gl_drawable;  /* GL drawable */
+    enum x11drv_escape_codes code;         /* escape code (X11DRV_PRESENT_DRAWABLE) */
+    Drawable                 drawable;  /* GL drawable */
     BOOL                     flush;        /* flush X11 before copying */
 };
 
@@ -435,6 +435,7 @@ extern BOOL show_systray DECLSPEC_HIDDEN;
 extern BOOL grab_pointer DECLSPEC_HIDDEN;
 extern BOOL grab_fullscreen DECLSPEC_HIDDEN;
 extern BOOL usexcomposite DECLSPEC_HIDDEN;
+extern BOOL use_xfixes DECLSPEC_HIDDEN;
 extern BOOL managed_mode DECLSPEC_HIDDEN;
 extern BOOL decorated_mode DECLSPEC_HIDDEN;
 extern BOOL private_color_map DECLSPEC_HIDDEN;
@@ -442,6 +443,7 @@ extern int primary_monitor DECLSPEC_HIDDEN;
 extern int copy_default_colors DECLSPEC_HIDDEN;
 extern int alloc_system_colors DECLSPEC_HIDDEN;
 extern int xrender_error_base DECLSPEC_HIDDEN;
+extern int xfixes_event_base DECLSPEC_HIDDEN;
 extern char *process_name DECLSPEC_HIDDEN;
 extern Display *clipboard_display DECLSPEC_HIDDEN;
 extern WNDPROC client_foreign_window_proc;
@@ -637,6 +639,7 @@ extern void sync_gl_drawable( HWND hwnd, BOOL known_ch
 extern void set_gl_drawable_parent( HWND hwnd, HWND parent ) DECLSPEC_HIDDEN;
 extern void destroy_gl_drawable( HWND hwnd ) DECLSPEC_HIDDEN;
 extern void wine_vk_surface_destroy( HWND hwnd ) DECLSPEC_HIDDEN;
+extern void sync_vk_surface( HWND hwnd, BOOL known_child ) DECLSPEC_HIDDEN;
 extern void vulkan_thread_detach(void) DECLSPEC_HIDDEN;
 
 extern void wait_for_withdrawn_state( HWND hwnd, BOOL set ) DECLSPEC_HIDDEN;
