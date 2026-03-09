class dog_shed_wooden_kit extends ItemBase
{
	ref protected EffectSound 						m_DeployLoopSound;
	
	void dog_shed_wooden_kit()
	{
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
	}
	
	override void EEInit()
	{
		super.EEInit();
	}

	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
    {
        super.OnPlacementComplete(player, position, orientation);
        if ( GetGame().IsServer() )
        {
            dog_shed_wooden wooddogshed = dog_shed_wooden.Cast( GetGame().CreateObjectEx( "dog_shed_wooden", position, ECE_PLACE_ON_SURFACE ) );
            wooddogshed.SetPosition( position );
            wooddogshed.SetOrientation( orientation );
			
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