param(
    [Parameter(Mandatory = $true)][String]$sourceDirectory,
    [Parameter(Mandatory = $true)][String]$classDirectory,
    [Parameter(Mandatory = $true)][String]$headerDirectory,
    [Parameter(Mandatory = $false)][String]$sourceVersion = "1.8",
    [Parameter(Mandatory = $false)][String]$targetVersion = "1.8"
)

function Gen-Headers([String]$sourceDirectory, [String]$classDirectory, [String]$headerDirectory)
{
    Write-Host "Generating headers to $headerDirectory"

    [String]$javaHome = $env:JAVA_HOME

    if (-not($javaHome))
    {
        Write-Error "Environment variable 'JAVA_HOME' is not found"
        exit 1
    }

    if (-not([System.IO.Directory]::Exists($javaHome)))
    {
        Write-Error "JAVA_HOME directory ($javaHome) does not exist"
        exit 1
    }

    [String]$javac = [System.IO.Path]::Combine($javaHome, "bin", "javac")
    Write-Host "Using javac: $javac"

    function Compile-SourceFile([String]$sourceFilePath, [String]$classDir, [String]$headerDir)
    {
        Write-Host "Compiling: $sourceFilePath"

        & "$javac" "$sourceFilePath" -d "$classDir" -h "$headerDir" -source "$sourceVersion" -target "$targetVersion" -classpath "$sourceDirectory"

        if (-not($?))
        {
            exit 1
        }
    }

    function Compile-Directory([String]$root, [String]$prefix)
    {
        [System.IO.FileInfo[]]$files = Get-ChildItem $root -Filter "*.java" -File

        if ($files.Length -gt 0)
        {
            foreach ($file in $files)
            {
                [System.IO.FileInfo]$f = $file

                [String]$classDir = [System.IO.Path]::Combine($classDirectory, $prefix)

                if (-not([System.IO.Directory]::Exists($classDir)))
                {
                    New-Item $classDir -Type Directory -ErrorAction SilentlyContinue > $null
                }

                Compile-SourceFile -sourceFilePath $f.FullName -classDir $classDir -headerDir $headerDirectory
            }
        }

        [System.IO.DirectoryInfo[]]$dirs = Get-ChildItem $root -Directory

        if ($dirs.Length -gt 0)
        {
            foreach ($dir in $dirs)
            {
                [System.IO.DirectoryInfo]$d = $dir
                [String]$newPrefix = [System.IO.Path]::Combine($prefix, $d.Name)

                Compile-Directory -root $d.FullName -prefix $newPrefix
            }
        }
    }

    Compile-Directory -root $sourceDirectory -prefix ""
}

Gen-Headers -sourceDirectory $sourceDirectory -classDirectory $classDirectory -headerDirectory $headerDirectory
Write-Host "Done."
