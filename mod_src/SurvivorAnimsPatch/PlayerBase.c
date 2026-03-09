modded class PlayerBase
{
	protected int m_SAEmoteID;
	protected int m_FacialState;
	
	override void Init()
	{
		super.Init();
		m_FacialState = -1;
	}
	
	override void SetActions() 
	{
		super.SetActions();
		AddAction(ActionSearchPlayerAtWall);
	}
	int GetSAEmoteID()
	{
		return m_SAEmoteID;
	}
	/*
	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{
		super.CommandHandler(pDt,pCurrentCommandID,pCurrentCommandFinished);
		
		EmoteManager em = GetEmoteManager();
		if (em && em.IsEmotePlaying() ) //&& em.GetGesture() && em.GetGesture() == EMOTE_SAHandsOnWall) 
		{
			//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Playing EMOTE", ""));
			if (em.m_Callback && em.m_Callback.m_callbackID == EmoteSurAnmHandsOnWall.CMD_GESTUREFB_HandsOnWall)
			{
				//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Callback EMOTE " + em.m_Callback.m_callbackID, ""));
				m_SAEmoteID = em.m_Callback.m_callbackID;
			} else
			{
				m_SAEmoteID = 0;
			}
		}
		else
		{
			m_SAEmoteID = 0;
			//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "NO EMOTE", ""));
		}

	}
	*/
	
	int GetFacialState()
	{
		return m_FacialState;
	}
	
	void SetFacialState(int state)
	{
		m_FacialState = state;
	}
	
	override bool ModCommandHandlerAfter (float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		// BAE-Z patch: block ALL ModCommandHandlerAfter processing on
		// non-local entities on client. Expansion AI proxies trigger
		// engine-level "GetHealth01 cannot be called on client" errors
		// that accumulate and destabilize the engine, eventually causing
		// access violation crashes (e.g. WeaponManager::GetCurrentModeName).
		// Guard MUST be before super to prevent the entire chain.
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
		{
			if (GetGame().GetPlayer() != this)
				return false;
		}

		//! MUST BE HERE TO NOT DISABLE OTHER MODS
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}

		if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE)
		{

			HumanAnimInterface 	hai = GetAnimInterface();
			TAnimGraphVariable m_VarFacialState = hai.BindVariableInt("FacialState");
				
			AnimSetInt(m_VarFacialState, GetFacialState());
			return true;
		}

		return false;	// not handled
	}
	
	override void OnParticleEvent(string pEventType, string pUserString, int pUserInt)
	{
		// BAE-Z patch: guard against bone/health access on AI proxies
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
		{
			if (GetGame().GetPlayer() != this)
			{
				super.OnParticleEvent(pEventType, pUserString, pUserInt);
				return;
			}
		}

		super.OnParticleEvent(pEventType ,pUserString, pUserInt);

		if (!GetGame().IsDedicatedServer())
		{
			if (pUserInt == 85501)
			{
				PlayerBase player = PlayerBase.Cast(this);
				int boneIdx = player.GetBoneIndexByName("RightWristExtra");

				if (boneIdx != -1)
				{
					EffectParticle eff;

					eff = new EffMoneyRain();

					vector wp = player.GetBonePositionWS(boneIdx);

					eff.SetDecalOwner(player);
					eff.SetAutodestroy(true);
					SEffectManager.PlayInWorld(eff, wp);
				}

			}

		}
	}
	
}