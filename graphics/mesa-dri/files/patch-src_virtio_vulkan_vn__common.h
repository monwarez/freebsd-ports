--- src/virtio/vulkan/vn_common.h.orig	2025-08-06 16:32:17 UTC
+++ src/virtio/vulkan/vn_common.h
@@ -617,6 +617,8 @@ vn_gettid(void)
 {
 #if DETECT_OS_ANDROID
    return gettid();
+#elif defined(__FreeBSD__)
+   return syscall(SYS_thr_self);
 #else
    return syscall(SYS_gettid);
 #endif
