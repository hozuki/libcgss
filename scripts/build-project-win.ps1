[Int32]$processorCount = [Environment]::ProcessorCount
& msbuild build\vc\cgss.sln /p:"Configuration=$env:CONFIGURATION" /m:"$processorCount"

if (!$?)
{
    Write-Error "An error occurred while trying to invoke ``msbuild``"
}
