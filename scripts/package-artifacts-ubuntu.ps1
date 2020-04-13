function Get-UbuntuCodeName()
{
    [String]$osReleaseInfo = Get-Content /etc/os-release | Out-String
    [String]$codeNameLine = Write-Output $osReleaseInfo | grep 'UBUNTU_CODENAME=' | Out-String
    $codeNameLine = $codeNameLine.Trim()
    [String[]]$parts = $codeNameLine.Split('=')
    return $parts[1]
}

Set-Location $env:APPVEYOR_BUILD_FOLDER

# Detected in common_defines.cmake; but Ubuntu 18.04+ only supports 64-bit apps so it's a constant
[String]$cmakeArch = "x64";
# Set in build.sh
[String]$cmakeBuildType = "MinSizeRel";

& 7z a libcgss.zip -r "bin/$cmakeArch/$cmakeBuildType/*"

[String]$codeName = Get-UbuntuCodeName
[String]$buildVersion = $Env:APPVEYOR_BUILD_VERSION;

Push-AppveyorArtifact "libcgss.zip" -FileName "libcgss-bin-$codeName-$buildVersion.zip" -DeploymentName "Binaries"
Push-AppveyorArtifact "libcgss.zip" -FileName "libcgss-ci-$codeName-latest.zip" -DeploymentName "CI auto update"
