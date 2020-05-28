& git submodule update --init --recursive

[String]$extScript = [System.IO.Path]::Combine($PSScriptRoot, "..", "lib", "vgaudio-cpp", "scripts", "ci-install.ps1")

. $extScript
