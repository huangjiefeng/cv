rem 复制一些编译后的dll到debug目录下，以防止重新生成工程
copy "libs\webcam\debug\webcam.dll" ".\debug\" /Y
copy "wifo\owbase\debug\owbase.dll"  ".\debug\" /Y
copy "wifo\owsl\debug\owsl.dll"   ".\debug\" /Y
copy "libs\3rdparty\glib\glib-2.18.1\glib\debug\glib.dll"  ".\debug\" /Y
copy "wifo\phapi\debug\phapi.dll"  ".\debug\" /Y
rem pause