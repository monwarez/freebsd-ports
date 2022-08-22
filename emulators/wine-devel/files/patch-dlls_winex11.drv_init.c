--- dlls/winex11.drv/init.c.orig	2022-07-29 20:03:04 UTC
+++ dlls/winex11.drv/init.c
@@ -244,16 +244,16 @@ static INT CDECL X11DRV_ExtEscape( PHYSDEV dev, INT es
                     return TRUE;
                 }
                 break;
-            case X11DRV_FLUSH_GL_DRAWABLE:
-                if (in_count >= sizeof(struct x11drv_escape_flush_gl_drawable))
+            case X11DRV_PRESENT_DRAWABLE:
+                if (in_count >= sizeof(struct x11drv_escape_present_drawable))
                 {
-                    const struct x11drv_escape_flush_gl_drawable *data = in_data;
+                    const struct x11drv_escape_present_drawable *data = in_data;
                     RECT rect = physDev->dc_rect;
 
                     OffsetRect( &rect, -physDev->dc_rect.left, -physDev->dc_rect.top );
                     if (data->flush) XFlush( gdi_display );
                     XSetFunction( gdi_display, physDev->gc, GXcopy );
-                    XCopyArea( gdi_display, data->gl_drawable, physDev->drawable, physDev->gc,
+                    XCopyArea( gdi_display, data->drawable, physDev->drawable, physDev->gc,
                                0, 0, rect.right, rect.bottom,
                                physDev->dc_rect.left, physDev->dc_rect.top );
                     add_device_bounds( physDev, &rect );
