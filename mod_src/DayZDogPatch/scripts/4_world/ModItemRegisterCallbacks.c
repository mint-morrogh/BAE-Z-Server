modded class ModItemRegisterCallbacks
{
    override void RegisterHeavy(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
        super.RegisterHeavy(pType, pBehavior);
        /*
		pType.AddItemInHandsProfileIK("dog_shed_wooden_kit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,						"dz/anims/anm/player/ik/heavy/tent_large.anm");
		pType.AddItemInHandsProfileIK("dog_shed_big_kit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,						"dz/anims/anm/player/ik/heavy/tent_large.anm");
		pType.AddItemInHandsProfileIK("dog_shed_small_kit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior,						"dz/anims/anm/player/ik/two_handed/BatteryCar.anm");
		*/
		pType.AddItemInHandsProfileIK("dog_shed_wooden_kit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,						"dayz_dog/anims/player/dog_shed_big_kitIK.anm");
		pType.AddItemInHandsProfileIK("dog_shed_big_kit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,						"dayz_dog/anims/player/dog_shed_big_kitIK.anm");
		pType.AddItemInHandsProfileIK("dog_shed_small_kit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior,						"dayz_dog/anims/player/dog_shed_small_kitIK.anm");
		
    }
	
	override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
		super.RegisterTwoHanded(pType,  pBehavior);

		pType.AddItemInHandsProfileIK("DogPainting_Base", "dz/anims/workspaces/player/player_main/player_main_2h.asi",	pBehavior, "dz/anims/anm/player/ik/two_handed/tent_medium_packed.anm");
	}
};