/**
 * InventoryMoveSoundsFix - BAE-Z
 *
 * Inventory Move Sounds (Workshop 2444247391) sets
 *   ItemMoveSounds[] = {"MagRifle_empty_in_SoundSet"}
 * on Magazine_Base and Grenade_Base, and plays it from ItemBase.EEItemLocationChanged
 * on the client. DayZ 1.29 removed that vanilla sound set, so every magazine / ammo /
 * grenade move logs "EffectSound ... Invalid sound set".
 *
 * A config override cannot fix this reliably: both IMS PBOs declare the same CfgPatches
 * name (IMS_Sounds), so requiredAddons cannot order us after the movesounds config and
 * its value wins. Swapping the name at the script level is deterministic instead:
 * SEffectManager.CreateSound() -> EffectSound.SetSoundSet() is the only entry point.
 */
modded class EffectSound
{
	override void SetSoundSet(string snd)
	{
		if (snd == "MagRifle_empty_in_SoundSet")
			snd = "MagRifle_fill_out_SoundSet";

		super.SetSoundSet(snd);
	}
}
