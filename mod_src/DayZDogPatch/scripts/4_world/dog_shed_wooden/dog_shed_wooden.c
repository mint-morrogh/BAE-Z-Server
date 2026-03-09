class dog_shed_wooden extends dog_shed_big
{
	void dog_shed_wooden()
	{
		
	}
	
	override string GetConstructionKitType()
	{
		return "dog_shed_wooden_kit";
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionOpendogshed_door);
		AddAction(ActionClosedogshed_door);
    }
}