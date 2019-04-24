[String]$arch = [String]::Empty;

if ($Env:ARCH -eq "Win64") {
    $arch = "Win64";
    Write-Output "Arch: $arch";
    # prepare for generator concatenation
    $arch = " $arch";
} else {
    Write-Output 'Arch: (default -> Win32)';
}

[String]$workerImage = $Env:APPVEYOR_BUILD_WORKER_IMAGE;

Write-Output "Build image: $workerImage";

[String]$generator = [String]::Empty;

if ($workerImage -eq 'Visual Studio 2017') {
    $generator = "Visual Studio 15 2017$arch";
} elseif ($workerImage -eq 'Visual Studio 2015') {
    $generator = "Visual Studio 14 2015$arch";
} else {
    Write-Output 'Error: Unsupported worker image.';
    $generator = "Visual Studio 15 2017$arch";
    exit 1;
}

Write-Output '------******------';

if (![System.IO.Directory]::Exists('build\vc')) {
    mkdir build\vc
}

Set-Location build\vc

cmake --version

[String]$cmakeGenCmd = "cmake ..\.. -G `"$generator`""

[ScriptBlock]$script = [ScriptBlock]::Create($cmakeGenCmd);

Invoke-Command -ScriptBlock $script
