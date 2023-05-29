Package: gsoap[core]:x86-windows -> 2.8.112#1

**Host Environment**

- Host: x64-windows
- Compiler: MSVC 19.36.32532.0
-    vcpkg-tool version: 2023-04-07-bedcba5172f5e4b91caac660ab7afe92c27a9895
    vcpkg-scripts version: 1c5a340f6 2023-05-28 (9 hours ago)

**To Reproduce**

`vcpkg install `

**Failure logs**

```
CMake Error at ports/gsoap/portfile.cmake:1 (message):
  gsoap does not offer permanent public downloads of its sources; all
  versions except the latest are removed from sourceforge.  Therefore, vcpkg
  cannot support this library directly in the central catalog.  If you would
  like to use gsoap, you can use this port as a starting point
  (C:/vcpkg/ports/gsoap) and update it to use a permanent commercial copy or
  the latest public download.  Do not report issues with this library to the
  vcpkg GitHub.
Call Stack (most recent call first):
  scripts/ports.cmake:147 (include)



```


**Additional context**

<details><summary>vcpkg.json</summary>

```
{
  "name": "my-project",
  "version-string": "0.0.1",
  "dependencies": [
    "gsoap"
  ]
}

```
</details>
