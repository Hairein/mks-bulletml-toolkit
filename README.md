# mks-bulletml-toolkit
MKS BulletML Toolkit - A collection of tools to visualize and work with specific BulletML XML files. Initially, this library parses output from the [MKS BulletML Pattern Builder](https://store.steampowered.com/app/3950780/MKS_BulletML_Pattern_Builder/) but the intention here is to make it a generic BulletML XML file parser.

The entire project is provided under the MIT license except foreign files and libraries used within which retain their respective licensing.

## Interpreter 
The interpreter library loads and parses the BulletML XML files provided and enables them to be played back. After each updated frame, the virtual bullets can be read back and tracked with unique handles per bullet.
The library using application is responsible for providing information when specific bullets can be deleted within the library, e.g. when bullets leave specific bounds. Otherwise, memory usage can accumulate.

## Player
A small application to show how the library is used and those functions can be called.
It visualizes the bullets interpreted within the library using Raylib/RaylibGui.
To use this sample app, provide BulletML XML files within the folder 'bulletml_files', all will be loaded automatically on startup. 
The 'asset' folder contains all resources required to run the player application.

---

## API Functions

| Function Signature                                                                 | Description            |
|------------------------------------------------------------------------------------|------------------------|
| `int mksbmli_init();`                                                              | Fist call before using the library                      |
| `int mksbmli_shutdown();`                                                          | Last call during shutdown of the application to free the resources used                     |
| `void mksbmli_get_version(int* major, int* minor, int* patch);`                    | Print out the library version                       |
| `int mksbmli_set_random_seed(int seed);`                                           | Set the same seed for repeated results or random each run                       |
| `int mksbmli_load_xml(const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE* handle);` | Each XML file gets it's own run and can be frame-rate controlled                        |
| `int mksbmli_unload_xml(MKSBMLI_PLAYBACK_HANDLE handle);`                          | Remove the XML file if not needed and that slot can be reused with new file, limit is 100 concurrently at this time                       |
| `int mksbmli_count_xmls();`                                                        | Shows how many slots of 1000 max. are used in the library                       |
| `void mksbmli_start_playback(MKSBMLI_PLAYBACK_HANDLE handle);`                     | Start the BulletML sequence with the first Action                       |
| `void mksbmli_stop_playback(MKSBMLI_PLAYBACK_HANDLE handle);`                      | Stop the specific playback frames, loses the frame number reached in playback previously                       |
| `void mksbmli_next_frame(MKSBMLI_PLAYBACK_HANDLE handle);`                         | Moves playback of the specific Bulletml XML file to the next iteration/frame. playback restarts continously at the end                       |
| `int mksbmli_get_bullets(MKSBMLI_PLAYBACK_HANDLE handle, int max_bullets, VirtualBullet** bullets, int* nos_bullets);` | After each next frame call, get the new positions of the bullets as they were handled. Provide an array of max_bullets, returned nos will be provided               |
| `int mksbmli_delete_bullets(MKSBMLI_PLAYBACK_HANDLE handle, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles);` | Any bullets that leave the user's app bounds or are not needed anymore for other reasons can be deleted         |
| `int mksbmli_clear_bullets(MKSBMLI_PLAYBACK_HANDLE handle);`                       | Clears all currently active and tracked bullets of the XML file playback, playback doesn't reset to start                       |
| `int mksbmli_set_emitter_center(MKSBMLI_PLAYBACK_HANDLE handle, float emitter_position_x, float emitter_position_y);` | The coordinate from which new bullets without parent bullets should be emitted, defaults to (0.0/0.0)              |
| `int mksbmli_set_player_position(MKSBMLI_PLAYBACK_HANDLE handle, float player_position_x, float player_position_y);` | For using aimed shots in the BulletML XML file, you need to provide the player virtual positon to be aimed at              |
| `int mksbmli_set_rank(MKSBMLI_PLAYBACK_HANDLE handle, float rank);` | The rank value [0.0f..1.0f] for expression evaluation              |

---

## Player Controls
```
Keys Q,W,E,R - Stop, Play, Play frame, Pause playback
C - Hold With click of LMB to set emitter center
X - To reset emitter center to (zero/zero), i.e. to the default middle of the playfield
```
---

## NOTE

The library performs the outer looping of a BulletML file structure playback. Avoid using wrapping <repeat> loops with large repeat times count, instead
let the playback functionality of the library play the frame continuously and control playback with start and stop API calls through the library.

---

## DEV

### Build

The project is configured with CMAKE and built using Qt Creator on various Ubuntu 24.04 LTS and Windows 11 platforms.

### Install dependencies

#### Windows

Under Windows, you need to have environment variables set for these paths or you can provide hard-coded paths in the Player CMakeLists.txt, see that file for examples
```
Raylib Header and Libs/DLLs - RAYLIB_INCLUDE_DIR, RAYLIB_LIB_DIR

LibXML2 Root Folder with include/lib - LIBXML2_ROOT through 

(Optional) IConv ICONV_ROOT with include/lib - Only if your libXML2 build includes IConv
```

#### Linux

Under Linux the following dependencies exist which can be installed, for example using Ubuntu/APT
```
sudo apt update
sudo apt install libraylib-dev
sudo apt install libxml2-dev
```

---

## NOTES

These projects were created with the help of AI.

### Check mks-bulletml-interpreter exports

#### Linux
`nm -D libmks-bulletml-interpreter.so | grep init`

### Contact

If you have any questions or want to get in touch about anything related to this project.

Website:  
[Micah Koleoso Software](https://micahkoleososoftware.com)


Email:  
[micah@micahkoleososoftware.com](mailto:micah@micahkoleososoftware.com)


BulletML Pattern Editor (Steam), creates BulletML patterns:  
[MKS BulletML Pattern Builder](https://store.steampowered.com/app/3950780/MKS_BulletML_Pattern_Builder/)
