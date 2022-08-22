--- dlls/winex11.drv/window.c.orig	2022-07-29 20:03:04 UTC
+++ dlls/winex11.drv/window.c
@@ -1736,7 +1736,17 @@ void X11DRV_SetWindowStyle( HWND hwnd, INT offset, STY
 {
     struct x11drv_win_data *data;
     DWORD changed = style->styleNew ^ style->styleOld;
+    HWND parent = NtUserGetAncestor( hwnd, GA_PARENT );
 
+    if (offset == GWL_STYLE && (changed & WS_CHILD))
+    {
+        if (NtUserGetWindowRelative( parent, GW_CHILD ) ||
+            NtUserGetAncestor( parent, GA_PARENT ) != NtUserGetDesktopWindow())
+            sync_vk_surface( parent, TRUE );
+        else
+            sync_vk_surface( parent, FALSE );
+    }
+
     if (hwnd == NtUserGetDesktopWindow()) return;
     if (!(data = get_win_data( hwnd ))) return;
     if (!data->whole_window) goto done;
@@ -1762,7 +1772,12 @@ void X11DRV_DestroyWindow( HWND hwnd )
 {
     struct x11drv_thread_data *thread_data = x11drv_thread_data();
     struct x11drv_win_data *data;
+    HWND parent = NtUserGetAncestor( hwnd, GA_PARENT );
 
+    if (!NtUserGetWindowRelative( parent, GW_CHILD ) &&
+        NtUserGetAncestor( parent, GA_PARENT ) == NtUserGetDesktopWindow())
+        sync_vk_surface( parent, FALSE );
+
     if (!(data = get_win_data( hwnd ))) return;
 
     destroy_whole_window( data, FALSE );
@@ -1972,6 +1987,7 @@ static struct x11drv_win_data *X11DRV_create_win_data(
      * that will need clipping support.
      */
     sync_gl_drawable( parent, TRUE );
+    sync_vk_surface( parent, TRUE );
 
     display = thread_init_display();
     init_clip_window();  /* make sure the clip window is initialized in this thread */
@@ -2450,6 +2466,7 @@ done:
      * that will need clipping support.
      */
     sync_gl_drawable( parent, TRUE );
+    sync_vk_surface( parent, TRUe );
 
     fetch_icon_data( hwnd, 0, 0 );
 }
