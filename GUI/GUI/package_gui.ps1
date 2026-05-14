param(
    [string]$Configuration = "Release",
    [string]$QtRoot = "C:\Qt\6.10.2\mingw_64",
    [string]$BuildDir = "",
    [string]$DeployDir = ""
)

$ErrorActionPreference = "Stop"

$ProjectDir = $PSScriptRoot
$CMake = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$Ninja = "C:\Qt\Tools\Ninja\ninja.exe"
$MingwBin = "C:\Qt\Tools\mingw1310_64\bin"
$WinDeployQt = Join-Path $QtRoot "bin\windeployqt.exe"

if ([string]::IsNullOrWhiteSpace($BuildDir)) {
    $BuildDir = Join-Path $ProjectDir "build\package-$Configuration"
}

if ([string]::IsNullOrWhiteSpace($DeployDir)) {
    $DeployDir = Join-Path $ProjectDir "deploy\GUI-$Configuration"
}

foreach ($RequiredPath in @($CMake, $Ninja, $MingwBin, $QtRoot, $WinDeployQt)) {
    if (-not (Test-Path $RequiredPath)) {
        throw "Required path was not found: $RequiredPath"
    }
}

$env:Path = "$QtRoot\bin;$MingwBin;C:\Qt\Tools\Ninja;$env:Path"

& $CMake `
    -S $ProjectDir `
    -B $BuildDir `
    -G Ninja `
    "-DCMAKE_BUILD_TYPE=$Configuration" `
    "-DCMAKE_PREFIX_PATH=$QtRoot" `
    "-DCMAKE_MAKE_PROGRAM=$Ninja" `
    "-DCMAKE_C_COMPILER=$MingwBin\gcc.exe" `
    "-DCMAKE_CXX_COMPILER=$MingwBin\g++.exe"

& $CMake --build $BuildDir

$ExecutablePath = Join-Path $BuildDir "GUI.exe"

if (-not (Test-Path $ExecutablePath)) {
    throw "GUI.exe was not found after build: $ExecutablePath"
}

if (Test-Path $DeployDir) {
    Remove-Item -LiteralPath $DeployDir -Recurse -Force
}

New-Item -ItemType Directory -Path $DeployDir | Out-Null
Copy-Item -LiteralPath $ExecutablePath -Destination $DeployDir

$DeployedExecutable = Join-Path $DeployDir "GUI.exe"
$DeployMode = if ($Configuration -eq "Debug") { "--debug" } else { "--release" }

& $WinDeployQt $DeployMode --compiler-runtime $DeployedExecutable

Write-Host "GUI package created:"
Write-Host $DeployDir
