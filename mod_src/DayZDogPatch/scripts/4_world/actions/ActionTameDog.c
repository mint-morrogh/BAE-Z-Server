class ActionTameDog: ActionInteractBase
{
	void ActionTameDog()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "#STR_DD_feeddog";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) && player.GetItemInHands() )
		{
			if (player.GetItemInHands().GetType() == "Bone") //jidlo, kosti
			{
				return true;
			}
			
		}
		return false;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{	
		//ItemBase item = action_data.m_Player.GetItemInHands();
		//action_data.m_Player.PhysicalPredictiveDropItem(item);
		Dayz_Doggo dog = Dayz_Doggo.Cast(action_data.m_Target.GetObject());
		//ItemBase neco = ItemBase.Cast(GetGame().CreateObject("Bone", dog.ModelToWorld("0 0 1") ));
		vector dst_transform[4];
		dog.GetTransform(dst_transform);
		//neco.DeleteSave(); 
		
		dst_transform[3] = dog.ModelToWorld("0 0 1");
		auto item = action_data.m_Player.GetItemInHands();
		auto source = new InventoryLocation;
		auto destination = new InventoryLocation;

		item.GetInventory().GetCurrentInventoryLocation(source);
		destination.SetGround(item, dst_transform);

		InventoryMode mode = InventoryMode.PREDICTIVE;
		if (action_data.m_Player.NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), action_data.m_Player))
		{
			mode = InventoryMode.JUNCTURE;
		}

		action_data.m_Player.GetHumanInventory().RedirectToHandEvent(mode, source, destination);
		//asi premenit na jiny item at to nejde sebrat
		
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		ItemBase item = action_data.m_Player.GetItemInHands();
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();

		Dayz_Doggo dog = Dayz_Doggo.Cast(action_data.m_Target.GetObject());
		dog.SetDoggoCmd(DoggoCmd.EATING);
		//item.SetPosition(dog.ModelToWorld("1 0 0"));
		
	}

};