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
        echo "Unsupported arch: $arch";
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
        echo "Unsupported worker: $workerImage";
        exit 1;
    }
}

$configuration = $Env:CONFIGURATION;

function Run-Cmd([String]$Command) {
    [ScriptBlock]$script = [ScriptBlock]::Create($Command);
    Invoke-Command -ScriptBlock $script
}

$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.exe";
Run-Cmd -Command $7zcmd
$7zcmd = "7z a libcgss.zip -r bin\$buildArch\$configuration\*.dll";
Run-Cmd -Command $7zcmd

[String]$buildVersion = $Env:APPVEYOR_BUILD_VERSION;

[String]$dest = "libcgss-$buildArch-$vsVersion-$buildVersion.zip";
Copy-Item -Path 'libcgss.zip' -Destination $dest
$dest = "libcgss-$buildArch-$vsVersion-latest.zip";
Copy-Item -Path 'libcgss.zip' -Destination $dest
