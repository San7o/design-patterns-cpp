# run-targets.ps1
# ===============
#
# Run all build targets from the `build` directory.
#

$path = "build"

$files = Get-ChildItem $path | Where { $_.Name -like '*.exe' }

foreach ($f in $files) {
    $name = $f.Name
    $filePath = $f.FullName
    Write-Output "Running $name"
    Write-Output "-------------------------"

    # Execute the file
    & $filePath

    Write-Output ""
}
