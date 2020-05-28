& git submodule update --init --recursive

[String]$vgaudioCppProjectRoot = [System.IO.Path]::Combine($PSScriptRoot, "..", "lib", "vgaudio-cpp")
[String]$extScriptRoot = [System.IO.Path]::Combine($vgaudioCppProjectRoot, "scripts")
[String]$extScript = [System.IO.Path]::Combine($extScriptRoot, "ci-install.ps1")

Push-Location $vgaudioCppProjectRoot

. $extScript

Pop-Location
