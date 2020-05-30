[String]$arch = [String]::Empty;

if ($Env:ARCH -eq "Win64")
{
    $arch = "Win64";
    Write-Host "Arch: $arch";
    # prepare for generator concatenation
    $arch = " $arch";
}
else
{
    Write-Host 'Arch: (default -> Win32)';
}

[String]$workerImage = $Env:APPVEYOR_BUILD_WORKER_IMAGE;

Write-Host "Build image: $workerImage";

[String]$generator = [String]::Empty;
[Boolean]$multiPlatform = $false;

if ($workerImage -eq 'Visual Studio 2019')
{
    $generator = 'Visual Studio 16 2019';
    $multiPlatform = $true
}
elseif ($workerImage -eq 'Visual Studio 2017')
{
    $generator = "Visual Studio 15 2017$arch";
    $multiPlatform = $false;
}
elseif ($workerImage -eq 'Visual Studio 2015')
{
    $generator = "Visual Studio 14 2015$arch";
    $multiPlatform = $false;
}
else
{
    Write-Error 'Error: Unsupported worker image.';
    $generator = "Visual Studio 15 2017$arch";
    exit 1;
}

Write-Host '------******------';

if (![System.IO.Directory]::Exists('build\vc'))
{
    mkdir build\vc
}

Set-Location build\vc

cmake --version

$cmakeDefs = @{
    "VGAUDIO_APPS_DYNAMIC_LINKING" = "ON";
    "VGAUDIO_NO_CMAKE_OUTPUT_DIRECTORY_OVERRIDE" = "ON";
    "VGAUDIO_DONT_GENERATE_TEST_TARGETS" = "ON";
}

[System.Collections.Generic.List[String]]$cmakeParams = New-Object System.Collections.Generic.List[String]

# Must specify types explicitly otherwise PowerShell can't find the correct overload
$cmakeParams.AddRange([System.Linq.Enumerable]::Select(
        [String[]]$cmakeDefs.Keys, [Func[String, String]]{
            param ([String]$key)
            [String]$value = $cmakeDefs[$key]
            return "-D$key=$value"
        })
)

if ($env:CI)
{
    if (-not [String]::IsNullOrWhiteSpace($env:LIBCGSS_LIBRARY_VERSION))
    {
        $cmakeParams.Add("-DLIBCGSS_LIBRARY_VERSION=`"$env:LIBCGSS_LIBRARY_VERSION`"")
    }
    else
    {
        Write-Error "Should not happen."
    }
}

if ($multiPlatform)
{
    [String]$cmakeGenArch = 'Win32';

    if ($Env:ARCH -eq 'Win64')
    {
        $cmakeGenArch = 'x64';
    }

    $cmakeParams.AddRange([String[]]@("-G", "`"$generator`"", "-A", $cmakeGenArch, "..\.."))
}
else
{
    $cmakeParams.AddRange([String[]]@("-G", "`"$generator`"", "..\.."))
}

$cmakeParamsLit = [String]::Join(' ', $cmakeParams)
Write-Host "CMake parameters: $cmakeParamsLit"

cmake $cmakeParams.ToArray()
