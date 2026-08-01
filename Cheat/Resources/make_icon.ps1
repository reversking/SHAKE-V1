Add-Type -AssemblyName System.Drawing

$root = Split-Path -Parent $PSScriptRoot
$header = Join-Path $root "..\Libraries\imgui\include\ImGui\TransparentLogo.hpp"
$png = Join-Path $PSScriptRoot "shake_logo.png"
$ico = Join-Path $PSScriptRoot "shake_icon.ico"

$content = Get-Content $header -Raw
$matches = [regex]::Matches($content, '0x([0-9a-fA-F]{2})')
$bytes = New-Object byte[] $matches.Count
for ($i = 0; $i -lt $matches.Count; $i++) {
    $bytes[$i] = [Convert]::ToByte($matches[$i].Groups[1].Value, 16)
}
[IO.File]::WriteAllBytes($png, $bytes)

$source = [System.Drawing.Bitmap]::FromFile($png)
$bmp256 = New-Object System.Drawing.Bitmap($source, 256, 256)
$iconHandle = $bmp256.GetHicon()
$icon = [System.Drawing.Icon]::FromHandle($iconHandle)
$stream = [IO.File]::Create($ico)
$icon.Save($stream)
$stream.Close()
$source.Dispose()
$bmp256.Dispose()

Write-Host "Created $ico from TransparentLogo"
