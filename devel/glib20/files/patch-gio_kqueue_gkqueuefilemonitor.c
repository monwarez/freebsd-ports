--- gio/kqueue/gkqueuefilemonitor.c.orig	2018-04-07 04:10:22.000000000 +0200
+++ gio/kqueue/gkqueuefilemonitor.c	2018-05-03 20:21:55.830634000 +0200
@@ -22,33 +22,73 @@
 
 #include "config.h"
 
-#include "gkqueuefilemonitor.h"
-#include "kqueue-helper.h"
-#include "kqueue-exclusions.h"
+#include <sys/types.h>
+#include <sys/event.h>
+#include <sys/time.h>
+#include <sys/socket.h>
+#include <sys/stat.h>
+
+#include <errno.h>
+#include <fcntl.h>
+#include <string.h>
+
+#include <glib-object.h>
+#include <gio/gfilemonitor.h>
+#include <gio/glocalfilemonitor.h>
+#include <gio/giomodule.h>
 #include <gio/gpollfilemonitor.h>
 #include <gio/gfile.h>
-#include <gio/giomodule.h>
+#include <glib-unix.h>
+#include "glib-private.h"
 
+#include "kqueue-helper.h"
+#include "dep-list.h"
 
-struct _GKqueueFileMonitor
+G_LOCK_DEFINE_STATIC (kq_lock);
+static GSource       *kq_source;
+static int	      kq_queue = -1;
+
+#define G_TYPE_KQUEUE_FILE_MONITOR	(g_kqueue_file_monitor_get_type ())
+#define G_KQUEUE_FILE_MONITOR(inst)	(G_TYPE_CHECK_INSTANCE_CAST ((inst), \
+					G_TYPE_KQUEUE_FILE_MONITOR, GKqueueFileMonitor))
+
+typedef GLocalFileMonitorClass GKqueueFileMonitorClass;
+
+typedef struct
 {
   GLocalFileMonitor parent_instance;
 
   kqueue_sub *sub;
-
+#ifndef O_EVTONLY
   GFileMonitor *fallback;
   GFile *fbfile;
-};
+#endif
+} GKqueueFileMonitor;
 
+GType g_kqueue_file_monitor_get_type (void);
+G_DEFINE_TYPE_WITH_CODE (GKqueueFileMonitor, g_kqueue_file_monitor, G_TYPE_LOCAL_FILE_MONITOR,
+	g_io_extension_point_implement (G_LOCAL_FILE_MONITOR_EXTENSION_POINT_NAME,
+		g_define_type_id,
+                "kqueue",
+		20))
+
+#ifndef O_EVTONLY
+#define O_KQFLAG O_RDONLY
+#else
+#define O_KQFLAG O_EVTONLY
+#endif
+
+#define NOTE_ALL (NOTE_DELETE|NOTE_WRITE|NOTE_EXTEND|NOTE_ATTRIB|NOTE_RENAME)
+
 static gboolean g_kqueue_file_monitor_cancel (GFileMonitor* monitor);
+static gboolean g_kqueue_file_monitor_is_supported (void);
 
