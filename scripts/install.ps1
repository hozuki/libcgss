& git submodule update --init --recursive

[String]$extScript = [Path]::Combine($PSScriptRoot, "..", "lib", "vgaudio-cpp", "scripts", "ci-install.ps1")

. $extScript
