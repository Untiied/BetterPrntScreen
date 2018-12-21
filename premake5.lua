--INCLUDE
function includeCURL()
   includedirs {"vendor/curl/include"}
end
function includeLIBUI()
   includedirs {"vendor/libui"}
end
function includeZIPPER()
   includedirs {"vendor/zipper"}
end
function includeNANA()
   includedirs {"vendor/nana/include"}
end
--INCLUDE
--DEBUG
function linkCURL_DEBUG()
   libdirs{"vendor/libs"}
   links  {"libcurl-d_imp"}
end
function linkLIBUI_DEBUG()
   libdirs{"vendor/libs"}
   links {"libui_d"}
end
function linkZIPPER_DEBUG()
   libdirs{"vendor/libs"}
   links {"zlib_d", "libZipper-static_d"}
end
function linkNANA_DEBUG()
   libdirs{"vendor/libs"}
   links {"nana_d"}
end
--DEBUG
-- RELEASE
function linkCURL_RELEASE()
   libdirs{"vendor/libs"}
   links  {"libcurl_imp"}
end
function linkLIBUI_RELEASE()
   libdirs{"vendor/libs"}
   links {"libui"}
end
function linkZIPPER_RELEASE()
   libdirs{"vendor/libs"}
   links {"zlib", "libZipper-static"}
end
function linkNANA_RELEASE()
   libdirs{"vendor/libs"}
   links {"nana"}
end
-- RELEASE

workspace "BPS"
   configurations {"Debug", "Release", "LocalDebug", "LocalRelease"}
   cppdialect "C++17"

project "BPSUpdater"
   kind "ConsoleApp"
   language "C++"

   targetdir "build/%{cfg.buildcfg}"
   objdir "build-int/updater/"
   files { "updater/src/**.h", "updater/src/**.cpp" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"
 
   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"  

project "BetterPrntScreen"
   kind "ConsoleApp"
   language "C++"

   targetdir "build/%{cfg.buildcfg}"
   objdir "build-int/"
   files { "src/**.h", "src/**.cpp" }
 
   includeCURL()
   includeLIBUI()
   includeZIPPER()
   includeNANA()

   filter "configurations:Debug"
      linkCURL_DEBUG()
      linkLIBUI_DEBUG()
      linkZIPPER_DEBUG()
      linkNANA_DEBUG()
      defines { "DEBUG" }
      symbols "On"
 
   filter "configurations:Release"
      linkCURL_RELEASE()
      linkLIBUI_RELEASE()
      linkZIPPER_RELEASE()
      linkNANA_RELEASE()
      defines { "NDEBUG" }
      optimize "On"  
  
   filter "configurations:LocalDebug"
      linkCURL_DEBUG()
      linkLIBUI_DEBUG()
      linkZIPPER_DEBUG()
      linkNANA_DEBUG()
      defines { "DEBUG", "LOCALBUILD" }
      runtime("Debug")

   filter "configurations:LocalRelease"
      linkCURL_RELEASE()
      linkLIBUI_RELEASE()
      linkZIPPER_RELEASE()
      linkNANA_RELEASE()
      defines { "NDEBUG", "LOCALBUILD" }
      optimize "On"  