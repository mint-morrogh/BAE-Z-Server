class ActionSetDogMission: ActionInteractBase
{
	void ActionSetDogMission()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "#STR_DD_set_mission";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase player_me = PlayerBase.Cast(GetGame().GetPlayer());

		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		DoggoConfig config = GetDayZGame().GetDayzDogConfig();
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) && config.EnableDogWaypoints )
		{
			if((body_EAI.GetDoggoCmd() != DoggoCmd.PLRCONTROL )&& body_EAI.GetOwnerId() == player.GetDogOwnerID())
			{
				return true;
			}
		}
		return false;
	}
	override void OnExecuteClient( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();	
		//GetGame().SelectSpectator( GetGame().GetPlayer().GetIdentity() , "DoggoCamera", (targetObject.ModelToWorld("0 1 -1")));
		//DoggoCamera.Cast(Camera.GetCurrentCamera()).SetTarget(targetObject);
		if (!GetGame().GetUIManager().FindMenu(DOG_MAP_MENU))
		{
			GetGame().GetUIManager().EnterScriptedMenu(DOG_MAP_MENU, NULL);
		}

	}

	override void OnExecuteServer( ActionData action_data )
	{	
		// LATER USE RPC FROM MENU
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();
		
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		//target_dog.SetDoggoCmd(DoggoCmd.MISSION);
	}
};