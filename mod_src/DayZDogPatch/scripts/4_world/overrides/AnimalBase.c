modded class AnimalBase
{
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		
		Dayz_Doggo dog = Dayz_Doggo.Cast(ai);
		if (dog && (dog.GetDoggoCmd() == DoggoCmd.MISSION || dog.GetDoggoCmd() == DoggoCmd.FOLLOW || dog.GetDoggoCmd() == DoggoCmd.STAY || dog.GetDoggoCmd() == DoggoCmd.PLRCONTROL ) )
		{
			return false;
		}
		return super.CanBeTargetedByAI( ai );
	}
	
	override void EEKilled( Object killer )
	{

		Dayz_Doggo dog = Dayz_Doggo.Cast(killer);
		if (dog)
		{
			if (dog.GetOwnerId())
			{
				PlayerBase player = GetPlayerByEntityID(dog.GetOwnerId());
				if (player)
				{
					EntityAI dogslot = player.FindAttachmentBySlotName("Dog");
					if (dogslot)
					{
						DogPreview_Base dogpr = DogPreview_Base.Cast(dogslot);
						dogpr.AddAnimalKill();
						if (this.IsInherited(Animal_GallusGallusDomesticus))
						{
							dogpr.AddChickenKill();
						}
					}
				}
			}
			
		}
		
		super.EEKilled( killer );
	}
	//I should make that in some helper class :)
	protected static PlayerBase GetPlayerByEntityID(int entityID) //bylo protected static
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetID() == entityID)
			{
				return PlayerBase.Cast(players.Get(i));
			}
		}
		return null;
	}
}