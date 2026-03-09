class ActionClosedogshed_door: ActionInteractBase
{
	void ActionClosedogshed_door()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#close";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			dog_shed_big dogshed = dog_shed_big.Cast( targetObject );
			string selection = dogshed.GetActionComponentName(target.GetComponentIndex());
			
			if ( dogshed && dogshed.CanClosedogshed_door() && selection == "wall_interact" )
			{
				return true;
			}
		}
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		dog_shed_big dogshed = dog_shed_big.Cast( action_data.m_Target.GetObject() );
		dogshed.Closedogshed_door();
	}
}