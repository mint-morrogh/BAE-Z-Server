class BBP_FollowMe: ActionInteractBase
{
	void BBP_FollowMe()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "#STR_DD_followme";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) )
		{
			//Print("FOLLOW OWNER ID " + body_EAI.GetOwnerId() + " player " + player.GetDogOwnerID() );
			if((body_EAI.GetDoggoCmd() != DoggoCmd.FOLLOW ) && body_EAI.GetOwnerId() == player.GetDogOwnerID())
			{
				return true;
			}
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		target_dog.SetDoggoCmd(DoggoCmd.FOLLOW);
		/*
		if (target_dog.GetType() == "Doggo_Follow"+action_data.m_Player.GetDogIndex())
		{
			target_dog.SetDoggoCmd(DoggoCmd.FOLLOW);
			return;
		}
		targetObject.SetPosition(vector.Zero);
		GetGame().ObjectDelete( targetObject );
		
		target_dog = GetGame().CreateObject("Doggo_Follow"+action_data.m_Player.GetDogIndex(), targetObjectPos, false, true );
		//GetRPCManager().SendRPC("dayz_dog", "Set_My_Dog", new Param1< PlayerBase >( action_data.m_Player ), false, NULL, target_dog);

		//needs to be an rpc
		target_dog.SetOwnerId(action_data.m_Player.GetID());
		action_data.m_Player.setMyDog(target_dog);
		target_dog.SetDoggoCmd(DoggoCmd.FOLLOW);
		//Print("DOG AI " + target_dog.GetAIAgent() + " " + target_dog.GetCreatureAIType());
		//GetGame().RemoteObjectCreate( target_dog );
		*/
	}
};