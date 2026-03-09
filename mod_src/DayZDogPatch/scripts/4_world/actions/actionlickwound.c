class ActionLickWoundCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1.5);
	}
};

class ActionLickWound: ActionBandageBase
{	
	void ActionLickWound()
	{
		m_CallbackClass = ActionLickWoundCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_SALINEBLOODBAGTARGET;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}

	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINotPresent;
		m_ConditionTarget = new CCTDummy( );
	}

	override bool HasTarget()
	{
		return true;
	}
		
	override string GetText()
	{
		return "Lick Wound";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		
		if(body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)))
		{
			PlayerBase target_player = PlayerBase.Cast(GetGame().GetPlayer());
			if(target_player)
			{
				return target_player.IsBleeding();
			}
		}
		return false;	
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase target = PlayerBase.Cast(action_data.m_Player);
		if(target)
		{
			ApplyBandage( NULL, target );
			//action_data.m_Player.GetSoftSkillsManager().AddSpecialty( m_SpecialtyWeight );
		}
	}
};
