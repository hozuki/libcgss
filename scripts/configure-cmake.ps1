if ($env:CGSS_BUILD_JNI_LIBRARY)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "../bindings/java/scripts/Invoke-GenHeaders.ps1")

    if ( [System.IO.File]::Exists($scriptPath))
    {
        . ($scriptPath)
    }
}

if ($isWindows)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "configure-cmake-win.ps1")
}
elseif ($isLinux)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "configure-cmake-ubuntu.ps1")
}

if ($null -ne $scriptPath)
{
    . ($scriptPath)

    # Setup version info
    {
        [String]$buildVersion = $env:APPVEYOR_BUILD_VERSION
        [String[]]$versionParts = $buildVersion.Split('.')

        $env:LIBCGSS_VERSION_MAJOR = $versionParts[0]
        $env:LIBCGSS_VERSION_MINOR = $versionParts[1]
        $env:LIBCGSS_VERSION_PATCH = $versionParts[2]
        $env:LIBCGSS_VERSION_TWEAK = "0" # always "0"
    }
}
else
{
    Write-Error "Error: Platform not supported"
}
