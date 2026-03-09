class ActionBandageDogCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float effectivity = m_ActionData.m_MainItem.GetBandagingEffectivity();
		float adjusted_time_spent;
		if (effectivity > 0)
			adjusted_time_spent = UATimeSpent.BANDAGE / effectivity;
		m_ActionData.m_ActionComponent = new CAContinuousTime(adjusted_time_spent);
	}
};

class ActionBandageDog: ActionContinuousBase

{
	void ActionBandageDog()
	{
		m_CallbackClass = ActionBandageDogCB;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#STR_DD_bandagedog";
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTObject(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Dayz_Doggo dog = Dayz_Doggo.Cast(target.GetObject());
		// only if needed heal
		
		if (dog && dog.GetDoggoCmd() == DoggoCmd.STAY && dog.IsDogWounded() )
		{
			return true;
		}
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		Dayz_Doggo target = Dayz_Doggo.Cast(action_data.m_Target.GetObject());
		
		if(action_data.m_MainItem && target)
		{
			ApplyBandage( action_data.m_Player, action_data.m_MainItem, target );
			action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
	
	override void OnFinishProgressClient( ActionData action_data )
	{
		GetGame().GetAnalyticsClient().OnActionBandageTarget();
	}
	
	void ApplyBandage( PlayerBase player, ItemBase item, Dayz_Doggo dog )
	{	
		// heal dog
		dog.SetHealth("", "", dog.GetMaxHealth("", ""));
		dog.SetHealth("", "Blood", dog.GetMaxHealth("", "Blood"));
		player.StoreDogHealth();
		if (item.HasQuantity())
		{
			item.AddQuantity(-1,true);
		}
		else
		{
			item.Delete();
		}
	}
	
};