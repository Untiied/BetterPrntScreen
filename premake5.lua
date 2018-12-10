--INCLUDE
function includeCURL()
   includedirs {"vendor/curl/include"}
end
function includeZIPLIB()
   includedirs {"vendor/ziplib/Source/"}
end
function includeLIBUI()
   includedirs {"vendor/libui"}
end
--INCLUDE
--DEBUG
function linkCURL_DEBUG()
   libdirs{"vendor/curl/build/lib/Debug"}
   links  {"libcurl-d_imp"}
end
function linkZIPLIB_DEBUG()
   libdirs{"vendor/ziplib/Bin/x86/Debug"}
   links  {"bzip2_d", "lzma_d", "ZipLib_d", "zlib_d"}
end
function linkLIBUI_DEBUG()
   libdirs{"vendor/libui/build/out/Debug"}
   links {"libui_d"}
end
--DEBUG
-- RELEASE
function linkCURL_RELEASE()
   libdirs{"vendor/curl/build/lib/Release"}
   links  {"libcurl_imp"}
end
function linkZIPLIB_RELEASE()
   libdirs{"vendor/ziplib/Bin/x86/Release"}
   links  {"bzip2", "lzma", "ZipLib", "zlib"}
end
function linkLIBUI_RELEASE()
   libdirs{"vendor/libui/build/out/Release"}
   links {"libui"}
end
-- RELEASE



workspace "BPS"
   configurations {"Debug", "Release", "LocalDebug", "LocalRelease"}
   cppdialect "C++17"

project "BetterPrntScreen"
   kind "ConsoleApp"
   language "C++"

   targetdir "build/%{cfg.buildcfg}"
   objdir "build-int/"
   files { "src/**.h", "src/**.cpp" }
 
   includeCURL()
   includeZIPLIB()
   includeLIBUI()

   filter "configurations:Debug"
      linkCURL_DEBUG()
      linkZIPLIB_DEBUG()
      linkLIBUI_DEBUG()
      defines { "DEBUG" }
      symbols "On"
 
   filter "configurations:Release"
      linkCURL_RELEASE()
      linkZIPLIB_RELEASE()
      linkLIBUI_RELEASE()
      defines { "NDEBUG" }
      optimize "On"  
  
   filter "configurations:LocalDebug"
      linkCURL_DEBUG()
      linkZIPLIB_DEBUG()
      linkLIBUI_DEBUG()
      defines { "DEBUG", "LOCALBUILD" }
      runtime("Debug")

   filter "configurations:LocalRelease"
      linkCURL_RELEASE()
      linkZIPLIB_RELEASE()
      linkLIBUI_RELEASE()
      defines { "NDEBUG", "LOCALBUILD" }
      optimize "On"  