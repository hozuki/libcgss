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
}
else
{
    Write-Error "Error: Platform not supported"
}
