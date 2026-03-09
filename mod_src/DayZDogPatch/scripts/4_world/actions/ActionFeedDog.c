class ActionFeedDog: ActionInteractBase
{
	void ActionFeedDog()
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
		EntityAI mask_ent, mouth_ent;
		Class.CastTo(body_EAI, target.GetObject());
		TStringArray dogmeal = {"Bone", "DeerSteakMeat", "CowSteakMeat", "PigSteakMeat", "GoatSteakMeat", "BoarSteakMeat", "BearSteakMeat", "SheepSteakMeat", "RabbitLegMeat", "ChickenBreastMeat"};
		//DoggoLogger.Log("feed " + dogmeal.Find("Bone") + " x " + dogmeal.Find("Kokot"));
		if (body_EAI)
		{
			mask_ent = body_EAI.FindAttachmentBySlotName("DogMask");
			mouth_ent = body_EAI.FindAttachmentBySlotName("DogMouth");
		}
		if (!mask_ent && !mouth_ent && body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) && player.GetItemInHands() && body_EAI.GetDoggoCmd() != DoggoCmd.EATING && body_EAI.GetDoggoCmd() != DoggoCmd.TAME)
		{
			if (dogmeal.Find(player.GetItemInHands().GetType()) > -1) //jidlo, kosti
			{
				if (body_EAI.IsWild() || body_EAI.GetOwnerId() == player.GetDogOwnerID())
				{
					if (!player.HaveDog() && player.GetLockedTame())
					{
						return false;
					}
					return true;
				}
				
			}
			
		}
		return false;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{	
		action_data.m_Player.SetLockedTame(true);
		//ItemBase item = action_data.m_Player.GetItemInHands();
		//action_data.m_Player.PhysicalPredictiveDropItem(item);
		Dayz_Doggo dog = Dayz_Doggo.Cast(action_data.m_Target.GetObject());
		//ItemBase neco = ItemBase.Cast(GetGame().CreateObject("Bone", dog.ModelToWorld("0 0 1") ));
		vector dst_transform[4];
		dog.GetTransform(dst_transform);
		//neco.DeleteSave(); 
		if (dog.GetSpeed().Normalize() == 0)
		{
			dst_transform[3] = dog.ModelToWorld("0 0 1");
		}
		else {
			dst_transform[3] = dog.ModelToWorld("0 0 1.5");
		}
		
		auto item = action_data.m_Player.GetItemInHands();
		auto source = new InventoryLocation;
		auto destination = new InventoryLocation;
		ClearInventoryReservationEx(action_data);
		item.GetInventory().GetCurrentInventoryLocation(source);
		destination.SetGround(item, dst_transform);

		InventoryMode mode = InventoryMode.PREDICTIVE;
		if (action_data.m_Player.NeedInventoryJunctureFromServer(item, item.GetHierarchyParent(), action_data.m_Player))
		{
			mode = InventoryMode.JUNCTURE;
		}

		action_data.m_Player.GetHumanInventory().RedirectToHandEvent(mode, source, destination);
		//asi premenit na jiny item at to nejde sebrat
		
		//pak dat jinam
		if (!action_data.m_Player.HaveDog() && dog.IsWild())
		{
			Particle m_ParticleTaming = Particle.PlayOnObject(ParticleList.DOGGO_HEARTS, dog, Vector(0, 0.7, 0.8));
		}
		
		

		
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		ItemBase item = action_data.m_Player.GetItemInHands();
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();

		Dayz_Doggo dog = Dayz_Doggo.Cast(action_data.m_Target.GetObject());
		int prevcmd = dog.GetDoggoCmd();
		dog.SetDoggoCmd(DoggoCmd.EATING);
		dog.SetStartEating(item, action_data.m_Player, prevcmd);
		//item.SetLifetime(18);
		//item.SetPosition(dog.ModelToWorld("1 0 0"));
		
		//pokud je wild uloz id hrace co ho krmi
		//pokud hrac nema psa, ochoci si ho tim po uplynuti casu
		//timer se spusti jak na itemu tak na psovi
		//po dobu behu timeru item nepujde  sebrat
		
		
	}

};