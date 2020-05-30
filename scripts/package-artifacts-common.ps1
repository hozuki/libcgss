function Copy-CommonTextFilesTo([String] $destDir)
{
    $sourceFiles = @{ }
    $sourceFiles.Add((Join-Path $PSScriptRoot "../README.md" -Resolve), "README-libcgss.md")
    $sourceFiles.Add((Join-Path $PSScriptRoot "../LICENSE.md" -Resolve), "LICENSE-libcgss.md")
    $sourceFiles.Add((Join-Path $PSScriptRoot, "../lib/vgaudio-cpp/README.md" -Resolve), "README-libvgaudio.md")
    $sourceFiles.Add((Join-Path $PSScriptRoot, "../lib/vgaudio-cpp/LICENSE.txt" -Resolve), "LICENSE-libvgaudio.txt")
    $sourceFiles.Add((Join-Path $PSScriptRoot, "../lib/vgaudio-cpp/ext/zlib/README" -Resolve), "README-zlib")

    foreach ($srcFile in $sourceFiles)
    {
        [String]$dstFile = Join-Path $destDir $sourceFiles[$srcFile]
        Copy-Item $srcFile $dstFile
    }
}
