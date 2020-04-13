Set-Location $Env:APPVEYOR_BUILD_FOLDER

[string]$arch = $Env:ARCH
[String]$workerImage = $Env:APPVEYOR_BUILD_WORKER_IMAGE;

[String]$buildArch = [String]::Empty;
[String]$vsVersion = [String]::Empty;

switch ($arch)
{
    'Win64' {
        $buildArch = 'x64';
    }
    'Win32' {
        $buildArch = 'x86';
    }
    default {
        Write-Error "Unsupported arch: $arch";
        exit 1;
    }
}

switch ($workerImage)
{
    'Visual Studio 2019' {
        $vsVersion = 'vc14.2';
    }
    'Visual Studio 2017' {
        $vsVersion = 'vc14.1';
    }
    'Visual Studio 2015' {
        $vsVersion = 'vc14';
    }
    default {
        Write-Error "Unsupported worker: $workerImage";
        exit 1;
    }
}

[String]$configuration = $Env:CONFIGURATION;

function Invoke-CommandLine([String]$Command)
{
    [ScriptBlock]$script = [ScriptBlock]::Create($Command);
    Invoke-Command -ScriptBlock $script
}

$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.exe";
Invoke-CommandLine -Command $7zcmd
$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.dll";
Invoke-CommandLine -Command $7zcmd
$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.lib";
Invoke-CommandLine -Command $7zcmd

[String]$buildVersion = $Env:APPVEYOR_BUILD_VERSION;

Push-AppveyorArtifact "libcgss.zip" -FileName "libcgss-bin-$buildArch-$vsVersion-$buildVersion.zip" -DeploymentName "Binaries"
Push-AppveyorArtifact "libcgss.zip" -FileName "libcgss-ci-$buildArch-$vsVersion-latest.zip" -DeploymentName "CI auto update"
