--INCLUDE
function includeCURL()
   includedirs {"vendor/curl/include"}
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
   includeLIBUI()

   filter "configurations:Debug"
      linkCURL_DEBUG()
      linkLIBUI_DEBUG()
      defines { "DEBUG" }
      symbols "On"
 
   filter "configurations:Release"
      linkCURL_RELEASE()
      linkLIBUI_RELEASE()
      defines { "NDEBUG" }
      optimize "On"  
  
   filter "configurations:LocalDebug"
      linkCURL_DEBUG()
      linkLIBUI_DEBUG()
      defines { "DEBUG", "LOCALBUILD" }
      runtime("Debug")

   filter "configurations:LocalRelease"
      linkCURL_RELEASE()
      linkLIBUI_RELEASE()
      defines { "NDEBUG", "LOCALBUILD" }
      optimize "On"  