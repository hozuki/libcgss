[string]$arch = $Env:ARCH
[String]$workerImage = $Env:APPVEYOR_BUILD_WORKER_IMAGE;

[String]$buildArch = [String]::Empty;
[String]$vsVersion = [String]::Empty;

switch ($arch) {
    'Win64' {
        $buildArch = 'x64';
    }
    'Win32' {
        $buildArch = 'x86';
    }
    default {
        Write-Output "Unsupported arch: $arch";
        exit 1;
    }
}

switch ($workerImage) {
    'Visual Studio 2017' {
        $vsVersion = 'vc19.1';
    }
    'Visual Studio 2015' {
        $vsVersion = 'vc19';
    }
    default {
        Write-Output "Unsupported worker: $workerImage";
        exit 1;
    }
}

$configuration = $Env:CONFIGURATION;

function Invoke-CommandLine([String]$Command) {
    [ScriptBlock]$script = [ScriptBlock]::Create($Command);
    Invoke-Command -ScriptBlock $script
}

$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.exe";
Invoke-CommandLine -Command $7zcmd
$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.dll";
Invoke-CommandLine -Command $7zcmd

[String]$buildVersion = $Env:APPVEYOR_BUILD_VERSION;

[String]$dest = "libcgss-bin-$buildArch-$vsVersion-$buildVersion.zip";
Copy-Item -Path 'libcgss.zip' -Destination $dest
$dest = "libcgss-ci-$buildArch-$vsVersion-latest.zip";
Copy-Item -Path 'libcgss.zip' -Destination $dest
