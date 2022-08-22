--- dlls/winex11.drv/opengl.c.orig	2022-07-29 20:03:04 UTC
+++ dlls/winex11.drv/opengl.c
@@ -1955,20 +1955,20 @@ static BOOL WINAPI glxdrv_wglShareLists(struct wgl_con
 
 static void wglFinish(void)
 {
-    struct x11drv_escape_flush_gl_drawable escape;
+    struct x11drv_escape_present_drawable escape;
     struct gl_drawable *gl;
     struct wgl_context *ctx = NtCurrentTeb()->glContext;
 
-    escape.code = X11DRV_FLUSH_GL_DRAWABLE;
-    escape.gl_drawable = 0;
+    escape.code = X11DRV_PRESENT_DRAWABLE;
+    escape.drawable = 0;
     escape.flush = FALSE;
 
     if ((gl = get_gl_drawable( NtUserWindowFromDC( ctx->hdc ), 0 )))
     {
         switch (gl->type)
         {
-        case DC_GL_PIXMAP_WIN: escape.gl_drawable = gl->pixmap; break;
-        case DC_GL_CHILD_WIN:  escape.gl_drawable = gl->window; break;
+        case DC_GL_PIXMAP_WIN: escape.drawable = gl->pixmap; break;
+        case DC_GL_CHILD_WIN:  escape.drawable = gl->window; break;
         default: break;
         }
         sync_context(ctx);
@@ -1976,26 +1976,26 @@ static void wglFinish(void)
     }
 
     pglFinish();
-    if (escape.gl_drawable)
+    if (escape.drawable)
         NtGdiExtEscape( ctx->hdc, NULL, 0, X11DRV_ESCAPE, sizeof(escape), (LPSTR)&escape, 0, NULL );
 }
 
 static void wglFlush(void)
 {
-    struct x11drv_escape_flush_gl_drawable escape;
+    struct x11drv_escape_present_drawable escape;
     struct gl_drawable *gl;
     struct wgl_context *ctx = NtCurrentTeb()->glContext;
 
-    escape.code = X11DRV_FLUSH_GL_DRAWABLE;
-    escape.gl_drawable = 0;
+    escape.code = X11DRV_PRESENT_DRAWABLE;
+    escape.drawable = 0;
     escape.flush = FALSE;
 
     if ((gl = get_gl_drawable( NtUserWindowFromDC( ctx->hdc ), 0 )))
     {
         switch (gl->type)
         {
-        case DC_GL_PIXMAP_WIN: escape.gl_drawable = gl->pixmap; break;
-        case DC_GL_CHILD_WIN:  escape.gl_drawable = gl->window; break;
+        case DC_GL_PIXMAP_WIN: escape.drawable = gl->pixmap; break;
+        case DC_GL_CHILD_WIN:  escape.drawable = gl->window; break;
         default: break;
         }
         sync_context(ctx);
@@ -2003,7 +2003,7 @@ static void wglFlush(void)
     }
 
     pglFlush();
-    if (escape.gl_drawable)
+    if (escape.drawable)
         NtGdiExtEscape( ctx->hdc, NULL, 0, X11DRV_ESCAPE, sizeof(escape), (LPSTR)&escape, 0, NULL );
 }
 
@@ -3321,15 +3321,15 @@ static void X11DRV_WineGL_LoadExtensions(void)
  */
 static BOOL WINAPI glxdrv_wglSwapBuffers( HDC hdc )
 {
-    struct x11drv_escape_flush_gl_drawable escape;
+    struct x11drv_escape_present_drawable escape;
     struct gl_drawable *gl;
     struct wgl_context *ctx = NtCurrentTeb()->glContext;
     INT64 ust, msc, sbc, target_sbc = 0;
 
     TRACE("(%p)\n", hdc);
 
-    escape.code = X11DRV_FLUSH_GL_DRAWABLE;
-    escape.gl_drawable = 0;
+    escape.code = X11DRV_PRESENT_DRAWABLE;
+    escape.drawable = 0;
     escape.flush = !pglXWaitForSbcOML;
 
     if (!(gl = get_gl_drawable( NtUserWindowFromDC( hdc ), hdc )))
@@ -3350,7 +3350,7 @@ static BOOL WINAPI glxdrv_wglSwapBuffers( HDC hdc )
     {
     case DC_GL_PIXMAP_WIN:
         if (ctx) sync_context( ctx );
-        escape.gl_drawable = gl->pixmap;
+        escape.drawable = gl->pixmap;
         if (pglXCopySubBufferMESA) {
             /* (glX)SwapBuffers has an implicit glFlush effect, however
              * GLX_MESA_copy_sub_buffer doesn't. Make sure GL is flushed before
@@ -3371,10 +3371,10 @@ static BOOL WINAPI glxdrv_wglSwapBuffers( HDC hdc )
     case DC_GL_WINDOW:
     case DC_GL_CHILD_WIN:
         if (ctx) sync_context( ctx );
-        if (gl->type == DC_GL_CHILD_WIN) escape.gl_drawable = gl->window;
+        if (gl->type == DC_GL_CHILD_WIN) escape.drawable = gl->window;
         /* fall through */
     default:
-        if (escape.gl_drawable && pglXSwapBuffersMscOML)
+        if (escape.drawable && pglXSwapBuffersMscOML)
         {
             pglFlush();
             target_sbc = pglXSwapBuffersMscOML( gdi_display, gl->drawable, 0, 0, 0 );
@@ -3384,12 +3384,12 @@ static BOOL WINAPI glxdrv_wglSwapBuffers( HDC hdc )
         break;
     }
 
-    if (escape.gl_drawable && pglXWaitForSbcOML)
+    if (escape.drawable && pglXWaitForSbcOML)
         pglXWaitForSbcOML( gdi_display, gl->drawable, target_sbc, &ust, &msc, &sbc );
 
     release_gl_drawable( gl );
 
-    if (escape.gl_drawable)
+    if (escape.drawable)
         NtGdiExtEscape( ctx->hdc, NULL, 0, X11DRV_ESCAPE, sizeof(escape), (LPSTR)&escape, 0, NULL );
     return TRUE;
 }
