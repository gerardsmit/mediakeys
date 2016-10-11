{
	"targets": [
		{
			"target_name": "MMShellHook",
			"type": "shared_library",
			"sources": [
				"MMShellHook.cpp"
			],
			"msvs_settings": {
				"VCLinkerTool": {
					"EntryPointSymbol": 'DllMain',
					'AdditionalDependencies': [
						'libucrt.lib'
					]
				}
			}
		},
		{
			"target_name": "keylistener",
			"type": "executable",
			"sources": [
				"win.cpp"
			],
			"msvs_settings": {
				"VCLinkerTool": {
					"SubSystem": 2,
					"EntryPointSymbol": 'WinMainCRTStartup',
					'AdditionalDependencies': [
						'libucrt.lib'
					]
				}
			}
		}
	]
}