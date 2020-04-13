if ($isWindows)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "package-artifacts-win.ps1")
}
elseif ($isLinux)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "package-artifacts-ubuntu.ps1")
}

if ($null -ne $scriptPath)
{
    . ($scriptPath)
}
else
{
    Write-Error "Error: Platform not supported"
}
