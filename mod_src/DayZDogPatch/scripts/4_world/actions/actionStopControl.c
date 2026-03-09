class ActionStopControlDog: ActionInteractBase
{
	void ActionStopControlDog()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		//m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_HEADLIGHT;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_ALL;
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		//m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "Stop control doggo";
		//m_FullBody = true;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool HasProgress()
	{
		return false;
	}
	
	/*
	override typename GetInputType()
	{
		return ToggleLightsActionInput;
	}
	*/
	
	override bool HasTarget()
	{
		return false;
	}
	/*
	override void Start(ActionData action_data)
	{
		super.Start( action_data );
		action_data.m_Player.TryHideItemInHands(false);
		action_data.m_Player.SetPlayerHidden(false);
		action_data.m_Player.UnlinkFromLocalSpace();
		dBodyEnableGravity(IEntity.Cast(action_data.m_Player), true);	
		if (action_data.m_Player.GetInventory())
		{
			action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}
		
	}
	*/
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if (player.GetParent() && player.GetParent().IsInherited(Dayz_Doggo))
		{
			return true;
		}
		
		return false;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		//action_data.m_Player.UnlinkFromLocalSpace();
		action_data.m_Player.TryHideItemInHands(false);
		action_data.m_Player.SetPlayerHidden(false);
		action_data.m_Player.UnlinkFromLocalSpace();
		dBodyEnableGravity(action_data.m_Player, true);	
		if (action_data.m_Player.GetInventory())
		{
			action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}
		//camera return
		GetGame().SelectPlayer(action_data.m_Player.GetIdentity(), action_data.m_Player);
	}

	override void OnExecuteServer( ActionData action_data )
	{
		//action_data.m_Player.UnlinkFromLocalSpace();
		//dBodyEnableGravity(IEntity.Cast(action_data.m_Player), true);
		action_data.m_Player.TryHideItemInHands(false);
		action_data.m_Player.SetPlayerHidden(false);
		action_data.m_Player.UnlinkFromLocalSpace();
		dBodyEnableGravity(action_data.m_Player, true);	
		if (action_data.m_Player.GetInventory())
		{
			action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
		}
		action_data.m_Player.GetMyDog().SetDoggoCmd(DoggoCmd.STAY);
	}
	
};