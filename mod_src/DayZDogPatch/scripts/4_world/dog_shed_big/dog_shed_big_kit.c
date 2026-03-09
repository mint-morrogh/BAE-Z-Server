class dog_shed_big_kit extends ItemBase
{
	ref protected EffectSound 						m_DeployLoopSound;
	
	void dog_shed_big_kit()
	{
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
	}
	
	override void EEInit()
	{
		super.EEInit();
	}

	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 180 0" )
    {
        super.OnPlacementComplete(player, position, orientation);
        if ( GetGame().IsServer() )
        {
            dog_shed_big dogshed = dog_shed_big.Cast( GetGame().CreateObjectEx( "dog_shed_big", position, ECE_PLACE_ON_SURFACE ) );
            dogshed.SetPosition( position );
            dogshed.SetOrientation( orientation );
			
			this.Delete();
        }
        
        SetIsDeploySound( true );
    }
	
	override bool IsBasebuildingKit()
    {
        return true;
    }
	
	override bool IsDeployable()
	{
		return true;
	}	
	
	override string GetDeploySoundset()
	{
		return "placeHescoBox_SoundSet";
	}
	
	override string GetLoopDeploySoundset()
	{
		return "hescobox_deploy_SoundSet";
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionDeployObject);
	}
}