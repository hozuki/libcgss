[Int32]$processorCount = [Environment]::ProcessorCount
& msbuild build\vc\cgss.sln /p:"Configuration=$env:CONFIGURATION" /m:"$processorCount"

if (!$?)
{
    Write-Error "An error occurred while trying to invoke ``msbuild``"
}

# Copy zlib dll from vgaudio-cpp and include it in the release
{
    $zlibDll = Join-Path $PSScriptRoot "../lib/vgaudio-cpp/ext/zlib/build/bin/zlib1.dll" -Resolve

    [String]$arch = $Env:ARCH
    [String]$buildArch = [String]::Empty
    [String]$configuration = $Env:CONFIGURATION

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

    Copy-Item $zlibDll (Join-Path $PSScriptRoot "../bin/$buildArch/$configuration/" -Resolve)
}
