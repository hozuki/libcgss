Set-Location build/make

& make

if (!$?) {
    Write-Error "An error occured while trying to invoke ``make``"
}

Set-Location ../..
