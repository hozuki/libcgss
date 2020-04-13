
if ($isWindows)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "build-project-win.ps1")
}
elseif ($isLinux)
{
    $scriptPath = [System.IO.Path]::Combine($PSScriptRoot, "build-project-ubuntu.ps1")
}

if ($null -ne $scriptPath)
{
    . ($scriptPath)
}
else
{
    Write-Error "Error: Platform not supported"
}
