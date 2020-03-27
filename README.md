# UnityCustom3rdPartyToolsLocations
Plugin to customize installation paths for 3rd party tools like blender

## Why?

Unity has the option to directly import models from 3rd party tools like Blender. This allows you to place your *.blend files directly into the Assets folder and Unity will happily import those as models *as long as you have Blender installed*. This is a really nice feature but you have to manage your Blender installation separately to your Unity installation.

This becomes a real hassle when working on multiple projects with different Blender requirements, like requiring a specific version or even a customized version.

So this plugin is built to allow you to customize the paths to your 3rd party tools like Blender for specific Unity projects. Any Unity project can now reference their very own Blender installation.

*(This project is really heavily influenced by the specifics of Blender. It currently has no support for other tools but that can be easily added)*

## How?

In order to customize the location of blender.exe for a specific project:

- Download the `UnityCustom3rdPartyToolsLocationsPlugin.dll` native plugin DLL for your platform
- Put the Plugin DLL into your Unity project. Mark it as editor-only and "LoadOnStartup"
- Close your Unity editor
- Create a text file `blender-config.txt` into your unity project folder (next to the Assets, Library, ProjectSettings, .. folders)
- Open the text file with a text editor and put the path to your blender.exe into it. It can be relative to the project directory, like `..\blender-2.79b-windows64\blender.exe` - save it
- Open your Unity project
- Add `.blend` files to it!

## Technicalities

In order to provide Unity with the path to the custom executable, the plugin hooks into the `FindExecutableW()` function. Unity uses this to find the program responsible for opening specific files, like .blend files. By hijacking the invocation the plugin can just return a custom executable locations when it sees fit, like the custom blender executable path when Unity wants to open a .blend file.

## How to build

- Clone the repo
- Open `UnityCustom3rdPartyToolsLocations.sln` 
- Build the project for x64 Release
- Copy the plugin DLL from `UnityCustom3rdPartyToolsLocations\Release\UnityCustom3rdPartyToolsLocationsPlugin.dll` into your Unity project

## License

This project is licensed under the [MIT](LICENSE) license.

## Acknowledgements

This project uses the <https://github.com/microsoft/Detours> library for hooking API methods.