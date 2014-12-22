{
  "targets": [
    {
      "target_name": "lxc",
      "sources": [ "src/lxc.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src"
      ],
      "libraries": [ '-llxc' ]
    }
  ]
}
