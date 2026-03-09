class ActionManageDog: ActionInteractBase
{
	void ActionManageDog()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "Dog Stats";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase player_me = PlayerBase.Cast(GetGame().GetPlayer());

		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		DoggoConfig config = GetDayZGame().GetDayzDogConfig();
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  body_EAI.IsInherited(Dayz_Doggo) )
		{
			if((body_EAI.GetDoggoCmd() != DoggoCmd.PLRCONTROL )&& body_EAI.GetOwnerId() == player.GetDogOwnerID())
			{
				return true;
			}
		}
		return false;
	}
	
	override void Start(ActionData action_data)
	{
		super.Start( action_data );
		
		if (GetGame().IsServer())
		{
			Object targetObject = action_data.m_Target.GetObject();				
			vector targetObjectPos = targetObject.GetPosition();
			
			Dayz_Doggo target_dog;
			Class.CastTo(target_dog, targetObject);
			DogPreview_Base dogpr = DogPreview_Base.Cast(action_data.m_Player.GetDogSlot());
			string dogname = dogpr.GetDogName();
			GetRPCManager().SendRPC( "Dayz_Dog", "SyncDogName", new Param1<string>(dogname) );
		}

		
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();	
		//GetGame().SelectSpectator( GetGame().GetPlayer().GetIdentity() , "DoggoCamera", (targetObject.ModelToWorld("0 1 -1")));
		//DoggoCamera.Cast(Camera.GetCurrentCamera()).SetTarget(targetObject);
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		action_data.m_Player.setMyDog(target_dog);
		if (!GetGame().GetUIManager().FindMenu(DOG_MANAGE_MENU))
		{
			GetGame().GetUIManager().EnterScriptedMenu(DOG_MANAGE_MENU, NULL);
		}

	}

	override void OnExecuteServer( ActionData action_data )
	{	
		// LATER USE RPC FROM MENU
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();
		
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		DogPreview_Base dogpr = DogPreview_Base.Cast(action_data.m_Player.GetDogSlot());
		string dogname = dogpr.GetDogName();
		//GetRPCManager().SendRPC( "Dayz_Dog", "SyncDogName", new Param1<string>(dogname) );
		//target_dog.SetDoggoCmd(DoggoCmd.MISSION);
	}
};