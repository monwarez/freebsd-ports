--- components/metrics/system_memory_stats_recorder_linux.cc.orig	2019-09-09 21:55:14 UTC
+++ components/metrics/system_memory_stats_recorder_linux.cc
@@ -30,6 +30,7 @@ namespace metrics {
   UMA_HISTOGRAM_LINEAR(name, sample, 2500, 50)
 
 void RecordMemoryStats(RecordMemoryStatsType type) {
+#if !defined(OS_FREEBSD)
   base::SystemMemoryInfoKB memory;
   if (!base::GetSystemMemoryInfo(&memory))
     return;
@@ -81,6 +82,7 @@ void RecordMemoryStats(RecordMemoryStatsType type) {
       break;
     }
   }
+#endif // !defined(OS_FREEBSD)
 }
 
 }  // namespace metrics
