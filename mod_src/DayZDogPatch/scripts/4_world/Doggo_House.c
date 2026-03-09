class Doggo_House extends Inventory_Base
{

	override bool CanPutInCargo( EntityAI parent )
    {
        return false;
    }
    
    override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
    }
	
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionSummonDogMenu);
	} 
	
};
/*
modded class Bone
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionSummonDogMenu);
	} 
};
*/