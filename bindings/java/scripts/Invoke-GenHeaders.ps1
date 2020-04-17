param(
    [Parameter(Mandatory = $false)][String]$jdkVersion = "11.0.6"
)

if (-not($env:JAVA_HOME))
{
    $env:JAVA_HOME = "C:\Program Files\Java\jdk-$jdkVersion"
}

[String]$srcDir = [System.IO.Path]::Combine($PSScriptRoot, "..", "src")
[String]$clsDir = [System.IO.Path]::Combine($PSScriptRoot, "..", "target", "classes")
[String]$hdrDir = [System.IO.Path]::Combine($PSScriptRoot, "..", "target", "headers")
[String]$scriptEntry = [System.IO.Path]::Combine($PSScriptRoot, "Gen-Headers.ps1")

. "$scriptEntry" -sourceDirectory $srcDir -classDirectory $clsDir -headerDirectory $hdrDir
