param([string]$CompilerDirectory = "")
$ErrorActionPreference = "Stop"
if ($CompilerDirectory) {
    $resolvedCompilerDirectory = (Resolve-Path -LiteralPath $CompilerDirectory).Path
    $env:PATH = $resolvedCompilerDirectory + [IO.Path]::PathSeparator + $env:PATH
}
$compiler = (Get-Command g++ -CommandType Application -ErrorAction Stop).Source
Push-Location -LiteralPath $PSScriptRoot
try {
    & $compiler '-std=c++14' '-O2' '-Wall' '-Wextra' '-pedantic' 'main.cpp' '-o' 'lab03.exe'
    if ($LASTEXITCODE -ne 0) { throw 'g++ failed' }
    Write-Host 'Build completed: lab03.exe'
} finally {
    Pop-Location
}
