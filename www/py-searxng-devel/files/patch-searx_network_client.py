--- searx/network/client.py.orig	2024-11-08 07:51:43 UTC
+++ searx/network/client.py
@@ -138,7 +138,7 @@ def get_transport(verify, http2, local_address, proxy_
 
 
 def get_transport(verify, http2, local_address, proxy_url, limit, retries):
-    verify = get_sslcontexts(None, None, verify, True, http2) if verify is True else verify
+    verify = True
     return httpx.AsyncHTTPTransport(
         # pylint: disable=protected-access
         verify=verify,
