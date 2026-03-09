class ActionOpendogshed_door: ActionInteractBase
{
	void ActionOpendogshed_door()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#open";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		//Print("target " + targetObject);
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			dog_shed_big dogshed = dog_shed_big.Cast( targetObject );
			string selection = dogshed.GetActionComponentName(target.GetComponentIndex());
			
			if ( dogshed && dogshed.CanOpendogshed_door() && selection == "wall_interact" )
			{
				return true;
			}
		}
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		dog_shed_big dogshed = dog_shed_big.Cast( action_data.m_Target.GetObject() );
		dogshed.Opendogshed_door();
	}
}