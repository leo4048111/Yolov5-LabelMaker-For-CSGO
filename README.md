# Yolov5LabelMakerForCSGO
This is a toy dll which allows you to make CSGO dataset for YOLOv5 network with some simple clicks of button.

### Features

+ **Visible bounding box**
+ **Simultaneous generation of image and label**

### Pictures

![image-20220609113325392](https://github.com/leo4048111/Yolov5LabelMakerForCSGO/blob/main/Pictures/image-20220609113325392.png)

​										           *This is what your game will look after injecting this dll(Non-visible entities will not have bbox*

![image-20220609113513957](https://github.com/leo4048111/Yolov5LabelMakerForCSGO/blob/main/Pictures/image-20220609113513956.png)

​                                                  *This is what your screenshot will look like(It won't have bounding box)*

![image-20220609113623803](https://github.com/leo4048111/Yolov5LabelMakerForCSGO/blob/main/Pictures/image-20220609113623803.png)

​												*This is what the corresponding label file will look like(YOLOv5 format,  Change id for each class in source code LabelMaker.h)*

### How to use?

+ **Open .sln with Visual studio 2019+**
+ **Open `Config.h` and change directory configs**
+ **Build the project source into .dll **
+ **Launch your csgo game** *(notice: This dll doesn't have any sort of VAC bypass so u might wanna start the game in `-insecure` mode to avoid VAC/Untrusted ban)*
+ **Start a match(Prefarably a bot match)**

+ **Inject this .dll with your favorite injector**

+ | Keybinds  |                                                              |
  | --------- | ------------------------------------------------------------ |
  | `HOME`    | Takes a screenshot and generates corresponding label in YOLOv5 format(.txt) |
  | `NUMPAD8` | Enlarges the bounding box height                             |
  | `NUMPAD2` | Reduces the bouding box height                               |
  | `INSERT`  | Unloads the dll                                              |

### Notice

+ Non-existing directories won't be created for you, so you have to manually create them before files' being saved.

+ This dll crashes the game sometimes when selecting/changing team side.

+ You need to rebuild this dll after changing configs, so a GUI menu might be added for runtime configuration later.
+ If the dll doesn't work after csgo updates, you need to manually update the offsets in  `csgo.hpp` from https://github.com/frk1/hazedumper

### Credits

+ https://github.com/TsudaKageyu/minhook 
+ https://github.com/frk1/hazedumper   For netvar offsets.

+ https://github.com/danielkrupinski/Osiris  Borrowed some features from SDK.
