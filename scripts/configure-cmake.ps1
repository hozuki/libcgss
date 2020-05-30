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
    # Setup version info
    if ($env:CI)
    {
        [String]$buildVersion = $env:APPVEYOR_BUILD_VERSION

        Write-Host "Using build version: $buildVersion"

        if (-not([String]::IsNullOrWhiteSpace($buildVersion)))
        {
            # OK I give up.
            $env:LIBCGSS_LIBRARY_VERSION = $buildVersion
        }
    }

    . ($scriptPath)
}
else
{
    Write-Error "Error: Platform not supported"
}
