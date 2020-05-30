if (-not(Get-Command cmake))
{
    Write-Error "CMake is not found."
    return 1;
}

cmake --version

[String]$cmakeVersionFound = cmake --version | grep -Eo "cmake version ([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+)" | cut -d' ' -f3 | Out-String
[String]$cmakeVersionMinimum = '3.2'

function Compare-Versions([Parameter(Mandatory = $true)][String]$v1, [Parameter(Mandatory = $true)][String]$v2)
{
    [String[]]$p1 = $v1.Split('.')
    [String[]]$p2 = $v2.Split('.')
    [Int32]$len1 = $p1.Length
    [Int32]$len2 = $p2.Length
    [Int32]$i = 0

    while ($true)
    {
        if ($i -ge $len1)
        {
            if ($i -ge $len2)
            {
                return 0
            }
            else
            {
                return -1
            }
        }
        else
        {
            if ($i -ge $len2)
            {
                return 1
            }
        }

        [Int32]$n1 = 0
        [Int32]$n2 = 0

        [Int32]::TryParse($p1[$i], [ref]$n1)
        [Int32]::TryParse($p2[$i], [ref]$n2)

        [Int32]$compared = $n1.CompareTo($n2)

        if ($compared -ne 0)
        {
            return $compared
        }

        $i += 1
    }
}

if ((Compare-Versions -v1 $cmakeVersionFound -v2 $cmakeVersionMinimum) -lt 0)
{
    Write-Error "Requires CMake >= $cmakeVersionMinimum but found $cmakeVersionFound"
    return 1;
}

New-Item -ItemType Directory -Force -Path build/make

Push-Location build/make

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

$cmakeParams.AddRange([String[]]@("-DCMAKE_BUILD_TYPE=MinSizeRel", "../.."))

$cmakeParamsLit = [String]::Join(' ', $cmakeParams)
Write-Host "CMake parameters: $cmakeParamsLit"

cmake $cmakeParams.ToArray()

Pop-Location