-G_DEFINE_TYPE_WITH_CODE (GKqueueFileMonitor, g_kqueue_file_monitor, G_TYPE_LOCAL_FILE_MONITOR,
-       g_io_extension_point_implement (G_LOCAL_FILE_MONITOR_EXTENSION_POINT_NAME,
-               g_define_type_id,
-               "kqueue",
-               20))
+static kqueue_sub	*_kqsub_new (const gchar *, GLocalFileMonitor *, GFileMonitorSource *);
+static void		 _kqsub_free (kqueue_sub *);
+static gboolean		 _kqsub_cancel (kqueue_sub *);
 
 
+#ifndef O_EVTONLY
 static void
 _fallback_callback (GFileMonitor      *unused,
                     GFile             *first,
@@ -57,22 +97,42 @@ _fallback_callback (GFileMonitor      *unused,
                     gpointer           udata)
 {
   GKqueueFileMonitor *kq_mon = G_KQUEUE_FILE_MONITOR (udata);
-  GFileMonitor *mon = G_FILE_MONITOR (kq_mon);
-  g_assert (kq_mon != NULL);
-  g_assert (mon != NULL);
-  (void) unused;
 
-  if (event == G_FILE_MONITOR_EVENT_CHANGED)
-    {
-      GLocalFileMonitor *local_monitor = G_LOCAL_FILE_MONITOR (kq_mon);
-
-      _kh_dir_diff (kq_mon->sub, local_monitor->source);
-    }
-  else
-    g_file_monitor_emit_event (mon, first, second, event);
+  g_file_monitor_emit_event (G_FILE_MONITOR (kq_mon), first, second, event);
 }
 
+/*
+ * _ke_is_excluded:
+ * @full_path - a path to file to check.
+ *
+ * Returns: TRUE if the file should be excluded from the kqueue-powered
+ *      monitoring, FALSE otherwise.
+ **/
+gboolean
+_ke_is_excluded (const char *full_path)
+{
+  GFile *f = NULL;
+  GMount *mount = NULL;
 
+  f = g_file_new_for_path (full_path);
+
+  if (f != NULL) {
+    mount = g_file_find_enclosing_mount (f, NULL, NULL);
+    g_object_unref (f);
+  }
+
+  if ((mount != NULL && (g_mount_can_unmount (mount))) || g_str_has_prefix (full_path, "/mnt/"))
+  {
+    g_warning ("Excluding %s from kernel notification, falling back to poll", full_path);
+    if (mount)
+      g_object_unref (mount);
+    return TRUE;
+  }
+
+  return FALSE;
+}
+#endif /* !O_EVTONLY */
+
 static void
 g_kqueue_file_monitor_finalize (GObject *object)
 {
@@ -80,16 +140,18 @@ g_kqueue_file_monitor_finalize (GObject *object)
 
   if (kqueue_monitor->sub)
     {
-      _kh_cancel_sub (kqueue_monitor->sub);
-      _kh_sub_free (kqueue_monitor->sub);
+      _kqsub_cancel (kqueue_monitor->sub);
+      _kqsub_free (kqueue_monitor->sub);
       kqueue_monitor->sub = NULL;
     }
 
+#ifndef O_EVTONLY
   if (kqueue_monitor->fallback)
     g_object_unref (kqueue_monitor->fallback);
 
   if (kqueue_monitor->fbfile)
     g_object_unref (kqueue_monitor->fbfile);
+#endif
 
   if (G_OBJECT_CLASS (g_kqueue_file_monitor_parent_class)->finalize)
     (*G_OBJECT_CLASS (g_kqueue_file_monitor_parent_class)->finalize) (object);
@@ -103,21 +165,25 @@ g_kqueue_file_monitor_start (GLocalFileMonitor *local_
                              GFileMonitorSource *source)
 {
   GKqueueFileMonitor *kqueue_monitor = G_KQUEUE_FILE_MONITOR (local_monitor);
-  GObject *obj;
-  GKqueueFileMonitorClass *klass;
-  GObjectClass *parent_class;
-  kqueue_sub *sub = NULL;
-  gboolean ret_kh_startup = FALSE;
-  const gchar *path = NULL; 
+  kqueue_sub *sub;
+  const gchar *path;
 
-
-  ret_kh_startup = _kh_startup ();
-  g_assert (ret_kh_startup);
-
   path = filename;
-  if (!path)
+  if (path == NULL)
     path = dirname;
 
+#ifndef O_EVTONLY
+  if (_ke_is_excluded (path))
+    {
+      GFile *file = g_file_new_for_path (path);
+      kqueue_monitor->fbfile = file;
+      kqueue_monitor->fallback = _g_poll_file_monitor_new (file);
+      g_signal_connect (kqueue_monitor->fallback, "changed",
+			G_CALLBACK (_fallback_callback), kqueue_monitor);
+      return;
+    }
+#endif
+
   /* For a directory monitor, create a subscription object anyway.
    * It will be used for directory diff calculation routines. 
    * Wait, directory diff in a GKqueueFileMonitor?
@@ -125,35 +191,15 @@ g_kqueue_file_monitor_start (GLocalFileMonitor *local_
    * file, GIO uses a GKqueueFileMonitor object for that. If a directory
    * will be created under that path, GKqueueFileMonitor will have to
    * handle the directory notifications. */
+  sub = _kqsub_new (path, local_monitor, source);
+  if (sub == NULL)
+    return;
 
-  sub = _kh_sub_new (path, TRUE, source);
-
-  /* FIXME: what to do about errors here? we can't return NULL or another
-   * kind of error and an assertion is probably too hard (same issue as in
-   * the inotify backend) */
-  g_assert (sub != NULL);
   kqueue_monitor->sub = sub;
-
-  if (!_ke_is_excluded (path))
-    _kh_add_sub (sub);
-  else
-    {
-      GFile *file = g_file_new_for_path (path);
-      kqueue_monitor->fbfile = file;
-      kqueue_monitor->fallback = _g_poll_file_monitor_new (file);
-      g_signal_connect (kqueue_monitor->fallback,
-                        "changed",
-                        G_CALLBACK (_fallback_callback),
-                        kqueue_monitor);
-    }
+  if (!_kqsub_start_watching (sub))
+    _km_add_missing (sub);
 }
 
-static gboolean
-g_kqueue_file_monitor_is_supported (void)
-{
-  return _kh_startup ();
-}
-
 static void
 g_kqueue_file_monitor_class_init (GKqueueFileMonitorClass *klass)
 {
@@ -175,24 +221,218 @@ g_kqueue_file_monitor_init (GKqueueFileMonitor *monito
 }
 
 static gboolean
+g_kqueue_file_monitor_callback (gint fd, GIOCondition condition, gpointer user_data)
+{
+  gint64 now = g_source_get_time (kq_source);
+  kqueue_sub *sub;
+  GFileMonitorSource *source;
+  struct kevent ev;
+  struct timespec ts;
+
+  memset (&ts, 0, sizeof(ts));
+  while (kevent(fd, NULL, 0, &ev, 1, &ts) > 0)
+    {
+        GFileMonitorEvent mask = 0;
+
+        if (ev.filter != EVFILT_VNODE || ev.udata == NULL)
+          continue;
+
+	sub = ev.udata;
+        source = sub->source;
+
+        if (ev.flags & EV_ERROR)
+          ev.fflags = NOTE_REVOKE;
+
+        if (ev.fflags & (NOTE_DELETE | NOTE_REVOKE))
+          {
+            _kqsub_cancel (sub);
+            _km_add_missing (sub);
+          }
+
+        if (sub->is_dir && ev.fflags & (NOTE_WRITE | NOTE_EXTEND))
+          {
+            _kh_dir_diff (sub);
+            ev.fflags &= ~(NOTE_WRITE | NOTE_EXTEND);
+          }
+
+        if (ev.fflags & NOTE_DELETE)
+          {
+            mask = G_FILE_MONITOR_EVENT_DELETED;
+          }
+        else if (ev.fflags & NOTE_ATTRIB)
+          {
+            mask = G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED;
+          }
+        else if (ev.fflags & (NOTE_WRITE | NOTE_EXTEND))
+          {
+            mask = G_FILE_MONITOR_EVENT_CHANGED;
+          }
+        else if (ev.fflags & NOTE_RENAME)
+          {
+            /* Since there???s apparently no way to get the new name of the
+             * file out of kqueue(), all we can do is say that this one has
+             * been deleted. */
+            mask = G_FILE_MONITOR_EVENT_DELETED;
+          }
+        else if (ev.fflags & NOTE_REVOKE)
+          {
+            mask = G_FILE_MONITOR_EVENT_UNMOUNTED;
+          }
+
+        if (mask)
+          g_file_monitor_source_handle_event (source, mask, NULL, NULL, NULL, now);
+    }
+
+  return TRUE;
+}
+
+static gboolean
+g_kqueue_file_monitor_is_supported (void)
+{
+  int errsv;
+
+  G_LOCK (kq_lock);
+
+  if (kq_queue == -1)
+    {
+      kq_queue = kqueue ();
+      errsv = errno;
+
+      if (kq_queue == -1)
+        {
+          g_warning ("Unable to create a kqueue: %s", g_strerror (errsv));
+          G_UNLOCK (kq_lock);
+          return FALSE;
+        }
+
+      kq_source = g_unix_fd_source_new (kq_queue, G_IO_IN);
+      g_source_set_callback (kq_source, (GSourceFunc) g_kqueue_file_monitor_callback, NULL, NULL);
+      g_source_attach (kq_source, GLIB_PRIVATE_CALL (g_get_worker_context) ());
+    }
+
+  G_UNLOCK (kq_lock);
+
+  return TRUE;
+}
+
+static gboolean
 g_kqueue_file_monitor_cancel (GFileMonitor *monitor)
 {
   GKqueueFileMonitor *kqueue_monitor = G_KQUEUE_FILE_MONITOR (monitor);
 
   if (kqueue_monitor->sub)
     {
-      _kh_cancel_sub (kqueue_monitor->sub);
-      _kh_sub_free (kqueue_monitor->sub);
+      _kqsub_cancel (kqueue_monitor->sub);
+      _kqsub_free (kqueue_monitor->sub);
       kqueue_monitor->sub = NULL;
     }
+#ifndef O_EVTONLY
   else if (kqueue_monitor->fallback)
     {
       g_signal_handlers_disconnect_by_func (kqueue_monitor->fallback, _fallback_callback, kqueue_monitor);
       g_file_monitor_cancel (kqueue_monitor->fallback);
     }
+#endif
 
   if (G_FILE_MONITOR_CLASS (g_kqueue_file_monitor_parent_class)->cancel)
     (*G_FILE_MONITOR_CLASS (g_kqueue_file_monitor_parent_class)->cancel) (monitor);
+
+  return TRUE;
+}
+
+static kqueue_sub *
+_kqsub_new (const gchar *filename, GLocalFileMonitor *mon, GFileMonitorSource *source)
+{
+  kqueue_sub *sub;
+
+  sub = g_slice_new (kqueue_sub);
+  sub->filename = g_strdup (filename);
+  sub->mon = mon;
+  g_source_ref ((GSource *) source);
+  sub->source = source;
+  sub->fd = -1;
+  sub->deps = NULL;
+  sub->is_dir = 0;
+
+  return sub;
+}
+
+static void
+_kqsub_free (kqueue_sub *sub)
+{
+  g_assert (sub->deps == NULL);
+  g_assert (sub->fd == -1);
+
+  g_source_unref ((GSource *) sub->source);
+  g_free (sub->filename);
+  g_slice_free (kqueue_sub, sub);
+}
+
+static gboolean
+_kqsub_cancel (kqueue_sub *sub)
+{
+  struct kevent ev;
+
+  /* Remove the event and close the file descriptor to automatically
+   * delete pending events. */
+  if (sub->fd != -1)
+    {
+      EV_SET (&ev, sub->fd, EVFILT_VNODE, EV_DELETE, NOTE_ALL, 0, sub);
+      if (kevent (kq_queue, &ev, 1, NULL, 0, NULL) == -1)
+        {
+          g_warning ("Unable to remove event for %s: %s", sub->filename, g_strerror (errno));
+          return FALSE;
+        }
+      close (sub->fd);
+      sub->fd = -1;
+    }
+
+  _km_remove (sub);
+
+  if (sub->deps)
+    {
+      dl_free (sub->deps);
+      sub->deps = NULL;
+    }
+
+  return TRUE;
+}
+
+gboolean
+_kqsub_start_watching (kqueue_sub *sub)
+{
+  struct stat st;
+  struct kevent ev;
+
+  sub->fd = open (sub->filename, O_KQFLAG);
+  if (sub->fd == -1)
+      return FALSE;
+
+  if (fstat (sub->fd, &st) == -1)
+    {
+      g_warning ("fstat failed for %s: %s", sub->filename, g_strerror (errno));
+      close (sub->fd);
+      sub->fd = -1;
+      return FALSE;
+    }
+
+  sub->is_dir = (st.st_mode & S_IFDIR) ? 1 : 0;
+  if (sub->is_dir)
+    {
+      if (sub->deps)
+        dl_free (sub->deps);
+
+      sub->deps = dl_listing (sub->filename);
+    }
+
+  EV_SET (&ev, sub->fd, EVFILT_VNODE, EV_ADD | EV_CLEAR, NOTE_ALL, 0, sub);
+  if (kevent (kq_queue, &ev, 1, NULL, 0, NULL) == -1)
+    {
+      g_warning ("Unable to add event for %s: %s", sub->filename, g_strerror (errno));
+      close (sub->fd);
+      sub->fd = -1;
+      return FALSE;
+    }
 
   return TRUE;
 }
