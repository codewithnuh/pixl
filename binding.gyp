{
  "targets": [{
    "target_name": "image_cli",  
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "vendor/stb"  
    ],
    "sources": [
      "src/main.cpp",
      
    ],
    "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
  }]
}