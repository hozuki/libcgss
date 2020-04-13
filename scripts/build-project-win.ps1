[Int32]$processorCount = [Environment]::ProcessorCount;
& msbuild build\vc\cgss.sln /p:Configuration="$env:CONFIGURATION" -maxcpucount:"$processorCount"
