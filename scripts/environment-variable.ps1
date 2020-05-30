function Set-PersistentEnvironmentVariable([String] $name, [String]$value)
{
    if ($IsWindows)
    {
        [Environment]::SetEnvironmentVariable($name, $value, [EnvironmentVariableTarget]::User)
    }
    else
    {
        # There is still no way to persist *nix environment variables in PowerShell (PowerShell/PowerShell#4341).
        # But this function should run in root scope and all CMake processes are its child so we should be OK.
        Set-Item -Path "env:$name" -Value $value
    }
}
