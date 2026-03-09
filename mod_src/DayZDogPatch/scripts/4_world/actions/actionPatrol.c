class BBP_Patrol: ActionInteractBase
{
	void BBP_Patrol()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "#STR_DD_patrol";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase player_me = PlayerBase.Cast(GetGame().GetPlayer());

		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) )
		{
			if((body_EAI.GetDoggoCmd() != DoggoCmd.PATROL )&& body_EAI.GetOwnerId() == player.GetDogOwnerID())
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
		target_dog.SetDoggoCmd(DoggoCmd.PATROL);
		/*
		if (target_dog.GetType() == "Doggo_Patrol"+action_data.m_Player.GetDogIndex())
		{
			target_dog.SetDoggoCmd(DoggoCmd.PATROL);
			return;
		}
		
		targetObject.SetPosition(vector.Zero);
		GetGame().ObjectDelete( targetObject );

		target_dog = GetGame().CreateObject("Doggo_Patrol"+action_data.m_Player.GetDogIndex(), targetObjectPos, false, true );

		//needs to be an rpc
		action_data.m_Player.setMyDog(target_dog);
		target_dog.SetOwnerId(action_data.m_Player.GetID());
		target_dog.SetDoggoCmd(DoggoCmd.PATROL);
		//GetGame().RemoteObjectCreate( target_dog );
		*/
	}
};