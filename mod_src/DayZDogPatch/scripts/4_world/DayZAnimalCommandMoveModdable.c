

modded class DayZAnimalCommandMoveModdable
{
	override void PreAnimUpdate( float pDt )
	{
		Dayz_Doggo dog = Dayz_Doggo.Cast(GetEntity());
		if (dog && dog.GetAnimST())
		{
			//behaviour slot 3 - enraged
			//dog.GetAnimST().SetBehaviorSlotIDM(this, 3);
			//dog.GetAnimST().SetAlertLevelM(this,3);
			dog.GetAnimST().SetSwimingM(this, dog.ShouldSwim());
			/*
			if (GetGame().GetWaterDepth( dog.GetPosition() ) > 0.6)
			{
				dog.GetAnimST().SetSwimingM(this, true);
			}
			else
			{
				dog.GetAnimST().SetSwimingM(this, false);
			}
			*/
			
		} 
		super.PreAnimUpdate( pDt );
		
		
	}
	
}

modded class DayZAnimalCommandAttackModdable
{
	override void PreAnimUpdate( float pDt )
	{
		Dayz_Doggo dog = Dayz_Doggo.Cast(GetEntity());
		if (dog && dog.ShouldDogAttackLow() && dog.GetAnimST())
		{
			dog.GetAnimST().CallAttackVanilla( this, 6 , 0);
			
		} else {
			super.PreAnimUpdate( pDt );
		}
	}
}