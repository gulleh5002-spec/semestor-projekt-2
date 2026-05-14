param(
    [string]$Configuration = "Release",
    [string]$QtRoot = "C:\Qt\6.10.2\mingw_64",
    [string]$BuildDir = "",
    [string]$DeployDir = "",
    [string]$RobotArmDir = ""
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
    if ([string]::IsNullOrWhiteSpace($RobotArmDir)) {
        $DeployDir = Join-Path $ProjectDir "deploy\GUI-$Configuration"
    } else {
        $DeployDir = Join-Path $ProjectDir "deploy\RobotBuild-$Configuration"
    }
}

foreach ($RequiredPath in @($CMake, $Ninja, $MingwBin, $QtRoot, $WinDeployQt)) {
    if (-not (Test-Path $RequiredPath)) {
        throw "Required path was not found: $RequiredPath"
    }
}

$RobotArmExecutable = ""

if (-not [string]::IsNullOrWhiteSpace($RobotArmDir)) {
    if (-not (Test-Path $RobotArmDir)) {
        throw "RobotArm directory was not found: $RobotArmDir"
    }

    $RobotArmExecutable = Join-Path $RobotArmDir "RobotArm.exe"

    if (-not (Test-Path $RobotArmExecutable)) {
        throw "RobotArm.exe was not found in: $RobotArmDir"
    }
}

$DeployDir = [System.IO.Path]::GetFullPath($DeployDir)

if (-not [string]::IsNullOrWhiteSpace($RobotArmDir)) {
    $RobotArmDir = [System.IO.Path]::GetFullPath($RobotArmDir)

    if ($DeployDir.TrimEnd('\') -ieq $RobotArmDir.TrimEnd('\')) {
        throw "DeployDir must not be the same as RobotArmDir."
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

if (-not [string]::IsNullOrWhiteSpace($RobotArmDir)) {
    Get-ChildItem -LiteralPath $RobotArmDir | Copy-Item -Destination $DeployDir -Recurse -Force
}

Copy-Item -LiteralPath $ExecutablePath -Destination $DeployDir

$DeployedExecutable = Join-Path $DeployDir "GUI.exe"
$DeployMode = if ($Configuration -eq "Debug") { "--debug" } else { "--release" }

& $WinDeployQt $DeployMode --compiler-runtime $DeployedExecutable

Write-Host "GUI package created:"
Write-Host $DeployDir

if (-not [string]::IsNullOrWhiteSpace($RobotArmDir)) {
    Write-Host "RobotArm.exe copied into package. GUI will save build_plan.json next to RobotArm.exe."
}
