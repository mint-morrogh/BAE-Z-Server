modded class ActionGetInTransport
{
	override void OnEndServer( ActionData action_data )
	{
		if(action_data.m_Player.HaveDog())
		{
			if (action_data.m_Player.GetMyDog() && action_data.m_Player.GetMyDog().GetDoggoCmd() == DoggoCmd.FOLLOW)
			{
				//save dog props
				action_data.m_Player.StoreDogInventory();
				action_data.m_Player.StoreDogHealth();
				DoggoLogger.Log("Deleted (get in transport) dog of owner " + action_data.m_Player.GetIdentity().GetName() + " at " + action_data.m_Player.GetMyDog().GetPosition());
				GetGame().ObjectDelete(action_data.m_Player.GetMyDog());
				/*
				//Dayz_Doggo dog = action_data.m_Player.GetMyDog();
				//dJointCreateFixed(action_data.m_Player, dog, "0 2.2 -2", "0 0 0", false, 0.1);
				
				//Dayz_Doggo dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow1", vector.Zero, false, false )); 
				Transport trans = Transport.Cast(action_data.m_Target.GetObject());
				vector offset = "2 0 2";
				Dayz_Doggo dog = action_data.m_Player.GetMyDog();
				dog.SetPosition( offset );
				//dog.GetAIAgent().SetKeepInIdle(true);
				//dog.SetDoggoCmd(DoggoCmd.STAY);
				dog.DisableSimulation(true);
				trans.AddChild( dog, -1, false ); //CRASHING CLIENT
				trans.Update();
				*/
			}
		}
		super.OnEndServer(action_data);
	}
	/*
	override void OnEndClient( ActionData action_data )
	{
		if(action_data.m_Player.HaveDog())
		{
			if (action_data.m_Player.GetMyDog() && action_data.m_Player.GetMyDog().GetDoggoCmd() == DoggoCmd.FOLLOW)
			{
				///GetGame().ObjectDelete(action_data.m_Player.GetMyDog());
				
				//Dayz_Doggo dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow1", vector.Zero, false, false )); 
				Transport trans = Transport.Cast(action_data.m_Target.GetObject());
				vector offset = "2 0 2";
				Dayz_Doggo dog = action_data.m_Player.GetMyDog();
				dog.SetPosition( offset );
				//dog.GetAIAgent().SetKeepInIdle(true);
				//dog.SetDoggoCmd(DoggoCmd.STAY);
				dog.DisableSimulation(true);
				trans.AddChild( dog, -1, false ); //CRASHING CLIENT
				trans.Update();
				
			}
		}
		super.OnEndClient(action_data);
	}
	*/
};