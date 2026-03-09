class BBP_StayHere: ActionInteractBase
{
	void BBP_StayHere()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "#STR_DD_stayhere";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) )
		{
			if((body_EAI.GetDoggoCmd() != DoggoCmd.STAY ) && body_EAI.GetOwnerId() == player.GetDogOwnerID())
			{
				return true;
			}
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();				
		//vector targetObjectPos = targetObject.GetPosition();
		
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		
		/*	
		targetObject.SetPosition(vector.Zero);
		GetGame().ObjectDelete( targetObject );

		target_dog = GetGame().CreateObject("Doggo_Stay"+action_data.m_Player.GetDogIndex(), targetObjectPos, false, true );
		target_dog.SetOwnerId(action_data.m_Player.GetID());
		action_data.m_Player.setMyDog(target_dog);
		target_dog.Update();
		*/
		target_dog.SetDoggoCmd(DoggoCmd.STAY);
		
		//GetGame().RemoteObjectCreate( target_dog );
		// pridat majitele
		//action_data.m_Player.GetIdentity().GetPlainId()
		//needs to be an rpc
		//action_data.m_Player.resetDog();
	}
};