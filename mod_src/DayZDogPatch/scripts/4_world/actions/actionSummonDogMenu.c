class ActionSummonDogMenu: ActionInteractBase
{

	void ActionSummonDogMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		//m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}
	/*
	override void CreateConditionComponents()
    {
       	m_ConditionItem = new CCINonRuined;
    	//m_ConditionTarget = new CCTNone;
		//m_ConditionTarget = new CCTCursor(UAMaxDistances.DEFAULT);
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
    }
	*/
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (GetGame().IsClient())
		{
			if (GetGame().GetUIManager().FindMenu(DOG_SELECT_MENU) )
				return false;		
		}
		
		if (!player.GetItemInHands())
		{
			return false;
		}
		
		Doggo_House doghouse;
		dog_shed_big dogshedbig;
		dog_shed_small dogshedsmall;
		Class.CastTo(doghouse, target.GetObject());
		Class.CastTo(dogshedbig, target.GetObject());
		Class.CastTo(dogshedsmall, target.GetObject());
		bool haveitem = false;
		DoggoConfig config = GetDayZGame().GetDayzDogConfig();
		//Print("ITEM IN HANDS " + player.GetItemInHands().GetType() ) ;
		//rewrite config synchronization	
		if (config.SummonItem)
		{
			if (config.SummonItem == player.GetItemInHands().GetType())
			{
				haveitem = true;
			} else {
				haveitem = false;
			}
		} else {
			if (player.GetItemInHands().GetType() == "Bone")
			{
				haveitem = true;
			} else {
				haveitem = false;
			}
		}

		if ( (doghouse || dogshedbig || dogshedsmall) && !player.HaveDog() && haveitem)
		{
			return true;
		}
		return false;
	}
	/*
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	*/
	override string GetText()
	{
		return "#DD_summon_dog";
	}
	

	override void OnExecuteClient( ActionData action_data )
	{
		//ClearInventoryReservationEx(action_data);
		
		if (!GetGame().GetUIManager().FindMenu(DOG_SELECT_MENU))
			{
				GetGame().GetUIManager().EnterScriptedMenu(DOG_SELECT_MENU, NULL);
				Object targetObject = action_data.m_Target.GetObject();
				vector dogspawn;
				if (targetObject.IsInherited(dog_shed_big))
				{
					dogspawn = targetObject.ModelToWorld("0 0.2 0");
				}				
				else if (targetObject.IsInherited(Doggo_House) || targetObject.IsInherited(dog_shed_small)){
			    	dogspawn = targetObject.ModelToWorld("2 0 0");
				}
				action_data.m_Player.SetDogSpawnPos(dogspawn);
			}	
	}
	
	/*
	void OnExecuteServer( ActionData action_data )
	{	
		//EntityAI item = action_data.m_Player.GetItemInHands();
		//action_data.m_Player.GetInventory().DropEntity(InventoryMode.SERVER, action_data.m_Player, item);
		//item.DeleteSave();
	}
	*/
	override void OnEndServer( ActionData action_data )
	{
		EntityAI item = action_data.m_Player.GetItemInHands();
		if (item)
		{
			item.Delete();
		}
	}
	
	override void OnEndClient( ActionData action_data )
	{
		EntityAI item = action_data.m_Player.GetItemInHands();
		if (item)
		{
			item.DeleteOnClient();
		}
	}

};