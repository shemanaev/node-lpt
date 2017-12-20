{
  "targets": [
    {
      "target_name": "lpt",
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "sources": [
        "src/binding.cc",
        "src/port.cc",
        "src/port-status.cc",
        "src/port-control.cc"
      ]
    }
  ]
}
