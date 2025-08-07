TrenchBroom profiles are saved to:

* `~/.TrenchBroom/games/Nightfire Open` on Linux.
* `C:\Users\YourUser\AppData\Roaming\TrenchBroom\games\Nightfire Open` on Windows.

# Example Profiles

Some example profiles for compiling and running maps are below. These profiles copy the compiled map to both the game content directory in the repo and the executable game directory, assumed to be located in `build/install`.

The paths are taken from Linux - change the path separators as appropriate on Windows. You can run a find and replace on all `/` characters to swap them to `\\`.

## CompilationProfiles.cfg

```json
{
	"profiles": [
		{
			"name": "Default",
			"tasks": [
				{
					"target": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.map",
					"type": "export"
				},
				{
					"parameters": "-chart -texturedir \"${GAME_DIR_PATH}/nfopen/textures\" \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.map\"",
					"tool": "${GAME_DIR_PATH}/tools/nfcsg",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"parameters": "-chart -viewportal \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
					"tool": "${GAME_DIR_PATH}/tools/nfbsp",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"parameters": "-chart \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
					"tool": "${GAME_DIR_PATH}/tools/nfvis",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"parameters": "-chart -texturedir \"${GAME_DIR_PATH}/nfopen/textures\" \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
					"tool": "${GAME_DIR_PATH}/tools/nfrad",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"source": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp",
					"target": "${GAME_DIR_PATH}/nfopen/maps",
					"type": "copy"
				},
				{
					"source": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp",
					"target": "${GAME_DIR_PATH}/../../../../nightfire-open-content/content/nfopen/maps",
					"type": "copy"
				}
			],
			"workdir": "${MAP_DIR_PATH}"
		},
		{
			"name": "Final",
			"tasks": [
				{
					"target": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.map",
					"type": "export"
				},
				{
					"parameters": "-chart -texturedir \"${GAME_DIR_PATH}/nfopen/textures\" \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.map\"",
					"tool": "${GAME_DIR_PATH}/tools/nfcsg",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"parameters": "-chart -viewportal \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
					"tool": "${GAME_DIR_PATH}/tools/nfbsp",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"parameters": "-full -chart \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
					"tool": "${GAME_DIR_PATH}/tools/nfvis",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"parameters": "-extra -bounce 16 -softsky 1 -chart -texturedir \"${GAME_DIR_PATH}/nfopen/textures\" \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
					"tool": "${GAME_DIR_PATH}/tools/nfrad",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"source": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp",
					"target": "${GAME_DIR_PATH}/nfopen/maps",
					"type": "copy"
				},
				{
					"source": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp",
					"target": "${GAME_DIR_PATH}/../../../../nightfire-open-content/content/nfopen/maps",
					"type": "copy"
				}
			],
			"workdir": "${MAP_DIR_PATH}"
		},
		{
			"name": "Only Entities",
			"tasks": [
				{
					"target": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.map",
					"type": "export"
				},
				{
					"parameters": "-chart -onlyents -texturedir \"${GAME_DIR_PATH}/nfopen/textures\" \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.map\"",
					"tool": "${GAME_DIR_PATH}/tools/nfcsg",
					"treatNonZeroResultCodeAsError": true,
					"type": "tool"
				},
				{
					"source": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp",
					"target": "${GAME_DIR_PATH}/nfopen/maps",
					"type": "copy"
				},
				{
					"source": "${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp",
					"target": "${GAME_DIR_PATH}/../../../../nightfire-open-content/content/nfopen/maps",
					"type": "copy"
				}
			],
			"workdir": "${MAP_DIR_PATH}"
		}
	],
	"version": 1
}
```

## GameEngineProfiles.cfg

Replace the `path` value with the path to the game executable or launch script on your system. This must (I think) be an absolute path.

```json
{
	"profiles": [
		{
			"name": "Default",
			"parameters": "-dev 5 -windowed -width 1920 -height 1080 +map ${MAP_BASE_NAME}",
			"path": "/path/to/bond"
		}
	],
	"version": 1
}

```
