function Get-UbuntuCodeName()
{
    try
    {
        [String]$osReleaseInfo = Get-Content /etc/os-release | Out-String
        [String]$codeNameLine = Write-Output $osReleaseInfo | grep 'UBUNTU_CODENAME=' | Out-String
        $codeNameLine = $codeNameLine.Trim()
        [String[]]$parts = $codeNameLine.Split('=')

        if ($parts.Length -gt 1)
        {
            return $parts[1]
        }
    }
    catch
    {
    }

    return "ubuntu_unknown"
}

Set-Location $env:APPVEYOR_BUILD_FOLDER

# Detected in common_defines.cmake; but Ubuntu 18.04+ only supports 64-bit apps so it's a constant
[String]$cmakeArch = "x64"
# Set in build.sh
[String]$cmakeBuildType = "MinSizeRel"

& 7z a libcgss.zip -r "bin/$cmakeArch/$cmakeBuildType/*"

[String]$osCodeName = Get-UbuntuCodeName
[String]$buildVersion = $Env:APPVEYOR_BUILD_VERSION
[String]$buildArch = "x64"
[String]$buildToolchain = "gcc"

Push-AppveyorArtifact "libcgss.zip" -FileName "libcgss-$osCodeName-$buildArch-$buildToolchain-$buildVersion.zip" -DeploymentName "Binaries"
Push-AppveyorArtifact "libcgss.zip" -FileName "libcgss-$osCodeName-$buildArch-$buildToolchain-latest.zip" -DeploymentName "CI auto update"
