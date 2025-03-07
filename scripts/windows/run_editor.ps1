# Exit on error
$ErrorActionPreference = "Stop"

Start-Process -NoNewWindow -FilePath "./build/bin/everest-editor/everest-editor"
