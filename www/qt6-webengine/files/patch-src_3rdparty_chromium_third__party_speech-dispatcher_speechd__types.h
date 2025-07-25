--- src/3rdparty/chromium/third_party/speech-dispatcher/speechd_types.h.orig	2023-12-10 06:10:27 UTC
+++ src/3rdparty/chromium/third_party/speech-dispatcher/speechd_types.h
@@ -119,4 +119,139 @@ typedef struct {
 	SPDVoice voice;
 } SPDMsgSettings;
 
+/* TEMP_FAILURE_RETRY seems to be available only on Linux. For systems that
+ * don't have this macro we provide our own version. This code was taken from
+ * file "/usr/include/unistd.h" from Debian package "libc6-dev"
+ * version 2.3.2.ds1-20. */
+#ifndef TEMP_FAILURE_RETRY
+#define TEMP_FAILURE_RETRY(expression) \
+	(__extension__ \
+	 ({ long int __result; \
+		do __result = (long int) (expression); \
+		while (__result == -1L && errno == EINTR); \
+		__result; }))
+#endif
+
+#endif /* not ifndef SPEECHD_TYPES */
+
+/*
+ * speechd_types.h - types for Speech Dispatcher
+ *
+ * Copyright (C) 2001, 2002, 2003 Brailcom, o.p.s.
+ *
+ * This is free software; you can redistribute it and/or modify it
+ * under the terms of the GNU Lesser General Public License as published by
+ * the Free Software Foundation; either version 2.1, or (at your option)
+ * any later version.
+ *
+ * This software is distributed in the hope that it will be useful,
+ * but WITHOUT ANY WARRANTY; without even the implied warranty of
+ * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
+ * Lesser General Public License for more details.
+ *
+ * You should have received a copy of the GNU Lesser General Public License
+ * along with this program.  If not, see <https://www.gnu.org/licenses/>.
+ */
+
+#ifndef SPEECHD_TYPES_H
+#define SPEECHD_TYPES_H
+
+typedef enum {
+	SPD_PUNCT_ALL = 0,
+	SPD_PUNCT_NONE = 1,
+	SPD_PUNCT_SOME = 2,
+	SPD_PUNCT_MOST = 3
+} SPDPunctuation;
+
+typedef enum {
+	SPD_CAP_NONE = 0,
+	SPD_CAP_SPELL = 1,
+	SPD_CAP_ICON = 2
+} SPDCapitalLetters;
+
+typedef enum {
+	SPD_SPELL_OFF = 0,
+	SPD_SPELL_ON = 1
+} SPDSpelling;
+
+typedef enum {
+	SPD_MALE1 = 1,
+	SPD_MALE2 = 2,
+	SPD_MALE3 = 3,
+	SPD_FEMALE1 = 4,
+	SPD_FEMALE2 = 5,
+	SPD_FEMALE3 = 6,
+	SPD_CHILD_MALE = 7,
+	SPD_CHILD_FEMALE = 8,
+	SPD_UNSPECIFIED = -1
+} SPDVoiceType;
+
+typedef struct {
+	char *name;		/* Name of the voice (id) */
+	char *language;		/* 2/3-letter ISO language code,
+				 * possibly followed by 2/3-letter ISO region code,
+				 * e.g. en-US */
+	char *variant;		/* a not-well defined string describing dialect etc. */
+} SPDVoice;
+
+typedef enum {
+	SPD_DATA_TEXT = 0,
+	SPD_DATA_SSML = 1
+} SPDDataMode;
+
+typedef enum {
+	SPD_IMPORTANT = 1,
+	SPD_MESSAGE = 2,
+	SPD_TEXT = 3,
+	SPD_NOTIFICATION = 4,
+	SPD_PROGRESS = 5
+} SPDPriority;
+
+typedef enum {
+	SPD_BEGIN = 1,
+	SPD_END = 2,
+	SPD_INDEX_MARKS = 4,
+	SPD_CANCEL = 8,
+	SPD_PAUSE = 16,
+	SPD_RESUME = 32,
+
+	SPD_ALL = 0x3f
+} SPDNotification;
+
+typedef enum {
+	SPD_EVENT_BEGIN,
+	SPD_EVENT_END,
+	SPD_EVENT_INDEX_MARK,
+	SPD_EVENT_CANCEL,
+	SPD_EVENT_PAUSE,
+	SPD_EVENT_RESUME
+} SPDNotificationType;
+
+typedef enum {
+	SORT_BY_TIME = 0,
+	SORT_BY_ALPHABET = 1
+} ESort;
+
+typedef enum {
+	SPD_MSGTYPE_TEXT = 0,
+	SPD_MSGTYPE_SOUND_ICON = 1,
+	SPD_MSGTYPE_CHAR = 2,
+	SPD_MSGTYPE_KEY = 3,
+	SPD_MSGTYPE_SPELL = 99
+} SPDMessageType;
+
+typedef struct {
+	signed int rate;
+	signed int pitch;
+	signed int pitch_range;
+	signed int volume;
+
+	SPDPunctuation punctuation_mode;
+	SPDSpelling spelling_mode;
+	SPDCapitalLetters cap_let_recogn;
+
+	SPDVoiceType voice_type;
+	SPDVoice voice;
+} SPDMsgSettings;
+
 #endif /* not ifndef SPEECHD_TYPES */
