# LibraryRedirect
Like not having your Windows libraries on your system drive? Great. Use this so you don't have to move them manually.

# Usage
```
LibraryRedirect.exe [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  --documents PATH            Set Documents path to PATH
  --downloads PATH            Set Downloads path to PATH
  --desktop PATH              Set Desktop path to PATH
```

All options (except -h) can be used at once, but try not to set two different libraries to the same path. The system won't like that, and I'm too lazy to provide a way to reset it.

# Notes
This won't move (or remove) any existing files or subfolders from the libraries. I'm not touching your stuff. Move it yourself.

Uses the [IKnownFolderManager](https://msdn.microsoft.com/en-us/library/windows/desktop/bb761744(v=vs.85).aspx) interface. COM is fun, you should look into it.
