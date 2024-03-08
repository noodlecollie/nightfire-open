# Compilation Profiles
TrenchBroom compilation profiles are saved to `.TrenchBroom/games/Nightfire Open/CompilationProfiles.cfg` on Linux. **TODO:** Confirm location on Windows - put a breakpoint on `common/src/Model/GameFactory.cpp:448` in `GameFactory::writeCompilationConfig()`.

# Example Profiles

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
					"target": "${GAME_DIR_PATH}/../../../game/content/nfopen/maps",
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
					"parameters": "-chart -viewporal \"${WORK_DIR_PATH}/compile/${MAP_BASE_NAME}.bsp\"",
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
					"target": "${GAME_DIR_PATH}/../../../game/content/nfopen/maps",
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
					"target": "${GAME_DIR_PATH}/../../../game/content/nfopen/maps",
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

```json
{
	"profiles": [
		{
			"name": "Default",
			"parameters": "-dev 5 -windowed -width 1920 -height 1080 +map ${MAP_BASE_NAME}",
			"path": "/path/to/bond.sh"
		}
	],
	"version": 1
}

```
