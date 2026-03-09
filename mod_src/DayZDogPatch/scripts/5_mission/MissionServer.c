modded class MissionServer
{

	void MissionServer()
    {
		DoggoConfig config;
		DayzDogConfig.LoadConfig("DayzDog.json", config);
        GetDayZGame().SetDayzDogConfig(config);
   		//AIBehaviourHL.RegAIBehaviour("kokot",AIBehaviourHLKokot2,AIBehaviourHLDataKokot2);
		if (GetGame().IsServer()) 
        {
			DogServerRPC.ClearInstance();
			DogServerRPC.GetInstance();
        }
		
    }
	/*
	override void InvokeOnDisconnect( PlayerBase player )
	{
		if (player.HaveDog() && player.GetMyDog())
		{
			player.SetDogLastPos(player.GetMyDog().GetPosition());
			player.GetMyDog().SetDoggoCmd(DoggoCmd.STAY);
			if (player.GetIdentity())
			{
				DoggoLogger.Log("Stored " + (player.GetMyDog().GetInventory().CountInventory() - 1).ToString() + " items in dog inventory of Player " + player.GetIdentity().GetName());
			}
			else
			{
				DoggoLogger.Log("Stored " + (player.GetMyDog().GetInventory().CountInventory() - 1).ToString() + " items in dog inventory of player which lost connection");
			}
			player.StoreDogInventory();
			player.StoreDogHealth();
			GetGame().ObjectDelete( player.GetMyDog() );
		}
		super.InvokeOnDisconnect( player );
	}
	*/
	
	override void PlayerDisconnected(PlayerBase player, PlayerIdentity identity, string uid)
	{
		if (player.HaveDog() && player.GetMyDog())
		{
			player.SetDogLastPos(player.GetMyDog().GetPosition());
			player.GetMyDog().SetDoggoCmd(DoggoCmd.STAY);
			if (player.GetIdentity())
			{
				DoggoLogger.Log("Stored " + (player.GetMyDog().GetInventory().CountInventory() - 1).ToString() + " items in dog inventory of Player " + player.GetIdentity().GetName());
			}
			else
			{
				DoggoLogger.Log("Stored " + (player.GetMyDog().GetInventory().CountInventory() - 1).ToString() + " items in dog inventory of player which lost connection");
			}
			player.StoreDogInventory();
			player.StoreDogHealth();
			GetGame().ObjectDelete( player.GetMyDog() );
		}
		
		super.PlayerDisconnected(player, identity, uid);
	}
	
};