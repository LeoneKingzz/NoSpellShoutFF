registerPatcher({
	info: info,
	gameModes: [xelib.gmTES5, xelib.gmSSE],
	settings: {
		label: 'No Spell Shout Friendly Fire',
		hide: false,
		templateUrl: `${patcherUrl}/partials/settings.html`,
		defaultSettings: {
			patchFileName: 'No Spell Shout FF.esp',
		}
	},
	requiredFiles: [],
	getFilesToPatch: function(filenames) {
		return filenames;
	},
	execute: (patchFile) =>({
		initialize: function() {	
			
			let fileHandle = patchFile;
			let glob = xelib.AddElement(fileHandle, 'GLOB\\GLOB');
			xelib.AddElementValue(glob, 'EDID - Editor ID', 'NSSFFLK_Enable');
			xelib.SetValue(glob, 'FNAM - Type', '102');
			xelib.SetValue(glob, 'FLTV - Value', '1.0');
		},
		process: [{
			load: function() {
				return {
					signature: 'MGEF',
					filter: function(record) {
						let hasHostileflag = xelib.HasElement(record, 'Magic Effect Data') && xelib.GetFlag(record, 'Magic Effect Data\\DATA\\Flags', 'Hostile');
						let Archy_X = xelib.HasElement(record, 'Magic Effect Data') && xelib.GetValue(record, 'Magic Effect Data\\DATA\\Archtype');
						let Kw_magicfire = xelib.HasElement(record, 'Magic Effect Data') && xelib.HasKeyword(record, 'MagicDamageFire');
						let Kw_magicfrost = xelib.HasElement(record, 'Magic Effect Data') && xelib.HasKeyword(record, 'MagicDamageFrost');
						let Kw_magicshock = xelib.HasElement(record, 'Magic Effect Data') && xelib.HasKeyword(record, 'MagicDamageShock');
						let Kw_magicshout = xelib.HasElement(record, 'Magic Effect Data') && xelib.HasKeyword(record, 'MagicShout');
						let Kw_Exclude = xelib.HasElement(record, 'Magic Effect Data') && (xelib.HasKeyword(record, 'MagicTrapGas') || xelib.HasKeyword(record, 'MagicTrapPoison') || xelib.HasKeyword(record, 'MagicVoiceChangeWeather'));
						let Kw_StormStagger = xelib.HasElement(record, 'Magic Effect Data') && (xelib.HasKeyword(record, 'MagicVoiceChangeWeather'))
						return (Kw_StormStagger && Archy_X == 'Stagger') || (!Kw_Exclude && (hasHostileflag || Kw_magicfire || Kw_magicfrost || Kw_magicshock || Kw_magicshout) && (Archy_X == 'Dual Value Modifier' || Archy_X == 'Value Modifier' || Archy_X == 'Peak Value Modifier' || Archy_X == 'Paralysis' || Archy_X == 'Demoralize' || Archy_X == 'Frenzy' || Archy_X == 'Disarm' || Archy_X == 'Absorb' || Archy_X == 'Stagger'))
					}
				}
			},
			patch: function(record) {
				let editorID = xelib.GetValue(record, 'EDID');
				if (!(editorID.includes('Trap') || editorID.includes('_NSSFFLK_exclude'))){
					if (editorID.includes('Contact') || editorID.includes('Hazard') || editorID.includes('_NSSFFLK_include') || editorID.includes('ShockDamageBoltStormAimed') || editorID.includes('FT_VoiceStormCallBoltEffect')){
						xelib.AddCondition(record, 'IsHostileToActor', '10000101', 'NSSFFLK_Enable', '');
					}else{
                        xelib.AddCondition(record, 'IsHostileToActor', '10000100', 'NSSFFLK_Enable', '');
					}
				}
				
			}
		}]
	})
});
