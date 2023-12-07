#pragma once

// Default object names
constexpr auto GRID_OBJ = "grid";
constexpr auto BG_PLANE_OBJ = "bgPlane";
constexpr auto TRANSFORM_HANDLE = "transformHandle";
constexpr auto CAMERA_AXIS_HANDLE = "cameraAxisHandle";

// Resource directories
constexpr auto TEXTURE_DIR = "resources/textures/";
constexpr auto FONT_DIR = "resources/fonts/";
constexpr auto MODELS_DIR = "resources/models/";
constexpr auto SHADER_DIR = "resources/shaders/";
constexpr auto TOOL_ICON_DIR = "resources/icons/toolIcons";
constexpr auto MODE_ICON_DIR = "resources/icons/modeIcons";

// Config locations
constexpr auto CONFIG_JSON = "configs/config.json";
constexpr auto HOTKEYS_JSON = "configs/hotkeys.json";
constexpr auto MATS_JSON = "configs/materials.json";
constexpr auto SHADERS_JSON = "configs/shaders.json";

// Icons
constexpr auto APP_NAME = "BlankMat";
constexpr auto ICON = "icon256.png";

constexpr auto SELECT_ICON = "Select.png";
constexpr auto MOVE_ICON = "Move.png";
constexpr auto ROTATE_ICON = "Rotate.png";
constexpr auto SCALE_ICON = "Scale.png";
constexpr auto EXTRUDE_ICON = "Extrude.png";

// Supported file formats
constexpr auto BLANK = ".blank";
constexpr auto OBJ = ".obj";
constexpr auto DAE = ".dae";
constexpr auto STL = ".stl";
constexpr auto PLY = ".ply";
constexpr auto FBX = ".fbx";
constexpr auto BLEND = ".blend";
constexpr auto GLTF = ".gltf";
constexpr auto MAX3DS = ".3ds";