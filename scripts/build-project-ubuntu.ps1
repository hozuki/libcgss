Push-Location build/make
[Int32]$processorCount = [Environment]::ProcessorCount

& make -j "$processorCount"

if (!$?) {
    Write-Error "An error occured while trying to invoke ``make``"
}

Pop-Location
