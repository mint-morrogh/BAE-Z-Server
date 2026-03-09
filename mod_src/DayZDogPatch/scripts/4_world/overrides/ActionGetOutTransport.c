modded class ActionGetOutTransport
{
	override void OnEndServer( ActionData action_data )
	{
		if (action_data.m_Player.HaveDog())
		{
			if (action_data.m_Player.HaveDog() && !action_data.m_Player.GetMyDog())
			{
				//get dog props
				Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+action_data.m_Player.GetDogIndex(), action_data.m_Player.ModelToWorld("0 0 1.2"), false, true ));
				target_dog.SetOwnerId(action_data.m_Player.GetID());
				action_data.m_Player.setMyDog(target_dog);
				target_dog.SetDoggoCmd(DoggoCmd.FOLLOW);
				action_data.m_Player.RestoreDogInventory();
				action_data.m_Player.RestoreDogHealth();
				DoggoLogger.Log("Respawned (get out transport) dog entity ID " + target_dog.GetID() + " of owner " + action_data.m_Player.GetIdentity().GetName() + " at " + target_dog.GetPosition());
			}

		}
	
		super.OnEndServer(action_data);
	}
};